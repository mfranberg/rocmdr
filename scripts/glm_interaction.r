sink( "/dev/null" )

library( snpMatrix, quietly = T )
library( lmtest, quietly = T )
library( multicore, quietly = T )

sink( )

argv = commandArgs( trailingOnly = TRUE )
if ( length( argv ) != 2 )
{
    message( "glm_interaction: Wrong number of arguments." )
    message( "Usage: glm_interaction pairs plink_file" )
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
    snps = 2 - as( snps, "numeric" )
    
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
# Computes a p-value for the interaction term in a logistic
# regression model.
#
# @param pair A pair of snps, that will be indexed in the global snp matrix.
#             It is global to avoid copies of it.
#
# @return The computed p-value.
#
compute_pvalue = function(pair)
{   
    snp1 = snps[ ,pair[ 1 ] ]
    snp2 = snps[ ,pair[ 2 ] ]
    
    model_data = data.frame( snp1, snp2, phenotype )
    
    result = glm( phenotype ~ snp1 + snp2 + snp1 * snp2,
                  family = binomial( "logit" ), data = model_data )
    stat = summary( result )
    
    return( stat$coefficients[ 4, 4 ] )
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

