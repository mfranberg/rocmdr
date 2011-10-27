library(infotheo)

# Load relative script file
argv = commandArgs( trailingOnly = FALSE )
base_dir = dirname( substring( argv[ grep( "--file=", argv ) ], 8 ) )
source( paste( base_dir, 'generate.r', sep = "/" ) )
source( paste( base_dir, 'rocmdr_call.r', sep = "/" ) )

argv = commandArgs( trailingOnly = TRUE )
if ( length( argv ) != 2 )
{
    message( "two_interaction_analysis: Wrong number of arguments." )
    message( "Usage: two_interaction_analysis rocmdr_location output_file" )
    quit( )
}

rocMdrLocation = argv[ 1 ]
outputFile = argv[ 2 ]

generate = function( effectSize, outputFiles )
{
    N = 2000
    
    snp = rbinom( N, 2, 0.4 )
    env  = rnorm( N, 0, 1 )
    env.discretized = discretize( env, nbins = 2 ) - 1
    
    int.env1 = rnorm( N, 0, 1 )
    int.env1.disc = discretize( int.env1, nbins = 2 ) - 1
    
    int.env2 = rnorm( N, 0, 1 )
    int.env2.disc = discretize( int.env2, nbins = 2 ) - 1

    # Coefficients
    beta_int = effectSize
    beta_snp = 0.35

    risk = 1 / ( 1 + exp( -( beta_snp * snp * env + beta_int * int.env1 * int.env2 ) ) )
    flip = runif( length( snp ) )
    phenotype = ifelse( risk <= flip, 1, 0 )

    # Store temporary file
    saveAsCSV( outputFiles$factor, cbind( env.discretized, int.env1.disc, int.env2.disc ) )
    saveAsCSV( outputFiles$snp, snp )
    saveAsCSV( outputFiles$phenotype, phenotype )   
}

reduce = function( effectSize, resultList, powerList )
{
    totalNumSignificant = 0
    for( result in resultList )
    {
        totalNumSignificant = totalNumSignificant + ( sum( result$P.value <= 0.05 ) )
    }
    powerList[[ length( powerList ) + 1 ]] = totalNumSignificant / length( resultList )
    
    return( powerList )
}
    
numIterations = 200;
effectSizes = rev( seq( 0.1, 0.5, 0.1 ) )

powerList = list( )
rocMdrBatch = RocMdrBatch( rocMdrLocation, numIterations )
powerList = runBatch( rocMdrBatch, effectSizes, generate, reduce, powerList )

reduceData = data.frame( effectSize = effectSizes, power = unlist( powerList ) )

saveAsCSV( outputFile, reduceData )

