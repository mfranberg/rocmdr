library(infotheo)

argv = commandArgs( trailingOnly = TRUE )
if ( length( argv ) != 2 )
{
    message( "plot_effect: Wrong number of arguments." )
    message( "Usage: plot_effect rocmdrStatsFile outputFile" )
    quit( )
}

rocmdrStatsFile = argv[ 1 ]
outputFile = argv[ 2 ]

# Read data and coalesce columns
rocmdrStats = read.table( rocmdrStatsFile, header = TRUE )

sampleSizes = rocmdrStats[ ,1 ]
mdrPower = rocmdrStats[ ,2 ]

# Make a plot and store it in the given output file
png( outputFile, width=800, height=600 )

barplot( mdrPower,
         beside = TRUE,
         names.arg = sampleSizes,
         xlab = "Effect size",
         ylab = "Power",
         main = "Power analysis (p = 0.05)" )

dev.off( )

