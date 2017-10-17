/*
 * printer.h
 *
 * Project:      Telvin Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    17.10.2017
 */

#ifndef PRINTER_H_
#define PRINTER_H_

#include "global.h"

enum printHowEnum
{
    PRINT_DISPLAY,
    PRINT_WRITE
};

#define START_CH 	"("
#define END_CH 		")"
#define SPASE_CH 	" "
#define POINT_CH 	" . "
#define UNIMPLEMEMT	"print unimplemented"

static inline double
floatValue(TEL_OBJ obj)
{
	CHECK(isFloat(obj), "not a float");
	return (obj->tel_float.fVal);
}

static inline char *
symbolValue(TEL_OBJ obj)
{
	CHECK(isSymbol(obj), "not a symbol");
	return (obj->tel_symbol.chars);
}

static inline char *
stringValue(TEL_OBJ obj)
{
	CHECK(isString(obj), "not a string");
	return (obj->tel_string.chars);
}



/*
 * print realisation for T_CONS result
 */
void print_list(FILE *outFile, TEL_OBJ theCons, enum printHowEnum printHow);// tel_printList

/*
 * print result by necessarily function decide it by input T
 *
 */
void print (FILE *outFile, TEL_OBJ o, enum printHowEnum printHow); // tel_print

#endif /* PRINTER_H_ */
