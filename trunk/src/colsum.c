/*******************************************************************************
 *            COLSUM - Column Computation Tool
 ***************************************************************************//**
 *
 * @mainpage colsum - Column Computation Tool
 *
 * @copydoc colsum
 *
 * @section manual User Manual
 *
 * - @ref colsum Manual Page
 *
 * @section copying License and Copying
 *
 * - @subpage colsum_license
 *
 * @section documentation Program Documentation
 *
 * @subsection main Main Program
 * - @ref colsum.c
 *
 * @subsection utilities Utilities
 * - @ref usage
 * - @ref parse_mathexp
 * - @ref cflib
 *
 * @defgroup colsum colsum
 * @{
 * @details
 *
 * Analyze, compute and transform numerical data tables of unlimited length
 *
 * @section synopsis SYNOPSIS
 * @dontinclude examples/usage_output.txt
 * @skipline Usage
 * @until ...
 *
 * @section description DESCRIPTION
 *
 * Colsum extracts columns with numerical values from an input stream, does
 * some computations on them and outputs the results in a well-defined,
 * formatted form to an output stream.
 *
 * Colsum is designed to be used as a filter. Input- and output stream can
 * both be a file, pipe or standard I/O channel. You can easily do more
 * complex computations by combining colsum calls with different instructions
 * in a pipeline.
 *
 * @subsection basics Basic operations and options:
 *
 * 1. Compute number of values, sum, maximum, minimum, arithmetic mean
 *    and standard deviation based on one result value per non-empty
 *    input file row
 *
 * 2. Return a "clean" selection of input columns and/or values computed from
 *    them
 *
 * 3. Output format for numerical values is decimal float by default, but
 *    can be octal or hexadecimal depending on output option "-v" or specified
 *    as "printf"-Style format string (see 'man 3 printf')
 *
 * 4. Every input column can be transformed by one mathematical operation:
 *    - add/substract a constant value or value of another column
 *    - multiply/divide by a constant value or value of another column
 *    - raise to power, given exponent
 *
 * @section commandline OPTIONS
 * Use command @b "colsum -h":
 * @dontinclude examples/usage_output.txt
 * @skipline option
 * @until blank
 *
 * @section version VERSION
 * @version   0.6.2 (\$Id: colsum.c 46 2009-02-01 20:12:09Z stefan $)
 * @section author AUTHOR
 * @author    Stefan Habermehl <stefan.habermehl@mcff.de>
 * @section copyright COPYRIGHT
 * @copyright (c) 1994,1995,1996,2000,2004,2008,2009 Stefan Habermehl
 * @license   GNU General Public License v3 or later, see @ref license
 * @section uses SEE ALSO
 * @uses      @ref cflib <http://cflib.berlios.de>
 * @}
 *
 ***************************************************************************//**
 *
 * @page colsum_license Colsum License
 *
 * This file is part of @b Colsum - Column Computation Tool.
 *
 * @author Stefan Habermehl <stefan.habermehl@mcff.de>
 *
 * @copyright (c) 1994,1995,1996,2000,2004,2008,2009 Stefan Habermehl
 *
 * @b Colsum is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * @b Colsum is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with @b Colsum (see LICENSE.txt).
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * @verbinclude LICENSE.txt
 *
 ***************************************************************************//**
 *
 * @internal Version history
 *
 * 1996-09-19 [sh] c:900 == c:900:900
 * 2004-05-06 [sh] changed default format to %-15.15g
 * 2008-08-03 [sh] code cleanup, header with meta info and license
 * 2009-01-31 [sh] eclipse project, doxygen documentation
 *
 ******************************************************************************/

#include "colsum.h"

/**
 * @file colsum.c Main Program @ref colsum
 *
 * @fn main( int argc, char *argv[] )
 *
 * @param argc Command Line Argument Count
 * @param argv Command Line Argument Array
 * @return
 *        @arg @c 0 : on normal, error free execution
 *        @arg CFLIB Error Code, see @ref errors
 *        @arg Colsum Error Code, see @ref usage
 *        @arg @b verbosity = @c 0 :
 *             Numerical Result of Computation (e.g. for use in shell scripts!)
 */
int main( int argc, char *argv[] )
{
	int i, v, sind = 0, cfret, def_column, comlines;
	double t, vgl;
	FILE *kanal, *fin = stdin, *fout = stdout;
	char *infile, *outfile, *format;
	char line[MAXLINE + 1], formbuf[MAXFORM + 1];
	char nostackval, nocolumn;
	char *tmp1, *tmp2, *tmp3;
	char *infoformat = "file: %s, column %d: %d values.\nsum = ";

	/**
	 * 	Declare Array of @ref COLUMN objects
	 */
	COLUMN C[SPAMAX + 1];

	/**
	 * Declare Global Result Structure @ref COLRESULT
	 */
	COLRESULT G;

	/**
	 * Set up CFLIB @ref config_initializer
	 */
	CONFIG setting[] = {
	 { "help",           CF_FLAG_OFF,    'h', CF_FLAG | CF_IGN_ENV | CF_NOSAVE, },
	 { "verbosity",      DEF_VERB,       'v', CF_CONCAT | CF_INT, },
	 { "infile",         "<stdin>",      'i', 0, },
	 { "outfile",        "<stdout>",     'o', 0, },
	 { "print_time",     CF_FLAG_OFF,    't', CF_FLAG | CF_IGN_ENV, },
	 { "print_date",     CF_FLAG_OFF,    'd', CF_FLAG | CF_IGN_ENV, },
	 { "save_comments",  CF_FLAG_OFF,    'k', CF_FLAG | CF_IGN_ENV, },
	 { "TIME",           NULL,           ' ', CF_TIME | CF_SET_PUT, },
	 { "DATE",           NULL,           ' ', CF_DATE | CF_SET_PUT, },
	 { "COLSUM_USAGE",   DEF_USAGE,      ' ', CF_USAGE | CF_NOSAVE, },
	 { "COLSUM_FORMAT",  DEF_FORMAT,     'f', CF_CONCAT, },
	 { "COLSUM_DEFCOL",  DEF_COLUMN,     ' ', 0, },
	 { "COLSUM_INI",     "colsum.ini",   'r', CF_SETFILE, },
	 { "COLSUM_SAVEINI", NULL,           's', CF_IGN_ENV | CF_CONCAT | CF_NOSAVE, },
	 { "COLSUM_SYSINI",  "~/colsum.ini", ' ', CF_SYS_SETFILE | CF_SET_ENV | CF_LAST, },
	};

	/**
	 * Call cfinit() to initialize configuration database
	 *
	 * Call usage() on error or help request
	 */
	if( (cfret = cfinit( setting, argc, argv ))
		< 0 ) usage( cfret, NULL, USG_ERRORS|USG_DUMP );
	if( cfgetflag("help") )
		usage( ERR_HLP, NULL, USG_VERSION|USG_LICENSE );

	/**
	 * Initialize @ref COLUMN and @ref COLRESULT structures
	 */
	memset( &G, 0, sizeof(struct _COLRESULT) );
	G.limcol = -1;

	for( i = 0; i <= SPAMAX; i++ ){
		C[i].column    = -1;
		C[i].spoi      = NULL;
		C[i].operation = 0;
		C[i].faktor    = 0.;
		C[i].value     = 0.;
		C[i].item      = 0.;
		C[i].valsp     = -1;
		C[i].sop       = NULL;
	}

	/**
	 * Check output @b verbosity level
	 */
	if( (v = cfgetnum("verbosity")) < VMIN || v > VMAX ){

		usage( ERR_VERB, NULL, USG_DEFAULT );

	} else if( v == 10 ){

		infoformat = "file = %s\ncolumn = %d\nvalues = %d\nsum = ";
		v = 2;
	}

	/**
	 * Get configuration parameters @b infile, @b outfile, @b COLSUM_FORMAT,
	 * @b COLSUM_DEFCOL, @b save_comments and @b COLSUM_SAVEINI
	 * from cofiguration
	 */
	infile     = cfgetstr("infile");
	outfile    = cfgetstr("outfile");
	format     = cfgetstr("COLSUM_FORMAT");
	def_column = cfgetnum("COLSUM_DEFCOL");
	comlines   = ( cfgetflag("save_comments")
			       && cfgetstr("COLSUM_SAVEINI") != NULL ) ? 0 : -1;

	/**
	 * Get residual command line arguments (Colsum instructions)
	 */
	while( (tmp3 = cfgetres()) != NULL ){

		switch( *tmp3 ){

			case '+':
			case '*':
				/**
				 * - operator between actual column's result and stack
				 */

				/* default column is assumed if before any col. specification */
				if( sind == 0 ) C[++sind].column = def_column;

				C[sind].sop = (*tmp3=='+') ? "+" : "*";
				break;

			case '(':
			case '=':
				/**
				 * - mathematical expression to apply to current column to give
				 *   this column's result
				 */
				/* default column is assumed if before any col. specification */
				if( sind == 0 ) C[++sind].column = def_column;

				parse_mathexp( sind, tmp3, &C[sind], &C[0].column );
				break;

			/**
			 * - numerical range condition for the specified column
			 */
			case 'C':
			case 'c':

				if( (G.limcol = atoi( &tmp3[1] )) >= 0 ){

					if( (tmp1 = strchr( tmp3, ':' ))
						== NULL ) usage( ERR_LIM, tmp3, USG_DEFAULT );
					G.limlow = atof( ++tmp1 );

					if( (tmp3 = strchr( tmp1, ':' ))
						== NULL ) G.limupp = G.limlow;

					else G.limupp = atof( ++tmp3 );

					if( G.limlow > G.limupp ){
						t = G.limupp;
						G.limupp = G.limlow;
						G.limlow = t;
					}
					if( G.limcol > C[0].column ) C[0].column = G.limcol;

				} else usage( ERR_LIM, tmp3, USG_DEFAULT );
				break;

			default:

				if( isdigit( *tmp3 ) ){	/* No. of next column, if possible */

					C[++sind].column = atoi( tmp3 );

				} else if( !strcmp( infile, "<stdin>" ) ){ /* infile w/o "-i" */

					infile = tmp3;
					cfputstr( "infile", infile );

				} else usage( ERR_ARG, tmp3, USG_DEFAULT ); /* wrong argument */

				break;
		}
	}

	/**
	 * Open input and output files unless @c stdin or @c stdout are used
	 */
	if( strcmp( infile, "<stdin>" ) ){
		if( (fin = fopen( infile, "r" ))
			== NULL ) usage( ERR_ACC, infile, USG_DEFAULT );
	}
	if( strcmp( outfile, "<stdout>" ) ){
		if( (fout = fopen( outfile, "w" ))
			== NULL ) usage( ERR_ACC, outfile, USG_DEFAULT );
	}

	kanal = fout;

	if( C[1].column == -1 ){

		C[1].column = def_column;
		sind = def_column; /* sind=1; */
	}

	/* Put maximum col.-no. in C[0].column */
	for( i = 1; i <= sind; i++ ){
		if( C[i].column > C[0].column ) C[0].column = C[i].column;
	}

	/**
	 * Only one column: assume stack mode "adding".
	 * No stack operation at all:  "for all"
	 */
	if( sind == 1 && C[1].sop == NULL )

		C[1].sop = "+"; /* can be removed */

	else if( sind > 1 ){

		for( i = 1; i <= sind && C[i].sop == NULL; i++ );
		if( i == sind + 1 )
			for( i = 1; i <= sind; i++ ) C[i].sop = "+";
	}

	/**
	 * Main loop over input stream:
	 *//*
	 * - apply computations for each (non-empty) input line
	 * - do line result output on the fly
	 * - update current result structure
	 */
	while( !feof(fin) ){

		*line = '\0';
		if( fgets( line, MAXLINE, fin ) == NULL || strlen( line ) < 1 ) break;
		RemoveCR( line );

		/**
		 * - Comment line:
		 * @li no computations
		 * @li include in configuration if program parameter @b save_comments
		 *     has been set, will be dumped on cfsave() call
		 */
		if( *line == '#' ){
			if( comlines >= 0 ){
				tmp2 = EatWhiteSpace( line+1 );
				if( *tmp2 != '\0' ){
					sprintf( formbuf, "COMMENT_%03d_%03d",
									  ++comlines, G.count+1 );
					cfputstr( formbuf, tmp2 );
				}
			}
			continue;
		}

		C[1].spoi = EatWhiteSpace( line );

		/** @n
		 * - Get column pointers and put a string-end behind each column
		 */
		for( i = 2; i <= C[0].column; i++ ){

			if( (tmp2 = strpbrk( C[i - 1].spoi, " \t" )) == NULL ){

				C[i].spoi = "";

			} else {

				C[i].spoi = EatWhiteSpace( tmp2 );
				*tmp2 = '\0';
			}
		}

		/**
		 * - Go through list of columns appying operations and add to
		 *   or multiply with stack
		 */
		G.scount++;
		C[0].item  = 0.;
		nocolumn   = TRUE;
		nostackval = TRUE;

		/**
		 * - Ignore line when specified column's value is outside range limits
		 */
		if( G.limcol > 0 && strlen( C[G.limcol].spoi ) > 0 )
			vgl = atof( C[G.limcol].spoi );
		else if( G.limcol == 0 )
			vgl = (double)G.scount;
		else vgl = G.limlow; /* not very clean style :-( */
		if( vgl < G.limlow || vgl > G.limupp ) continue;

		for( i = 1; i <= sind; i++ ){
			/**
			 * - Loop over columns in current line
			 *   @li Check column's existence
			 */
			if( C[i].column == 0 || strlen( C[C[i].column].spoi ) > 0 ){

				nocolumn = FALSE;

				/**
				 * @li Get column's value
				 */
				C[i].item = (C[i].column == 0) ?
							G.scount : atof( C[C[i].column].spoi );

				/**
				 * @li Apply operation if required
				 */
				if( C[i].operation != OP_NONE ){

					if( C[i].valsp > 0 ){

						C[i].value = atof( C[C[i].valsp].spoi );

						switch( C[i].operation ){

							case OP_DIV: /* division -> multiply by reciproc. */

								if( C[i].value != 0. ){
									C[i].value = 1. / C[i].value;
								} else {
									fprintf(stderr,
									  "Division by zero: column %d / column %d skipped. You will find the value %8.3f in the output\n",
									  i, C[i].valsp, CFE_RNF );
									fflush(stderr);
									C[i].item = (C[i].faktor<0) ? -1 : 1;
									C[i].value = CFE_RNF;
								}
								break;

							case OP_SUB: /* substraction -> revert sign */

								C[i].value *= -1.;
						}
					}

					switch( C[i].operation ){

						case OP_ADD: case OP_SUB:
							C[i].item += C[i].value; break;

						case OP_MUL: case OP_DIV:
							C[i].item *= C[i].faktor * C[i].value; break;

						case OP_ABS:
							C[i].item = C[i].value = fabs(C[i].item); break;

						case OP_POWB:
							C[i].item = pow( C[i].value, C[i].item ); break;

						case OP_POWE:
							C[i].item = pow( C[i].item, C[i].value ); break;

						case OP_AND:
							C[i].item = (int)floor( C[i].item + 0.5 )
										& (int)floor( C[i].value + 0.5 );
							break;

						case OP_OR:
							C[i].item = (int)floor( C[i].item + 0.5 )
										| (int)floor( C[i].value + 0.5 );
							break;
					}
				}

				/**
				 * @li Update Stack value
				 */
				if( C[i].sop != NULL ){

					nostackval = FALSE;
					if( C[i].sop[0] == '+' ) C[0].item += C[i].item;
					else C[0].item *= C[i].item;
				}
			}
		}

		if( nocolumn == FALSE ){

			/* G.scount++; */

			if( nostackval == FALSE ){
				/** @n
				 * - Update Result Stack
				 */
				G.count++;
				G.sum += C[0].item;
				G.qsum += C[0].item * C[0].item;
				if( C[0].item > G.max || G.count == 1 ) G.max = C[0].item;
				if( C[0].item < G.min || G.count == 1 ) G.min = C[0].item;
			}

			/**
			 * - Line output
			 */

			if( v >= 3 ){

				if( v == 4 ) fprintf( fout, "%4d: \t", G.scount );

				else if( v >= 6 ){

					for( i = 1; i <= sind; i++ ){

						if( v == 8 )
							fprintf( fout, "0%-15o\t",
										   (int)floor( C[i].item + 0.5 ) );
						else if( v == 9 )
							fprintf( fout, "0x%-15x\t",
										   (int)floor( C[i].item + 0.5 ) );
						else{
							fputs( " \t", fout );
							fprintf( fout, format, C[i].item );
						}
					}
				}

				if( nostackval == FALSE && v < 7 ){

					fprintf( fout, format, C[0].item );

					if( v == 4 ){

						fputc( '\t', fout );
						fprintf( fout, format, G.sum );
					}
				}
				fputs( "\n", fout );
			}
		}
	}

	/**
	 * Result Output
	 */
	switch( v ){

		case 0: exit( G.sum );

		case 1:
			fprintf( kanal, format, G.sum );
			fputc( '\n', kanal );
			break;

		case 3:
			kanal = stderr;

		case 2:	case 4:	case 10:

			fprintf( kanal, infoformat,
							infile, (sind == 1) ? C[1].column : 0, G.count );
			fprintf( kanal, format, G.sum );
			fputc( '\n', kanal );
			sprintf( formbuf, "mean = %s\n", format );
			fprintf( kanal, formbuf, G.sum / G.count );
			sprintf( formbuf, "standard deviation = %s\n", format );
			fprintf( kanal, formbuf,
						    sqrt( (double)( (G.qsum - G.sum * G.sum / G.count)
						    		        / (G.count - 1) ) ) );
			sprintf( formbuf, "minimum = %s\n", format );
			fprintf( kanal, formbuf, G.min );
			sprintf( formbuf, "maximum = %s\n", format );
			fprintf( kanal, formbuf, G.max );
			fflush( kanal );
			break;

		default:
			kanal = stderr;
			break;
	}

	if( cfgetflag("print_date") )
		fprintf( kanal, "date: %s\n", cfgetstr("DATE") );
	if( cfgetflag("print_time") )
		fprintf( kanal, "time: %s\n", cfgetstr("TIME") );

	fclose( fin );
	fclose( fout );

	/**
	 * Save configuration to file (new or private setfile)
	 */
	if( (tmp1 = cfgetstr("COLSUM_SAVEINI")) != NULL )
		cfsave( (*tmp1=='\0') ? NULL : tmp1, "w" );

	return 0;
}

/******************************************************************************/
