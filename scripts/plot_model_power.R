library( ggplot2 )

argv = commandArgs( trailingOnly = TRUE )
if ( length( argv ) != 5 )
{
    message( "error: Wrong number of arguments." )
    message( "Usage: plot_model_power title level rocmdr_input glm_input output_file" )
    quit( )
}

compute_power = function( x )
{
    return( sum( x <= 0.005 ) / length( x ) )
}

level = as.numeric( argv[ 2 ] )
rocmdr_power = read.table( argv[ 3 ], header = F, sep = "\t" )
rocmdr_power.agg = aggregate( rocmdr_power$V3, by = list( model = rocmdr_power$V4 ), FUN = compute_power )

glm_power = read.table( argv[ 4 ], header = F, sep = "\t" )
glm_power.agg = aggregate( glm_power$V2, by = list( model = glm_power$V3 ), FUN = compute_power )
output_file = argv[ 5 ]

power = c( rocmdr_power.agg$x, glm_power.agg$x )
model = c( rocmdr_power.agg$model, glm_power.agg$model )
desc = c( rep( "Rocmdr", length( rocmdr_power.agg$x ) ), rep( "Logistic", length( glm_power.agg$x ) ) )

power_data = data.frame( power = power, model = model, method = desc )

pdf( output_file )
ggplot( power_data, aes( x = model ) ) +
    geom_line( aes( y = power, col = method ) ) +
    scale_x_discrete( "Model (increasing effect size)", limits = c( 0, 1.0 ) ) +
    scale_y_continuous( "Power") +
    opts( title = paste( "Power under", argv[ 1 ], "model" ) )
dev.off( )
