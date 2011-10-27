library(infotheo)

argv = commandArgs( trailingOnly = TRUE )
if ( length( argv ) != 3 )
{
    message( "plot_power: Wrong number of arguments." )
    message( "Usage: plot_power rocmdrStatsFile xlabel outputFile" )
    quit( )
}

rocmdrStatsFile = argv[ 1 ]
xlabel = argv[ 2 ]
outputFile = argv[ 3 ]

# Read data and coalesce columns
rocmdrStats = read.table( rocmdrStatsFile, header = TRUE )

parameter = rocmdrStats[ ,1 ]
mdrPower = rocmdrStats[ ,2 ]

# Make a plot and store it in the given output file
png( outputFile, width=800, height=600 )

barplot( mdrPower,
         beside = TRUE,
         names.arg = parameter,
         xlab = xlabel,
         ylab = "Power",
         main = "Power analysis (p = 0.05)" )

dev.off( )

