##
# Generates a single column of discrete environmental factors.
#
# @param numSamples The length of the column.
# @param numIntervals The number of levels.
# 
# @return The generated column.
#
generateFactorColumn = function(numSamples, lnormMean, lnormVar, numIntervals = 2)
{
    continousFactors = rlnorm( numSamples, lnormMean, lnormVar )
    
    if( numIntervals > 0 )
    {
        return( discretize( continousFactors, nbins = numIntervals )$X - 1 )
    }
    else
    {
        return( continousFactors )
    }
}

##
# Generates a data frame of SNPs according to the binomial distribution.
#
# @param numSample The length of the column.
# @param numSnps The number of SNPs to generate.
# @param freq The expected frequency of the major homozygote.
#
# @return A data frame of SNPs. The name of each column is "snpX" where
#         X is a number from 1 to numSnps.
#
generateSNPs = function(numSamples, numSnps, freq = 0.4 )
{
    snpMatrix = matrix( nrow = numSamples, ncol = numSnps )
    for( i in 1:numSnps )
    {
        snpMatrix[ ,i ] = rbinom( numSamples, 2, sqrt( freq ) )
    }

    snpFrame = as.data.frame( snpMatrix )
    names( snpMatrix ) = paste( "snp", 1:numSnps, sep = '' )

    return( snpFrame )
}

##
# Generates a data frame of SNPs uniformly.
#
# @param numSample The length of the column.
# @param numSnps The number of SNPs to generate.
#
# @return A data frame of SNPs. The name of each column is "snpX" where
#         X is a number from 1 to numSnps.
#
generateSNPUniform = function(numSamples, numSnps)
{
    snpMatrix = matrix( nrow = numSamples, ncol = numSnps )
    for( i in 1:numSnps )
    {
        snpMatrix[ ,i ] = sample( 0:2, numSamples, replace = TRUE )
    }

    snpFrame = as.data.frame( snpMatrix )
    names( snpMatrix ) = paste( "snp", 1:numSnps, sep = '' )

    return( snpFrame )
}

##
# Generates a table of environmental factors.
#
# @param numIntervals The number of levels for each factor.
# @param numSample The number of samples of each factor.
# @param numColumns The number of factors.
#
# @return A data frame of environmental factors. The name of each column
#         is "envX" where X is a number from 1 to numColumns.
#
generateFactors = function(numIntervals, numSamples, numColumns)
{
    factorMatrix = matrix( nrow = numSamples, ncol = numColumns )
    for( i in 1:numColumns )
    {
        lnormMean = runif( 1, 1, 5 )
        lnormVar  = runif( 1, 0, 2 )
        factorMatrix[ ,i ] = generateFactorColumn( numSamples, lnormMean, lnormVar, numIntervals )
    }

    factorFrame = as.data.frame( factorMatrix )
    names( factorMatrix ) = paste( "env", 1:numColumns, sep = '' )

    return( factorFrame )
}

##
# Generates a set of uniformly distributed factors.
#
# @param numIntervals The number of levels for each factor.
# @param numSample The number of samples of each factor.
# @param numColumns The number of factors.
#
# @return A data frame of environmental factors. The name of each column
#         is "envX" where X is a number from 1 to numColumns.
#
generateFactorsUniform = function(numIntervals, numSamples, numColumns)
{
    factorMatrix = matrix( nrow = numSamples, ncol = numColumns )
    for( i in 1:numColumns )
    {
        factorMatrix[ ,i ] = sample( 0:(numIntervals-1), numSamples, replace = TRUE )   
    }

    factorFrame = as.data.frame( factorMatrix )
    names( factorMatrix ) = paste( "env", 1:numColumns, sep = '' )

    return( factorFrame )
}

##
# Generates a column of uniformly distributed phenotypes. 
#
# @param numSample The number of samples.
#
# @return The generated column.
# 
generatePhenotypes = function(numSamples)
{
    phenotypes = runif( numSamples, 0, 1 )
    return( data.frame( ifelse( phenotypes <= 0.5, 0, 1 ) ) )
}

##
# Given a set of snps, exposure for each snp, and a risk_map
# that determines the effect of the exposure in the context of
# the SNP.
# 
# @param snp Column of snps.
# @param exposure Column of exposures.
# @param risk_map Parameters for the risk function, a 3*3 matrix that
#        defines the br, a, t for the expose (see expose) function.
#
# @param Column of risk for each individual.
#
generateRisk = function(snp, exposure, risk_map)
{
    risk_params = risk_map[ snp + 1, ]
    return( expose( exposure, risk_params[,1], risk_params[,2], risk_params[,3] ) )
}

##
# Determines the risk of the list of exposures in x.
#
# @param x A vector of points.
# @param br The amount of risk at zero exposure.
# @param a How fast the risk increases.
# @param t The amount of exposure required to have a risk of 0.5.
#
# @return the risk of the values in x.
#
expose = function(x, br, a, t)
{
    return( 1 - (1 - br) * exp( -( x / t )^(2*a) ) )
}

##
# Given a vector of risk probabilities, this function returns
# a vector of case/control classifications based on the risk
# probabilities.
#
# @param risk Vector of risk probabilities.
# 
# @return a case/control classification.
#
riskToCaseControl = function(risk)
{
    
    case_flip = runif( length( risk ), 0, 1 )
    case = ifelse( case_flip < risk, 1, 0 )
    return( case )
}

##
# Generates a deterministic association signal with noise.
#
# 
#
generateRandomXor = function(numSamples, noise = 0.2)
{
    snps = rbinom( numSamples, 1, 0.5 )
    env  = rbinom( numSamples, 1, 0.5 )
    phenotype = ( !snps & env ) | ( snps & !env )

    noise = rbinom( numSamples, 1, noise )
    noisy_phenotype = ifelse( ((noise == 1 & phenotype == 0) | phenotype == 1), 1, 0 )

    return( list( snps, env, noisy_phenotype ) ) 
}

##
# Saves the given data frame in a CSV file, separated by tabs.
#
# @param filename Name or path of the file to save.
# @param dataFrame A data frame containing the data to save.
#
saveAsCSV = function(filename, dataFrame)
{
    write.table( dataFrame, file = filename, row.names = FALSE, sep = "\t" ) 
}
