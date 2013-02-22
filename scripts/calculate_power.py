import sys

if len( sys.argv ) != 6:
    print( "Usage: calculate_power.py method model column <ge|gt|lt|le> threshold" )
    print( "Counts the number of items in a givne column that are smaller or larger than a given threshold." )
    print( "For example: calculate_power.py 1 1 0 gt 0.4, counts all values in column 0 > 0.4 as significant." )
    print( "The output is then 1 1 num_significant" )
    exit( 1 )

method = sys.argv[ 1 ]
model = sys.argv[ 2 ]
column = int( sys.argv[ 3 ] )
op_map = { "ge" : float.__ge__, "gt" : float.__gt__, "lt" : float.__lt__, "le" : float.__le__ }
op = op_map.get( sys.argv[ 4 ], None )
threshold = float( sys.argv[ 5 ] )

if not op:
    print( "apply_threshold.py: error: No such operator '{0}'.".format( sys.argv[ 4 ] ) );
    exit( 1 )

num_significant = 0
total = 0
for line in sys.stdin:
    column_list = line.strip( ).split( )

    try:
        value = float( column_list[ column ] )
    except:
        continue

    if op( value, threshold ):
        num_significant += 1

    total += 1

power = num_significant / float( total )
print( "{0}\t{1}\t{2}".format( method, model, power ) )
