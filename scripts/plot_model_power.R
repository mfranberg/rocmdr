library( ggplot2 )

argv = commandArgs( trailingOnly = TRUE )
if ( length( argv ) != 4 )
{
    message( "error: Wrong number of arguments." )
    message( "Usage: plot_model_power title model_input output_file plot_type" )
    quit( )
}

model_power = read.table( argv[ 2 ], header = F )
names( model_power ) = c( "method", "model", "power" )

output_file = argv[ 3 ]
plot_type = argv[ 4 ]

pdf( output_file )

ggplot( model_power, aes( x = model, y = power, fill = method ) ) +
    geom_bar( position = "dodge", stat = "identity" ) +
    scale_x_discrete( "Model (increasing effect size)", limits = c( 1, max( model_power$model ) )  ) +
    scale_y_continuous( plot_type ) +
    opts( title = paste( plot_type, "under", argv[ 1 ], "model" ) )

dev.off( )
