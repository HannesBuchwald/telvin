/*
 * memory.h
 *
 * Project:      Telvin Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    17.10.2017
 */

#ifndef HEADERS_MEMORY_H_
#define HEADERS_MEMORY_H_

#include "global.h"

#define USE_REAL_HASH
#define INITIAL_SYMBOLTABLE_SIZE    950

extern TEL_OBJ create_user_def_func(TEL_OBJ, TEL_OBJ);//new_userDefinedFunction

TEL_OBJ create_built_func(VOIDFUNC funcPtr) ;//new_builtinFunc

TEL_OBJ create_built_syntax(VOIDPTRPTRFUNC syntaxPtr); //new_builtinSyntax

TEL_OBJ  new_int(long iVal); //really_new_integer

// chi ogtagorcvum
TEL_OBJ create_new_int(long iVal); //xxxnew_integer

TEL_OBJ create_cons(TEL_OBJ theCar, TEL_OBJ theCdr); //new_cons

TEL_OBJ create_symbol(char *chars) ;//new_symbol

void init_symbol_table();//initializeSymbolTable

void append_symbol_table(); //growSymbolTable + possiblyGrowSymbolTable

TEL_OBJ
create_string(char *chars) ;//new_string

TEL_OBJ
create_eof(); //new_eof

TEL_OBJ
create_true() ; // new_true

TEL_OBJ
create_false() ; //new_false

TEL_OBJ
create_void() ; //new_void
TEL_OBJ
create_nil() ; //new_nil

#endif /* HEADERS_MEMORY_H_ */
