/*
 * main.h
 *
 * Project:      Telvin Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    17.10.2017
 */

#ifndef HEADERS_MAIN_H_
#define HEADERS_MAIN_H_

#include "environment.h"
#include "eval.h"
#include "memory.h"
#include "reader.h"
#include "stack.h"
#include "printer.h"

#include <time.h>

#ifndef __BORLANDC__
# include <sys/time.h>
# include <sys/timeb.h>

# define USE_GETTIMEOFDAY
#endif

long
millisecondTime();


void init_system() ;

VOIDPTRPTRFUNC CP_repl1();

VOIDPTRPTRFUNC CP_repl();

#endif /* HEADERS_MAIN_H_ */
