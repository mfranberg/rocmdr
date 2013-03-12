library( ggplot2 )

argv = commandArgs( trailingOnly = TRUE )
if ( length( argv ) != 4 )
{
    message( "error: Wrong number of arguments." )
    message( "Usage: plot_model_power cov_model title ylabel output_file" )
    quit( )
}

model_power = read.table( argv[ 1 ], header = F )
names( model_power ) = c( "method", "effect", "power", "cov" )
model_power$cov[ model_power$cov == 0 ] = "Without covariates"
model_power$cov[ model_power$cov == 1 ] = "With covariates"

title = argv[ 2 ]
ylabel = argv[ 3 ]
output_file = argv[ 4 ]

pdf( output_file )

ggplot( model_power, aes( x = factor( effect ), y = power, group = method, fill = method, alpha = cov ) ) +
    geom_bar( stat = "identity", position = "dodge" ) +
    scale_alpha_manual( values = c( 1.0, 0.5, 1.0 ) ) +
    ggtitle( title ) + 
    scale_x_discrete( "Effect size (increasing)" ) + 
    scale_y_continuous( ylabel )

dev.off( )
