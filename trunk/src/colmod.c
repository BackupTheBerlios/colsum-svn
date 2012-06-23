/*******************************************************************************
 *            COLSUM - Column Computation Tool
 ***************************************************************************//**
 *
 * @file colmod.c
 *
 * Utility functions for colsum.c
 *
 * @version   0.6.2
 * @version   SVN: \$Id: colmod.c 46 2009-02-01 20:12:09Z stefan $
 * @author    Stefan Habermehl <stefan.habermehl@mcff.de>
 * @copyright (c) 1994,1995,1996,2000,2004,2008,2009 Stefan Habermehl
 * @license   http://www.gnu.org/licenses GNU General Public License v3 or later
 * @uses      @ref cflib <http://cflib.berlios.de>
 *
 ***************************************************************************//**
 *
 * @internal This file is part of Colsum.
 *
 * Colsum is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Colsum is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Colsum (see LICENSE.txt).
 * If not, see <http://www.gnu.org/licenses/>.
 *
 ***************************************************************************//**
 *
 * @internal Version history
 *
 * 2008-08-03 [sh] code cleanup, header with meta info and license
 * 2008-08-04 [sh] some (minor) improvements
 * 2009-01-31 [sh] eclipse project, doxygen documentation
 *
 ******************************************************************************/

#include "colsum.h"

/**
 * @ingroup usage
 *
 * @fn void usage( int code, char *str, int modemask )
 *
 * Output a command line usage message and @b @b @b end program execution
 *
 * @param	code		@b Colsum or @b CFLIB error code
 * @param	*str		Error message
 * @param	modemask	Bitmask with output features selection
 */
void usage( int code, char *str, int modemask )
{
	/** - Init CFLIB Error Code  */
    int ecode;
    /** - Init I/O String Buffer of maximum length @b CF_MAXERRSTR */
    char line[CF_MAXERRSTR+1];

    /** - Set Output Mode Mask to @ref USG_DEFAULT if required */
	if( modemask < 1 ) modemask = USG_DEFAULT;

	/**
	 * - Output all errors in list
	 */
	if( modemask & USG_ERRORS ){

		while( (ecode = cfgeterr( line, 0 )) > 0 )
			fprintf( stderr, "Configuration error %d: %s\n", ecode, line );
	}

	/**
	 * - Program Version and License
	 */
	if( modemask & USG_VERSION )
		fprintf( stderr, "\n%s\n", VERSION );
	if( modemask & USG_LICENSE ){
		fputs( "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/>\n", stderr );
		fputs( "This is free software: you are free to change and redistribute it.\n", stderr );
		fputs( "There is NO WARRANTY, to the extent permitted by law.\n\n", stderr );
	}

    /**
     * - Always show Usage Synopsis and ...
     */
    fputs( cfgetusg(), stderr );

	switch( code ){

		case ERR_USG: /** @arg @ref ERR_USG nothing more */
			break;

		case ERR_HLP: /** @arg @ref ERR_HLP @copybrief ERR_HLP */

			fprintf( stderr, "\n          option v: verbosity level(%2d-%2d), Default=%s\n",
					 VMIN, VMAX, DEF_VERB );
			fprintf( stderr, "          option f: output-format (C-style)\n" );
			fprintf( stderr, "          option h: help (set verbosity to 0 for configuration dump!)\n" );
			fprintf( stderr, "          option i: infile to read from instead of <stdin>\n" );
			fprintf( stderr, "          option o: outfile to write to instead of <stdout>\n" );
			fprintf( stderr, "          option r: configuration file\n" );
			fprintf( stderr, "          option s: save configuration to given filename or configuration file where configuration\n" );
			fprintf( stderr, "          \twas read from or default file (when without argument)\n" );
			fprintf( stderr, "          option k: collect comments for saved configuration file (see -s)\n" );
			fprintf( stderr, "          option t: output current time\n" );
			fprintf( stderr, "          option d: output current date\n" );
			fprintf( stderr, "          mathematical expression in the form \"(expr)\" or \"=expr\" must follow the\n" );
			fprintf( stderr, "          \tcolumn-no. to apply to, if given\n" );
			fprintf( stderr, "          + (default) or * after col-no. is operator between column's\n" );
			fprintf( stderr, "          \tresult and stack\n" );
			fprintf( stderr, "          c: computation and output only for lines where the specified\n" );
			fprintf( stderr, "          \tcolumn's value is #1 or between #1 and #2\n" );
			fprintf( stderr, "          Default column=%d, column 0 holds the row no. 1-.. (only significant lines)\n",
			         cfgetnum("COLSUM_DEFCOL") );
			fprintf( stderr, "          blank lines and comments(#) will be ignored (unless -s and -k are set)!\n\n" );

			/**
			 * - Trigger @ref USG_DUMP with option @b "-v0" or @b "-v+"
			 */
			if( cfgetnum("verbosity") == 0 ) modemask |= USG_DUMP;

			break;

		case ERR_ACC: /** @arg @ref ERR_ACC @copybrief ERR_ACC */
			fprintf( stderr, "Access error: %s\n\n", str );
			break;

		case ERR_VERB: /** @arg @ref ERR_VERB @copybrief ERR_VERB */
			fprintf( stderr, "Invalid verbosity: %s\n\n", cfgetstr("verbosity"));
			break;

		case ERR_MATH: /** @arg @ref ERR_MATH @copybrief ERR_MATH */
			fprintf( stderr, "Error in mathematical expression: %s\n\n", str );
			break;

		case ERR_LIM: /** @arg @ref ERR_LIM @copybrief ERR_LIM */
			fprintf( stderr, "Invalid limits: %s\n\n", str );
			break;

		case ERR_ARG: /** @arg @ref ERR_ARG @copybrief ERR_ARG */
			fprintf( stderr, "Invalid argument: %s\n\n", str );
			break;

		default: /** - Configuration error <0 from @ref cflib */
			fprintf( stderr, "Configuration error: %d\n\n", code );
			break;
	}
	/**
	 * - @copybrief USG_DUMP if @ref USG_DUMP is set
	 */
	if( modemask & USG_DUMP ){

		fputs( "Current Configuration:\n\n", stderr );
		cfdump( stderr );
		fputs( "\n", stderr );
	}

	exit( code );
}


/**
 * @ingroup parse_mathexp
 *
 * @fn void parse_mathexp( int sind, char *expr, COLUMN *C, int *max_column )
 *
 * @param sind          Index
 * @param expr          Expression
 * @param C             Column object
 * @param max_column	Max Column
 */
void parse_mathexp( int sind, char *expr, COLUMN *C, int *max_column )
{
	char *tmp1, *tmp2;

	if( *expr == '=' ){ /* new shell friendly syntax: "=expression" */

		tmp1 = expr;
		while( *tmp1 != '\0' ) tmp1++;

	} else { /* classic syntax: "(expression)" */

		if( (tmp1 = strchr( expr, ')' ))
			== NULL ) usage( ERR_MATH, expr, USG_DEFAULT );

		*tmp1 = '\0';    /* strip trailing ')' */
	}
	tmp2 = --tmp1;
	tmp1 = &expr[1]; /* pointer to beginning of expression */

	/**
	 * - Refuse wrongly placed or empty expressions
	 */
	if( *tmp1 == '\0' ) usage( ERR_MATH, expr, USG_DEFAULT );

	/**
	 * - Detect kind of mathematical operation
	 */

	if( *tmp2 == '^' ){

		C->operation = OP_POWB; /* expression value = base */
		*tmp2 = '\0';

	} else {

		switch( *tmp1++ ){

			case '+': C->operation = OP_ADD; break;
			case '-': C->operation = OP_SUB; break;
			case 'x':
			case '*': C->operation = OP_MUL; break;
			case '/': C->operation = OP_DIV; break;
			case '^': C->operation = OP_POWE; break; /* expr value = exponent */
			case 'a': C->operation = OP_ABS; break;
			case 'n':
			case '&': C->operation = OP_AND; break;
			case 'o':
			case '|': C->operation = OP_OR; break;

			default: usage( ERR_MATH, expr, USG_DEFAULT ); /* syntax error */
		}
	}

	/**
	 * - Get factor's sign
	 */
	if( C->operation == OP_DIV || C->operation == OP_MUL ){

		C->faktor = 1.;
		switch( *tmp1++ ){
			case '-': C->faktor = -1.;
			case '+': break;
			default: tmp1--;
		}
	}
	/**
	 * - Get value for operation
	 */
	switch( *tmp1 ){

		case 's': case 'S': /* value of other column specified by its number */

			C->valsp = atoi( ++tmp1 );
			if( C->valsp > *max_column ) *max_column = C->valsp;
			break;

		case 'e': case 'E':	/* Euler's number */

			C->value = exp( 1 ); break;

		default: /* numerical value */

			C->value = (double)atof( tmp1 );

			switch( C->operation ){

				case OP_DIV: /* division -> multiply by reciprocal value */

					if( C->value != 0. ) C->value = 1. / C->value;
					else usage( ERR_MATH, expr, USG_DEFAULT );
					break;

				case OP_SUB: /* substraction -> revert sign */

					C->value *= -1.;
			}
	}
}

/******************************************************************************/
