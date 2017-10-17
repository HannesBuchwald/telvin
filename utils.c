/*
 * global.c
 *
 * Project:      Telvin Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    17.10.2017
 */



//#include "global.h"
#include "headers/stack.h"

void
init_eval_stack()
{
	Input_Stack = (TEL_OBJ *)malloc(sizeof(TEL_OBJ) * INIT_EV_STACK_S);
	Input_Stack_P = Input_Stack;
	Input_Stack_Top = &(Input_Stack[INIT_EV_STACK_S]);
}

void
append_eval_stack()
{
	int oldSize = (int)(Input_Stack_P - Input_Stack);
	int newSize = oldSize * 2;

	Input_Stack = (TEL_OBJ *)realloc(Input_Stack, sizeof(TEL_OBJ) * newSize);
	Input_Stack_P = &Input_Stack[oldSize];
	Input_Stack_Top = &(Input_Stack[newSize]);
}

void
initi_return_stack()
{
	Return_Stack = (VOIDPTRPTRFUNC *)malloc(sizeof(VOIDPTRPTRFUNC) * INIT_RET_STACK_S);
	Return_Stack_P = Return_Stack;
	Return_Stack_Top = &(Return_Stack[INIT_RET_STACK_S]);
}

void
append_return_stack()
{
	int oldSize = (int)(Return_Stack_P - Return_Stack);
	int newSize = oldSize * 2;

	Return_Stack = (VOIDPTRPTRFUNC *)realloc(Return_Stack, sizeof(VOIDPTRPTRFUNC) * newSize);
	Return_Stack_P = &Return_Stack[oldSize];
	Return_Stack_Top = &(Return_Stack[newSize]);
}
