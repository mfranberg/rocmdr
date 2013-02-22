sink( "/dev/null" ) # Make sure package do not output anything to stdout

library( snpMatrix, quietly = T )
library( lmtest, quietly = T )
library( multicore, quietly = T )

sink( )

argv = commandArgs( trailingOnly = TRUE )
if ( length( argv ) != 2 )
{
    message( "ll_interaction: Wrong number of arguments." )
    message( "Usage: ll_interaction pairs plink_file" )
    quit( )
}

##
# Read a binary plink file from the given path prefix.
#
# @param plink_path Path to plink file without file ending.
#
# @return Snps as a matrix coded as 0, 1, 2 and 3 for missing.
#
read_snps = function(plink_path)
{
    plink_path_list = paste( plink_path, c( ".bed", ".bim", ".fam" ), sep = "" )
    
    sink( "/dev/null" ) # SNP matrix outputs some junk here, remove it
    snps = read.plink( plink_path_list[ 1 ],
                       plink_path_list[ 2 ],
                       plink_path_list[ 3 ] )
    sink( )
    
    # Make 2 major allele, for some reason it is usually coded as such.
    snps = 2 - as.numeric( snps )
    
    return( snps )
}

##
# Reads binary phenotypes from the plink .fam file.
#
# @param plink_path Path to plink files without file ending.
#
# @return List of phenotypes coded as 0, 1, 2
#
read_phenotypes = function(plink_path)
{
    fam_path = paste( plink_path, ".fam", sep="" )
    phenotypes = read.table( fam_path, header = F )[ , 6 ]
    phenotypes[ phenotypes == 0 ] = NA
    
    return( phenotypes - 1 )
}

##
# Computes the log likelihood for the saturated log-linear model.
#
# For each cell count n_ijk the likelihood is computed
# as (n_ijk/n)^(n_ijk). The final likelihood is the product
# of these.
#
# @return Log likelihood of estimated model.
#
full_likelihood = function(snp1, snp2, phenotype)
{
    all = table( snp1, snp2, phenotype )
    p_full = all / sum( all )
    
    return( sum( all * log( p_full ) ) )
}

##
# Computes the log likelihood for the a partially associcated log-linear model.
# Where only one of the SNPs is related to the disease, and the other is correlated
# to this SNP.
#
# For each cell count n_ijk the likelihood is computed
# as ( ( (n_ij * n_ik ) / n_i) ) / n )^(n_ijk). The final likelihood is the product
# of these.
#
# @return Log likelihood of estimated model.
#
partial_likelihood = function(snp1, snp2, phenotype)
{
    all = table( snp1, snp2, phenotype )
    
    snp_snp_table = all
    snp_snp = table( snp1, snp2 )
    snp_snp_table[ ,, ] = snp_snp
    
    snp1_pheno_table = all
    snp1_pheno = table( snp1, phenotype )
    snp1_pheno_table[ , 1, ] = snp1_pheno
    snp1_pheno_table[ , 2, ] = snp1_pheno
    snp1_pheno_table[ , 3, ] = snp1_pheno
    
    snp1_table = all
    snp1 = table( snp1 )
    snp1_table[ , 1, 1 ] = snp1
    snp1_table[ , 2, 1 ] = snp1
    snp1_table[ , 3, 1 ] = snp1
    snp1_table[ , 1, 2 ] = snp1
    snp1_table[ , 2, 2 ] = snp1
    snp1_table[ , 3, 2 ] = snp1
    
    partial = snp_snp_table * snp1_pheno_table / snp1_table
    p_partial = partial / sum( partial )
    
    return( sum( all * log( p_partial ) ) )
}

##
# Computes the log likelihood for a block independent log-linear model.
#
# For each cell count n_ijk the likelihood is computed
# as ( (n_ij * n_k ) / (n^2) ) )^(n_ijk). The final likelihood is the product
# of these.
#
# @return Log likelihood of estimated model.
#
block_likelihood = function(snp1, snp2, phenotype)
{
    all = table( snp1, snp2, phenotype )
    
    snp_snp_table = all
    snp_snp = table( snp1, snp2 )
    snp_snp_table[ ,, ] = snp_snp
    
    pheno_table = all
    pheno = table( phenotype )
    pheno_table[ 1, 1,  ] = pheno
    pheno_table[ 2, 1,  ] = pheno
    pheno_table[ 3, 1,  ] = pheno
    pheno_table[ 1, 2,  ] = pheno
    pheno_table[ 2, 2,  ] = pheno
    pheno_table[ 3, 2,  ] = pheno
    pheno_table[ 1, 3,  ] = pheno
    pheno_table[ 2, 3,  ] = pheno
    pheno_table[ 3, 3,  ] = pheno
    
    block = snp_snp_table * pheno_table / sum( all )
    p_block = block / sum( block )
    
    return( sum( all * log( p_block ) ) )
}

##
# Computes the p-value for the log-linear model with the highest BIC.
#
# First BIC is computed for partial likelihood and block likelihood models,
# then the model with the highest BIC is compared to the saturated model
# with a likelihood ratio test. The p-value for the test is the returned.
#
# @param pair A pair of snps, that will be indexed in the global snp matrix.
#             It is global to avoid copies of it.
#
# @return The p-value of the model with heighest BIC.
#
compute_pvalue = function(pair)
{   
    snp1 = snps[ ,pair[ 1 ] ]
    snp2 = snps[ ,pair[ 2 ] ]
    
    # Add 1 extra count to each cell in the full likelihood
    snp1 = c( snp1, rep( c( 0, 1, 2), 6 ) )
    snp2 = c( snp2, rep( c( rep( 0, 3 ), rep( 1, 3 ), rep( 2, 3 ) ), 2 ) )
    phenotype = c( phenotype, rep( 0, 9 ), rep( 1, 9 ) )
    
    model_likelihood = 1:3
    model_likelihood[ 1 ] = partial_likelihood( snp1, snp2, phenotype )
    model_likelihood[ 2 ] = partial_likelihood( snp2, snp1, phenotype )
    model_likelihood[ 3 ] = block_likelihood( snp1, snp2, phenotype )
    
    # Number of parameters and degrees of freedom for each model
    k = c( 10, 10, 8 )
    df = c( 6, 6, 8 )
    
    bic = -2 * model_likelihood + k * log( length( phenotype ) )
    
    best_model = which.min( bic )
    LR = model_likelihood[ best_model ] - full_likelihood( snp1, snp2, phenotype )
    
    return( 1 - pchisq( -2*LR, df[ best_model ] ) )
}

pair_file = argv[ 1 ]
pairs = read.table( pair_file, header = F )
list_pairs = as.list( as.data.frame( t(pairs), stringsAsFactors = F ) )

plink_file = argv[ 2 ]
snps = read_snps( plink_file )
phenotype = read_phenotypes( plink_file )

pvalues = unlist( mclapply( list_pairs, compute_pvalue, mc.cores = 2 ) )

print_pairs = unlist( lapply( list_pairs, function(pair){ paste( pair[ 1 ], pair[ 2 ] ) } ) )
results = data.frame( print_pairs, pvalues )

write.table( results, row.names = FALSE, col.names = c( "Loci", "P-value" ), quote = F, sep = "\t" )

