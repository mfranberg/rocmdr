library( ggplot2 )

argv = commandArgs( trailingOnly = TRUE )
if ( length( argv ) != 5 )
{
    message( "error: Wrong number of arguments." )
    message( "Usage: plot_model_power xlabel ylabel title model_input output_file" )
    quit( )
}

xlabel = argv[ 1 ]
ylabel = argv[ 2 ]
title = argv[ 3 ]

model_power = read.table( argv[ 4 ], header = F )
names( model_power ) = c( "method", "effect", "power", "lower", "upper" )

output_file = argv[ 5 ]

pdf( output_file )

dodge = position_dodge( width = 0.3 )
ggplot( model_power, aes( x = effect, y = power, group = method, ymin = lower, ymax = upper ) ) +
    geom_point( aes( shape = method ), position = dodge, size = 2.5 ) +
    scale_x_continuous( xlabel ) +
    scale_y_continuous( ylabel, limits = c( 0.0, 1.0 ) ) +
    ggtitle( title ) +
    geom_errorbar( width = 0.1, position = dodge ) +
    theme_bw( ) +
    scale_shape( solid = FALSE )

dev.off( )
