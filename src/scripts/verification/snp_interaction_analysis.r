library(infotheo)

# Load relative script file
argv = commandArgs( trailingOnly = FALSE )
base_dir = dirname( substring( argv[ grep( "--file=", argv ) ], 8 ) )
source( paste( base_dir, 'generate.r', sep = "/" ) )
source( paste( base_dir, 'rocmdr_call.r', sep = "/" ) )

argv = commandArgs( trailingOnly = TRUE )
if ( length( argv ) != 2 )
{
    message( "snp_interaction_analysis: Wrong number of arguments." )
    message( "Usage: snp_interaction_analysis rocmdr_location output_file" )
    quit( )
}

rocMdrLocation = argv[ 1 ]
outputFile = argv[ 2 ]

generate = function( interactionRisk, outputFiles )
{
    N = 2000
    numIntervals = 2
    numFactors = 2
    
    snp1 = rbinom( N, 2, 0.4 )
    snp2 = rbinom( N, 2, 0.4 )
    xor_int = ( snp1 == 0 & snp2 == 0 ) | ( snp1 == 2 & snp2 == 2 )
    
    risk = ifelse( xor_int, interactionRisk, 0.3 )
    flip = runif( N )
    phenotype = ifelse( flip <= risk, 1, 0 )
    
    factors = generateFactorsUniform( numIntervals, N, numFactors )

    # Store temporary file
    saveAsCSV( outputFiles$factor, factors )
    saveAsCSV( outputFiles$snp, cbind( snp1, snp2 ) )
    saveAsCSV( outputFiles$phenotype, phenotype )   
}

reduce = function( effectSize, resultList, powerList )
{
    totalNumSignificant = 0
    for( result in resultList )
    {
        totalNumSignificant = totalNumSignificant + ( sum( result$P.value <= 0.05 ) )
    }
    powerList[[ length( powerList ) + 1 ]] = totalNumSignificant / ( length( resultList ) * 2 )
    
    return( powerList )
}
    
numIterations = 200;
interactionRisk = rev( seq( 0.2, 1.0, 0.1 ) )

powerList = list( )
rocMdrBatch = RocMdrBatch( rocMdrLocation, numIterations )
powerList = runBatch( rocMdrBatch, interactionRisk, generate, reduce, powerList )

reduceData = data.frame( interactionRisk = interactionRisk, power = unlist( powerList ) )

saveAsCSV( outputFile, reduceData )

