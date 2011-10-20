library(infotheo)

# Load relative script file
argv = commandArgs( trailingOnly = FALSE )
base_dir = dirname( substring( argv[ grep( "--file=", argv ) ], 8 ) )
source( paste( base_dir, 'generate.r', sep = "/" ) )
source( paste( base_dir, 'rocmdr_call.r', sep = "/" ) )

argv = commandArgs( trailingOnly = TRUE )
if ( length( argv ) != 2 )
{
    message( "effect_size_analysis: Wrong number of arguments." )
    message( "Usage: effect_size_analysis rocmdr_location output_file" )
    quit( )
}

rocMdrLocation = argv[ 1 ]
outputFile = argv[ 2 ]

generate = function( effectSize, outputFiles )
{
    snp = rbinom( 2000, 2, 0.4 )
    env  = rnorm( 2000, 0, 1 )
    env.discretized = discretize( env, nbins = 2 ) - 1

    # Effect appears one sd away from the mean
    # jointMean = mean( snp * env )
    # jointSd   = sd( snp * env )
    # p = baseRisk + effectSize
    # beta = log( p / (1 - p) ) / ( jointMean + jointSd )
    beta = effectSize

    risk = 1 / ( 1 + exp( -( beta * snp * env ) ) )
    flip = runif( length( snp ) )
    phenotype = ifelse( risk <= flip, 1, 0 )

    # Store temporary file
    saveAsCSV( outputFiles$factor, env.discretized )
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
effectSizes = rev( seq( 0.1, 0.5, 0.05 ) )

powerList = list( )
rocMdrBatch = RocMdrBatch( rocMdrLocation, numIterations )
powerList = runBatch( rocMdrBatch, effectSizes, generate, reduce, powerList )

reduceData = data.frame( effectSize = effectSizes, power = unlist( powerList ) )

saveAsCSV( outputFile, reduceData )

