/*******************************************************************************
 *            COLSUM - Column Computation Tool
 ***************************************************************************//**
 *
 * @file colsum.h
 *
 * Header file for colsum.c and colmod.c
 *
 * - Include @ref cflib Header
 * - General Settings
 * - Error Codes
 * - Output Modes for usage()
 * - Mathematical Operation Instruction Codes
 * - Data Structure Definitions
 * - Function Prototypes
 *
 * @version   0.6.2
 * @version   SVN: \$Id: colsum.h 46 2009-02-01 20:12:09Z stefan$
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
 * 2009-01-31 [sh] eclipse project, doxygen documentation
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <math.h>

/**
 * @defgroup cflib Flexible Configuration Library
 * @{
 * @details The open source C library @b CFLIB provides to @ref colsum :
 * - configuration interface
 * - command line parsing
 * - configuration files handling
 * - user interface
 * - string utilities
 *
 * @see CFLIB Project Homepage http://cflib.berlios.de
 * @see CFLIB Library Documentation
 *   http://svn.berlios.de/svnroot/repos/cflib/trunk/cflib/doc/html/index.html
 *
 * We always include the
 * - CFLIB @b @b header file @ref cf.h matching the @b Patchlevel of the
 * - CFLIB @b library @b @b file @e libcf.a (or other name) to be linked
 *   (see @ref development of CFLIB).
 *
 * Colsum uses the following CFLIB functions and @e @e function macros:
 * - @ref cfinit()
 * - @ref cfsave()
 * - @ref cfputstr()
 * - @e cfgetstr()
 * - @e cfgetflag()
 * - @e cfgetnum()
 * - @e cfgetres()
 * - @ref cfgeterr()
 * - @ref cfgetusg()
 * - @ref cfdump()
 * - @ref EatWhiteSpace()
 * - @ref RemoveCR()
 * @}
 */
#include "cf.h"

/* General Settings */
#define VERSION    "colsum 0.6.2\n(c) Stefan Habermehl 1995-2009"
                                /**< Colsum Version String     */
#define MAXLINE    10240        /**< Maximum Input Line Length */
#define MAXFORM    100          /**< Maximum Format Length     */
#define MAXARG     90           /**< Maximum Argument Length   */
#define SPAMAX     70           /**< Maximum Column Number     */

#define VMIN       0            /**< Minimal Verbosity Level   */
#define VMAX       10           /**< Maximal Verbosity Level   */
#define DEF_VERB   "2"          /**< Default Verbosity Level   */
#define DEF_COLUMN "1"          /**< Default Column No.        */
#define DEF_FORMAT "%-15.15g"   /**< Default Output Format     */
                                /** Usage Message Format String */
#define DEF_USAGE  "Usage: %s \
[[-i] <infile>] [-o <outfile>] [-v#] [-r <ini-file>] [-h] [-t] [-d]\n\
\t[-s<save-ini-file>] [-k] [-f%%<format-specification>] [c|C<column>:#1[:#2]]\n\
\t{[<column>] [=[+|-|*|x|/|^|a|&|n|||o][+|-]#.#|S<column>[^]] [+|*]} {...} ...\n"

#undef TRUE
#undef FALSE
#define TRUE  '\1'          /**< Boolean True as Char 1 (1 Byte) */
#define FALSE '\0'          /**< Boolean False as Char 0 (1 Byte) */

/**
 * @defgroup usage User Interface and Error Codes
 * @{
 */
/* Error Codes */
#define ERR_HLP   0             /**< Detailed HeLP on options  */
#define ERR_USG   1             /**< USaGge Error              */
#define ERR_VERB  2             /**< Unknown VERBosity Level   */
#define ERR_MATH  3             /**< MATHematical Error        */
#define ERR_LIM   4             /**< LIMit Violation Error     */
#define ERR_ACC   5             /**< ACCess Error              */
#define ERR_ARG   6             /**< ARGument Error            */

/* Usage Output Options */
#define USG_DEFAULT   0         /**< Default Usage Message     */
#define USG_VERSION   1         /**< Show Version              */
#define USG_LICENSE   2         /**< Show License              */
#define USG_ERRORS    4         /**< Show errors               */
#define USG_DUMP      8         /**< Dump configuration        */

/**
 * @}
 * @defgroup parse_mathexp Parse Mathematical Operation Instructions
 * @{
 */
#define OP_DIV   -2         /**< Divide         */
#define OP_MUL   -1         /**< Multiply       */
#define OP_NONE   0         /**< No Operation   */
#define OP_ADD    1         /**< Add            */
#define OP_SUB    2         /**< Substract      */
#define OP_POWE   3         /**< Raise to Power, Exponent */
#define OP_POWB   4         /**< Raise to Power, Base     */
#define OP_ABS    5         /**< Absolute       */
#define OP_AND    6         /**< Bitwise And    */
#define OP_OR     7         /**< Bitwise Or     */
/** @} */

/**
 * Column Structure
 *
 * An input table will be represented by an array of
 * SPAMAX column structures (see define above).
 *
 * The first element (index @c 0) of the columns array holds the
 * current/last result of computations for one row, while all the
 * others (@c index = @c 1 - @ref SPAMAX) refer to corresponding
 * columns in input.
 */
typedef struct _COLUMN{
    int    column;    /**< input column-no. */
    char   *spoi;     /**< pointer to input column in line string */
    int    operation; /**< kind of operation to apply to that column */
    double value;     /**< constant value for operation */
    int    valsp;     /**< no. of column to use instead of constant value for operation */
    double faktor;    /**< factor for operation */
    double item;      /**< column's result */
    char   *sop;      /**< kind of stack operation: C[0].item=C[0].item +|* C[i].item */
} COLUMN;

/**
 * Colsum Result Structure for an input column or aggregate
 *
 * An input table will be represented by an array of
 * @ref SPAMAX @ref COLUMN structures.
 */
typedef struct _COLRESULT{
	int    count;     /**< count significant lines */
	int    scount;    /**< count ? */
	double sum;       /**< stack sum */
	double qsum;      /**< stack square sum */
	double max;       /**< stack maximum */
	double min;       /**< stack minimum */
	int    limcol;    /**< no. of column to check for limiting condition */
	double limlow;    /**< lower limit */
	double limupp;    /**< upper limit */
} COLRESULT;

/* Function Prototypes */
void usage( int code, char *str, int modemask );
void parse_mathexp( int sind, char *expr, COLUMN *C, int *max_column );

/******************************************************************************/
