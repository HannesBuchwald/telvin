/*
 * stack.h
 *
 * Project:      Telvin Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    17.10.2017
 */

#ifndef STACK_H_
#define STACK_H_

#include "global.h"

//TEL_OBJ* evalStack = NULL;
//TEL_OBJ* evalSP = NULL;
//TEL_OBJ* evalStackTop = NULL;
//
//VOIDPTRPTRFUNC* returnStack;
//VOIDPTRPTRFUNC* returnSP;
//VOIDPTRPTRFUNC* returnStackTop;

/*
 * initialize stacks for input parametrs
 */
void init_eval_stack(); // initializeEvalStack

/*
 * increase the current input-stack size 2 times
 */
void append_eval_stack(); // growEvalStacke

/*
 * initialize stacks for input parametrs
 */
void initi_return_stack(); //initializeReturnStack

/*
 * increase the current output-stack size 2 times
 */
void append_return_stack(); // growContinuationStack

#endif /* STACK_H_ */
