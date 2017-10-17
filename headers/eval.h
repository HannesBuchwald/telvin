/*
 * eval.h
 *
 * Project:      Telvin Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    17.10.2017
 */

#ifndef HEADERS_EVAL_H_
#define HEADERS_EVAL_H_

#include "global.h"
#include "environment.h"
#include "memory.h"
#include "stack.h"
#include "printer.h"

void initializeBuiltinFunctions();

TEL_OBJ
tel_eval(TEL_OBJ env, TEL_OBJ expr);

// if the two high bits are the same, their XOR is 0
// and the result is positive...
#define __ISVALIDSMALLINTEGER(i) (((i) ^ ((i)<<1)) >= 0)

// alternative form:
// #define __ISVALIDSMALLINTEGER(i) (((i) < 0) == (((i) << 1) < 0))
static inline TEL_OBJ
new_small_int(long value)   //new_smallInteger
{
	return (TEL_OBJ)((value << 1) | T_SMALLINTEGER);
}

static inline TEL_OBJ
new_integer(long value)
{
	if (__ISVALIDSMALLINTEGER(value))
	{
		return new_small_int(value);
	}
	return new_int(value);
}

#endif /* HEADERS_EVAL_H_ */
