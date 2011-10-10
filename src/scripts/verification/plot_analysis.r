library(infotheo)

argv = commandArgs( trailingOnly = TRUE )
if ( length( argv ) != 2 )
{
    message( "plot_pvalues: Wrong number of arguments." )
    message( "Usage: association_analysis rocmdrStatsFile outputFile" )
    quit( )
}

rocmdrStatsFile = argv[ 1 ]
outputFile = argv[ 2 ]


# Read data and coalesce columns
rocmdrStats = read.table( rocmdrStatsFile, header = TRUE )
coalescedResults = cbind( rocmdrStats[ ,2 ], rocmdrStats[ ,3 ] ) 

# Make a plot and store it in the given output file
png( outputFile, width=800, height=600 )

cellLabels = paste( "3 * 2", "^", rocmdrStats[ ,1 ], sep = '' )
barplot( t(coalescedResults),
         beside = TRUE,
         col = c("red", "darkblue"),
         names.arg = cellLabels,
         xlab = "Number of cells",
         ylab = "P-value / Fraction significant",
         legend.text = c("P-value of significant SNP", "Fraction of significant SNPs" ),
         main = "Number of cells and significance" )

dev.off( )

