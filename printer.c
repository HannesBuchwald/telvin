/*
 * printer.c
 *
 * Project:      Telvin Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    17.10.2017
 */

//#include "global.h"
#include "headers/printer.h"

static void
print_list_rest(FILE *out_f, TEL_OBJ theCons, enum printHowEnum print_style)
{
	while(C_TRUE)
	{
		print(out_f, car(theCons), print_style);
		if (isNil(cdr(theCons)))
		{
			fprintf(out_f, END_CH);
			return;
		}
		if (! isCons(cdr(theCons)))
		{
			fprintf(out_f, POINT_CH);
			print(out_f, cdr(theCons), print_style);
			fprintf(out_f, END_CH);
			return;
		}

		fprintf(out_f, SPASE_CH);
		theCons=cdr(theCons);
	}
	//print_list_rest(out_f, cdr(theCons), print_style);
}

void
print_list(FILE *out_f, TEL_OBJ theCons, enum printHowEnum print_style)
{
	fprintf(out_f,  START_CH );
	print(out_f, car(theCons), print_style);
	if (isNil(cdr(theCons)))
	{
		fprintf(out_f, END_CH);
		return;
	}
	if (! isCons(cdr(theCons)))
	{
		fprintf(out_f, POINT_CH);
		print(out_f, cdr(theCons), print_style);
		fprintf(out_f, END_CH);
		return;
	}

	fprintf(out_f, SPASE_CH);
	print_list_rest(out_f, cdr(theCons), print_style);
}

void print(FILE *out_f, TEL_OBJ tag_obj, enum printHowEnum print_style)
{
	tel_tag tag = tagOf(tag_obj);
	if (tag == T_SMALLINTEGER)
	{
		fprintf(out_f, "%ld", smallIntegerValue(tag_obj));
	}
	else if (tag == T_FULLINTEGER)
	{
		fprintf(out_f, "%ld", fullIntegerValue(tag_obj));
	}
	else if (tag == T_FLOAT)
	{
		fprintf(out_f, "%lf", floatValue(tag_obj));
	}
	else if (tag == T_SYMBOL)
	{
		fprintf(out_f, "%s", symbolValue(tag_obj));
	}
	else if (tag == T_STRING)
	{
		if (print_style == PRINT_WRITE)
		{
			fprintf(out_f, "\"%s\"", stringValue(tag_obj));
		}
		else
		{
			fprintf(out_f, "%s", stringValue(tag_obj));
		}

	}
	else if (tag == T_CONS)
	{
		print_list(out_f, tag_obj, print_style);
	}
	else if (tag == T_NIL)
	{
		fprintf(out_f, "()");
	}
	else if (tag == T_TRUE)
	{
		fprintf(out_f, "#t");
	}
	else if (tag == T_FALSE)
	{
		fprintf(out_f, "#f");
	}
	else if (tag == T_EOF)
	{
		fprintf(out_f, "#eof");
	}
	else if (tag == T_VOID)
	{
		fprintf(out_f, "#void");
	}
	else if (tag == T_BUILTINFUNC)
	{
		fprintf(out_f, "<some builtin function>");
	}
	else if (tag == T_BUILTINSYNTAX)
	{
		fprintf(out_f, "<some builtin syntax>");
	}
	else if (tag == T_USERDEFINEDFUNC)
	{
		fprintf(out_f, "(lambda ");
		print(out_f, tag_obj->tel_userDefinedFunc.argList, print_style);
		fprintf(out_f, SPASE_CH);
		print_list_rest(out_f, tag_obj->tel_userDefinedFunc.bodyList, print_style);
	}
	else
	{
		fprintf(out_f, UNIMPLEMEMT);
	}
}
