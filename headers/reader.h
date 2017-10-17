/*
 * reader.h
 *
 * Project:      Telvin Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    17.10.2017
 */

#ifndef HEADERS_READER_H_
#define HEADERS_READER_H_

#include "global.h"
#include "memory.h"
#include "eval.h"


#define START_LIST 	'('
#define END_LIST 	')'
#define QUOTE1_CH 	'\''
#define QUOTE2_CH 	'"'

#define TRUE_CH		"#t"
#define FALSE_CH	"#f"
#define VOID_CH		"#void"
#define NULL_CH		"nil"
#define SHARP_CH	'#'
#define MINUS_CH 	'-'
#define SLASH_CH 	'\\'

extern tel_stream new_fileStream(FILE *);
extern long a2l(char *);
extern TEL_OBJ tel_readFromCString(char *);
extern TEL_OBJ tel_read(tel_stream input);
#endif /* HEADERS_READER_H_ */
