library( snpMatrix, quietly = T )
library( lmtest, quietly = T )
library( multicore, quietly = T )

argv = commandArgs( trailingOnly = TRUE )
if ( length( argv ) != 2 )
{
    message( "glm_interaction: Wrong number of arguments." )
    message( "Usage: glm_interaction pairs plink_file" )
    quit( )
}

read_snps = function(plink_path)
{
    plink_path_list = paste( plink_path, c( ".bed", ".bim", ".fam" ), sep = "" )
    snps = read.plink( plink_path_list[ 1 ],
                       plink_path_list[ 2 ],
                       plink_path_list[ 3 ] )
    
    # Make 0 major allele.
    snps = 2 - as( snps, "numeric" )
    
    return( snps )
}

read_phenotypes = function(plink_path)
{
    fam_path = paste( plink_path, ".fam", sep="" )
    phenotypes = read.table( fam_path, header = F )[ , 6 ]
    
    return( phenotypes - 1 )
}

bind = function(FUN, ...)
{
    .orig = list( ... )
    function(...) do.call( FUN, c( .orig, list( ... ) ) )
}

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

compute_pvalue_dummy = function(pair)
{
    snp1 = snps[ ,pair[ 1 ] ]
    snp2 = snps[ ,pair[ 2 ] ]
    
    model_data = na.omit( data.frame( snp1, snp2, phenotype ) )
 
    dummy1 = ifelse( model_data$snp1 == 0 & model_data$snp2 == 0, 1, 0 )
    dummy2 = ifelse( model_data$snp1 == 0 & model_data$snp2 == 1, 1, 0 )
    dummy3 = ifelse( model_data$snp1 == 0 & model_data$snp2 == 2, 1, 0 )
    dummy4 = ifelse( model_data$snp1 == 1 & model_data$snp2 == 0, 1, 0 )
    dummy5 = ifelse( model_data$snp1 == 1 & model_data$snp2 == 1, 1, 0 )
    dummy6 = ifelse( model_data$snp1 == 1 & model_data$snp2 == 2, 1, 0 )
    dummy7 = ifelse( model_data$snp1 == 2 & model_data$snp2 == 0, 1, 0 )
    dummy8 = ifelse( model_data$snp1 == 2 & model_data$snp2 == 1, 1, 0 )
    dummy9 = ifelse( model_data$snp1 == 2 & model_data$snp2 == 2, 1, 0 )
    
    snp_and_phenotype = data.frame( dummy1, dummy2, dummy3, dummy4, dummy5, dummy6, dummy7, dummy8, dummy9, phenotype = model_data$phenotype )
    
    stat_alt = glm( phenotype ~ dummy1 + dummy2 + dummy3 + dummy4 + dummy5 + dummy6 + dummy7 + dummy8 + dummy9,
                data = snp_and_phenotype, family = binomial( "logit" ) )
    stat_null = glm( phenotype ~ 1, data = snp_and_phenotype, family = binomial( "logit") )
    
    return( lrtest( stat_alt, stat_null )[ "Pr(>Chisq)" ][ 2, 1 ] )
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

