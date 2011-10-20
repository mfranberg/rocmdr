library(infotheo)

# Load relative script file
argv = commandArgs( trailingOnly = FALSE )
base_dir = dirname( substring( argv[ grep( "--file=", argv ) ], 8 ) )
source( paste( base_dir, 'generate.r', sep = "/" ) )
source( paste( base_dir, 'rocmdr_call.r', sep = "/" ) )

argv = commandArgs( trailingOnly = TRUE )
if ( length( argv ) != 2 )
{
    message( "association_analysis: Wrong number of arguments." )
    message( "Usage: association_analysis rocmdr output_file" )
    quit( )
}

rocMdrLocation = argv[ 1 ]
outputFile = argv[ 2 ]

# Number of SNPs
numSnps = 100

##
# Generates samples from a XOR model
#
generate.xor = function( N, noise )
{
    real = list( )
    
    association = generateRandomXor( N, noise )
    real$snp = association[[ 1 ]]
    real$env.disc = association[[ 2 ]]
    real$phenotype = association[[ 3 ]]
    
    return( real )
}

##
# Generates samples from a logistic regression model that
# only contains an interaction term.
#
generate.logistic = function( N, numIntervals, effectSize )
{
    real = list( )
    
    real$snp = rbinom( N, 2, 0.4 )
    real$env  = rnorm( N, 0, 1 )
    real$env.disc = discretize( real$env, nbins = numIntervals ) - 1

    beta = effectSize
    risk = 1 / ( 1 + exp( -( beta * real$snp * real$env ) ) )
    flip = runif( N )
    real$phenotype = ifelse( risk <= flip, 1, 0 )
    
    return( real )
}

##
# Generates a sample from a model similar to logistic regression
# but where each SNP can have their own coefficients.
#
generate.realistic = function( N, numIntervals )
{
    # Map between genotype and risk paramterization
    risk_map = rbind(
        c( 0.0, 2, 8 ),
        c( 0.0, 2, 8 ),
        c( 0.0, 3, 2.5 )
    )
    
    real = list( )
    
    real$env = rlnorm( N, 1, 0.3 )
    real$env.disc = discretize( real$env, nbins = numIntervals )$X - 1
    real$snp = rbinom( N, 2, sqrt( 0.4 ) )
    real$phenotype = riskToCaseControl( generateRisk( real$snp, real$env, risk_map ) )
    
    return( real )
}

generate = function( numColumns, outputFiles )
{
    # Number of samples
    N = 2000
        
    # Number of intervals in each risk factors
    numIntervals = 2
    
    real = generate.realistic( N, numIntervals )
    
    factors = generateFactorsUniform( numIntervals, N, numColumns )
    snps = generateSNPUniform( N, numSnps )

    # Store temporary file
    saveAsCSV( outputFiles$factor, cbind( real$env.disc, factors ) )
    saveAsCSV( outputFiles$snp, cbind( real$snp, snps ) )
    saveAsCSV( outputFiles$phenotype, real$phenotype )
}

reduce = function( columnSize, resultList, pValueAndPowerList )
{
    totalPValue = 0.0
    totalNumSignificantNull = 0.0
    for( result in resultList )
    {
        totalPValue = totalPValue + ( result$P.value[ 1 ] )
        
        numSignificantNull = sum( result$P.value[ 2:(length( result$P.value )) ] <= 0.05 )
        totalNumSignificantNull = totalNumSignificantNull + numSignificantNull
    }
    
    averagePValue = totalPValue / length( resultList )
    averageFractionSignificantNull = totalNumSignificantNull / ( numSnps * length( resultList ) )
    
    nextElement = length( pValueAndPowerList ) + 1
    pValueAndPowerList[[ nextElement ]] = c( averagePValue, averageFractionSignificantNull )
    
    return( pValueAndPowerList )
}

# Number of iterations for each column size
numIterations = 30

# Range of no-association columns
columnStart = 1 
columnEnd = 15
columnSizes = columnStart:columnEnd

pValueAndPowerList = list( )
rocMdrBatch = RocMdrBatch( rocMdrLocation, numIterations )
pValueAndPowerList = runBatch( rocMdrBatch, columnSizes, generate, reduce, pValueAndPowerList )

allResults = data.frame( columnStart:columnEnd + 1 )
allResults$averagePValue = unlist( lapply( pValueAndPowerList, "[", c( 1 ) ) )
allResults$averageNumSignificant = unlist( lapply( pValueAndPowerList, "[", c( 2 ) ) )

saveAsCSV( outputFile, allResults )
