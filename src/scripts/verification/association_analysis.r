library(infotheo)

# Load relative script file
argv = commandArgs( trailingOnly = FALSE )
base_dir = dirname( substring( argv[ grep( "--file=", argv ) ], 8 ) )
source( paste( base_dir, 'generate.r', sep = "/" ) )

argv = commandArgs( trailingOnly = TRUE )
if ( length( argv ) != 2 )
{
    message( "association_analysis: Wrong number of arguments." )
    message( "Usage: association_analysis rocmdr output_file" )
    quit( )
}

rocMdrLocation = argv[ 1 ]
outputFile = argv[ 2 ]

tmpDir = "/tmp/"
factorFile = paste( tmpDir, "factor_test.txt", sep = '/' )
snpFile = paste( tmpDir, "snps_test.txt", sep = '/' )
phenotypeFile = paste( tmpDir, "phenotype_test.txt", sep = '/' )
resultFile = paste( tmpDir, "res.txt", sep = '/' )

# Map between genotype and risk paramterization
risk_map = rbind(
    c( 0.0, 2, 8 ),
    c( 0.0, 2, 8 ),
    c( 0.0, 3, 2.5 )
)

# Number of samples
N = 2000

# Number of iterations for each column size
numIterations = 50

# Number of intervals in each risk factors
numIntervals = 2

# Number of SNPs
numSnps = 200

# Range of no-association columns
columnStart = 1 
columnEnd = 15

snpsPerColumn = columnStart:columnEnd

allResults = data.frame( columnStart:columnEnd + 1 )
allResults$averagePValue = columnStart:columnEnd
allResults$averageNumSignificant = columnStart:columnEnd

for( numColumns in columnStart:columnEnd )
{
    totalPValue = 0.0
    totalNumSignificant = 0.0
    for( testIteration in 1:numIterations )
    {
        print( paste( "Column: ", numColumns, "Iter: ", testIteration ) )
        
        # Generate true assocation
        real.env = rlnorm( N, 1, 0.3 )
        real.env.disc = discretize( real.env, nbins = numIntervals )$X - 1
        real.snp = rbinom( N, 2, sqrt( 0.4 ) )
        real.phenotype = riskToCaseControl( generateRisk( real.snp, real.env, risk_map ) )
        
        # Generate no association
        factors = generateFactors( numIntervals, N, numColumns )
        snps = generateSNPs( N, numSnps )

        # Store temporary file
        saveAsCSV( factorFile, cbind( real.env.disc, factors ) )
        saveAsCSV( snpFile, cbind( real.snp, snps ) )
        saveAsCSV( phenotypeFile, real.phenotype )

        filenames = paste( snpFile, factorFile,  phenotypeFile )
        call = paste( rocMdrLocation, filenames )        

        system( paste( call, ">", resultFile ) )

        results = read.table( resultFile, header = TRUE )
        
        totalPValue = totalPValue + ( results$P.value[ 1 ] )
        totalNumSignificant = totalNumSignificant + sum( results$P.value <= 0.05 ) / ( numSnps + 1 )
    }

    index = numColumns - columnStart + 1
    allResults$averagePValue[ index ] = totalPValue / numIterations
    allResults$averageNumSignificant[ index ] = totalNumSignificant / numIterations
}

saveAsCSV( outputFile, allResults )
