#include <stdlib.h>

#include "cmdline.h"

int main(int argc, char **argv)
{
	gengetopt_args_info argsInfo;
	if( cmdline_parser( argc, argv, &argsInfo ) != 0 )
	{
		exit( -1 );
	}

	cmdline_parser_free( &argsInfo );

    return 0;
}
