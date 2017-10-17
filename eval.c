/*
 * eval.c
 *
 * Project:      Telvin Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    17.10.2017
 */




#include "headers/eval.h"

int trace_flag = C_FALSE;

//
// functions
//



static void
bCons(int numArgs)
{
	TEL_OBJ evaluatedCar, evaluatedCdr;
	TEL_OBJ newCons;

	if (numArgs != 2)
	{
		tel_error("(cons): two arguments expected", NULL);
	}
	evaluatedCdr = POP();
	evaluatedCar = POP();

	newCons = create_cons(evaluatedCar, evaluatedCdr);
	PUSH (newCons);
}

static void
bCar(int numArgs)
{
	TEL_OBJ evaluatedArg;

	if (numArgs != 1)
	{
		tel_error("(car): one arguments expected", NULL);
	}
	evaluatedArg = POP();
	PUSH( car(evaluatedArg) );
}

static void
bCdr(int numArgs)
{
	TEL_OBJ evaluatedArg;

	if (numArgs != 1)
	{
		tel_error("(cdr): one arguments expected", NULL);
	}
	evaluatedArg = POP();
	PUSH( cdr(evaluatedArg) );
}

static void
bPlus(int numArgs)
{
	long int sum = 0;
	// TEL_OBJ largeSum;

	while (--numArgs >= 0)
	{
		TEL_OBJ valueOfNextArg;

		valueOfNextArg = POP();
		if (isInteger(valueOfNextArg))
		{
			long arg = integerValue(valueOfNextArg);
			long newSum = sum + arg;
			if ((signOf(sum) == signOf(arg))
			        && (signOf(newSum) != signOf(arg)))
			{
				// overflow
				tel_error("integer overflow", NULL);
				// largeSum = new_largeInteger(newSum);
				// largeSum->sign = !signOf(newSum);
				// goto largeIntegerLoop;
			}
			sum = newSum;
		}
		else
		{
			tel_error("(+): non numeric argument: ", valueOfNextArg);
		}
	}
	PUSH( new_integer(sum) );
	return;
}

static void
bMinus(int numArgs)
{
	long diff = 0;
	TEL_OBJ valueOfNextArg;

	if (numArgs == 0)
	{
		tel_error("(-): at least one argument expected", NULL);
	}

	valueOfNextArg = POP();
	if (!isInteger(valueOfNextArg))
	{
		tel_error("(-): non numeric argument: ", valueOfNextArg);
	}
	diff = integerValue(valueOfNextArg);
	diff = -diff;

	if (numArgs == 1)
	{
		PUSH( new_integer( diff ) );
		return;
	}

	while (--numArgs > 1)
	{
		valueOfNextArg = POP();

		if (isInteger(valueOfNextArg))
		{
			long newDiff;

			newDiff = diff - integerValue(valueOfNextArg);
			diff = newDiff;
		}
		else
		{
			tel_error("(-): non numeric argument: ", valueOfNextArg);
		}
	}

	// the first arg: positive!
	valueOfNextArg = POP();
	if (isInteger(valueOfNextArg))
	{
		diff = diff + integerValue(valueOfNextArg);
	}
	else
	{
		tel_error("(-): non numeric argument: ", valueOfNextArg);
	}
	PUSH (new_integer(diff));
}

static void
bTimes(int numArgs)
{
	long product = 1;

	while (--numArgs >= 0)
	{
		TEL_OBJ valueOfNextArg;

		valueOfNextArg = POP();
		if (isInteger(valueOfNextArg))
		{
			product = product * integerValue(valueOfNextArg);
		}
		else
		{
			tel_error("(*): non numeric argument: ", valueOfNextArg);
		}
	}
	PUSH( new_integer(product) );
}

static void
bQuotient(int numArgs)
{
	long quotient;
	TEL_OBJ arg1;

	if (numArgs == 0)
	{
		tel_error("(/): at least one argument expected", NULL);
	}
	if (numArgs == 1)
	{
		// should return a fraction here (1 / arg)
		// as we have none, return a float...
		arg1 = POP();

		switch (tagOf(arg1))
		{
			case T_SMALLINTEGER:
			case T_FULLINTEGER:
				// PUSH( new_float( 1.0 / (double)(integerValue(arg1)) ) );
				PUSH( new_small_int(0) );
				return;

			default:
				tel_error("(/): non numeric argument", arg1);
				return;
		}
	}

	arg1 = STACK_SLOT(numArgs - 1);
	switch (tagOf(arg1))
	{
		case T_SMALLINTEGER:
		case T_FULLINTEGER:
			quotient = integerValue(arg1);
			break;

		default:
			tel_error("(/): non numeric argument", arg1);
			return;
	}

	while (--numArgs >= 1)
	{
		TEL_OBJ valueOfNextArg;

		valueOfNextArg = POP();
		if (isInteger(valueOfNextArg))
		{
			quotient = quotient / integerValue(valueOfNextArg);
		}
		else
		{
			tel_error("(/): non numeric argument: ", valueOfNextArg);
		}
	}
	// drop the first arg
	DROP(1);
	PUSH( new_integer(quotient) );
}

static void
bEq(int numArgs)
{
	if (numArgs != 2)
	{
		tel_error("(eq?): two arguments expected", NULL);
	}
	PUSH ((POP() == POP()) ? TEL_TRUE : TEL_FALSE);
}

static void
bEqNr(int numArgs)
{
	TEL_OBJ arg1, arg2;

	if (numArgs != 2)
	{
		tel_error("(=): two arguments expected", NULL);
		return;
	}
	arg2 = POP();
	arg1 = POP();
	switch (tagOf(arg1))
	{
		case T_SMALLINTEGER:
			PUSH (arg1 == arg2 ? TEL_TRUE : TEL_FALSE);
			return;

		case T_FULLINTEGER:
			switch (tagOf(arg2))
			{
				case T_SMALLINTEGER:
					PUSH (TEL_FALSE);
					return;

				case T_FULLINTEGER:
					PUSH (fullIntegerValue(arg1) == fullIntegerValue(arg2)
					      ? TEL_TRUE : TEL_FALSE);
					return;

				default:
					break;
			}
			break;

		default:
			break;
	}

	tel_error("(=): numeric argument expected: ", arg1);
	return;
}

static void
bDisplay(int numArgs)
{
	while (--numArgs >= 0)
	{
		TEL_OBJ evaluatedArg;

		evaluatedArg = POP();
		print(stdout, evaluatedArg, PRINT_DISPLAY);
	}
	PUSH( TEL_VOID );
}

// static void
// bTraceOn(int numArgs)
// {
// 	DROP(numArgs);
// 	trace_flag = C_TRUE;
// 	PUSH( TEL_VOID );
// }
//
// static void
// bTraceOff(int numArgs)
// {
// 	DROP(numArgs);
// 	trace_flag = C_FALSE;
// 	PUSH( TEL_VOID );
// }


//
// syntax
//
#ifdef RECURSIVE

static void
builtinDefine(TEL_OBJ env, TEL_OBJ args)
{
	TEL_OBJ theUnevaluatedNameArg, theUnevaluatedExprArg;
	TEL_OBJ evaluatedExpr;

	if (isNil(args))
	{
		tel_error("(define): two arguments expected", NULL);
	}
	theUnevaluatedNameArg = car(args);

	args = cdr(args);
	if (isNil(args))
	{
		tel_error("(define): two arguments expected", NULL);
	}
	theUnevaluatedExprArg = car(args);

	if (! isNil(cdr(args)))
	{
		tel_error("(define): more than two arguments given", NULL);
	}

	if (! isSymbol(theUnevaluatedNameArg))
	{
		tel_error("(define): non-symbol argument: ", theUnevaluatedNameArg);
	}
	evaluatedExpr = tel_eval(env, theUnevaluatedExprArg);

	add_binding(topEnvironment, theUnevaluatedNameArg, evaluatedExpr);
	PUSH( TEL_VOID );
}

static void
builtinLambda(TEL_OBJ env, TEL_OBJ args)
{
	TEL_OBJ theUnevaluatedArglist, theUnevaluatedBodyList;

	if (isNil(args))
	{
		tel_error("(lambda): at least two arguments expected", NULL);
	}
	theUnevaluatedArglist = car(args);

	args = cdr(args);
	if (isNil(args))
	{
		tel_error("(lambda): at least two arguments expected", NULL);
	}
	theUnevaluatedBodyList = args;

	PUSH( new_userDefinedFunction(theUnevaluatedArglist, theUnevaluatedBodyList) );
}

static void
builtinQuote(TEL_OBJ env, TEL_OBJ args)
{
	TEL_OBJ theUnevaluatedArg;

	if (isNil(args))
	{
		tel_error("(quote): one argument expected", NULL);
	}
	theUnevaluatedArg = car(args);

	if (! isNil(cdr(args)))
	{
		tel_error("(quote): more than one argument given", NULL);
	}

	PUSH( theUnevaluatedArg );
}

static void
builtinIf(TEL_OBJ env, TEL_OBJ args)
{
	TEL_OBJ theUnevaluatedCondition, theUnevaluatedIfExpr, theUnevaluatedElseExpr;
	TEL_OBJ evaluatedCondition;
	TEL_OBJ val;

	if (isNil(args))
	{
		tel_error("(if): three arguments expected", NULL);
	}
	theUnevaluatedCondition = car(args);

	args = cdr(args);
	if (isNil(args))
	{
		tel_error("(if): three arguments expected", NULL);
	}
	theUnevaluatedIfExpr = car(args);

	args = cdr(args);
	if (isNil(args))
	{
		tel_error("(if): three arguments expected", NULL);
	}
	theUnevaluatedElseExpr = car(args);

	if (! isNil(cdr(args)))
	{
		tel_error("(if): more than three arguments given", NULL);
	}

	evaluatedCondition = tel_eval(env, theUnevaluatedCondition);

	if (isTrue(evaluatedCondition))
	{
		val = tel_eval(env, theUnevaluatedIfExpr);
	}
	else
	{
		val = tel_eval(env, theUnevaluatedElseExpr);
	}
	PUSH(val);
}

#else

VOIDPTRPTRFUNC CP_eval();
static VOIDPTRPTRFUNC builtinDefine2();

static VOIDPTRPTRFUNC
bDefine(TEL_OBJ env, TEL_OBJ args)
{
	TEL_OBJ theUnevaluatedNameArg, theUnevaluatedExprArg;

	if (isNil(args))
	{
		tel_error("(define): two arguments expected", NULL);
	}
	theUnevaluatedNameArg = car(args);

	args = cdr(args);
	if (isNil(args))
	{
		tel_error("(define): two arguments expected", NULL);
	}
	theUnevaluatedExprArg = car(args);

	if (! isNil(cdr(args)))
	{
		tel_error("(define): more than two arguments given", NULL);
	}

	if (! isSymbol(theUnevaluatedNameArg))
	{
		tel_error("(define): non-symbol argument: ", theUnevaluatedNameArg);
	}
	// remember these
	PUSH(env);
	PUSH(theUnevaluatedNameArg);

	PUSH(env);
	PUSH(theUnevaluatedExprArg);
	PUSH_CP( (VOIDPTRPTRFUNC) builtinDefine2);
	return (VOIDPTRPTRFUNC)CP_eval;
}

static VOIDPTRPTRFUNC
builtinDefine2()
{
	TEL_OBJ evaluatedExpr = POP();
	TEL_OBJ theUnevaluatedNameArg = POP();
	TEL_OBJ env = POP();

	add_binding(env, theUnevaluatedNameArg, evaluatedExpr);
	PUSH( TEL_VOID );
	return POP_CP();
}

static VOIDPTRPTRFUNC
bLambda(TEL_OBJ env, TEL_OBJ args)
{
	TEL_OBJ theUnevaluatedArglist, theUnevaluatedBodyList;

	if (isNil(args))
	{
		tel_error("(lambda): at least two arguments expected", NULL);
	}
	theUnevaluatedArglist = car(args);

	args = cdr(args);
	if (isNil(args))
	{
		tel_error("(lambda): at least two arguments expected", NULL);
	}
	theUnevaluatedBodyList = args;

	PUSH( create_user_def_func(theUnevaluatedArglist, theUnevaluatedBodyList) );
	return POP_CP();
}

static VOIDPTRPTRFUNC
bQuote(TEL_OBJ env, TEL_OBJ args)
{
	TEL_OBJ theUnevaluatedArg;

	if (isNil(args))
	{
		tel_error("(quote): one argument expected", NULL);
	}
	theUnevaluatedArg = car(args);

	if (! isNil(cdr(args)))
	{
		tel_error("(quote): more than one argument given", NULL);
	}

	PUSH( theUnevaluatedArg );
	return POP_CP();
}

static VOIDPTRPTRFUNC builtinIf2();

static VOIDPTRPTRFUNC
bIf(TEL_OBJ env, TEL_OBJ args)
{
	TEL_OBJ theUnevaluatedCondition, theUnevaluatedIfExpr, theUnevaluatedElseExpr;

	if (isNil(args))
	{
		tel_error("(if): three arguments expected", NULL);
	}
	theUnevaluatedCondition = car(args);

	args = cdr(args);
	if (isNil(args))
	{
		tel_error("(if): three arguments expected", NULL);
	}
	theUnevaluatedIfExpr = car(args);

	args = cdr(args);
	if (isNil(args))
	{
		tel_error("(if): three arguments expected", NULL);
	}
	theUnevaluatedElseExpr = car(args);

	if (! isNil(cdr(args)))
	{
		tel_error("(if): more than three arguments given", NULL);
	}

	PUSH(env);
	PUSH(theUnevaluatedIfExpr);
	PUSH(theUnevaluatedElseExpr);

	PUSH(env);
	PUSH(theUnevaluatedCondition);
	PUSH_CP( (VOIDPTRPTRFUNC)builtinIf2 );
	return (VOIDPTRPTRFUNC)CP_eval;
}

static VOIDPTRPTRFUNC
builtinIf2()
{
	TEL_OBJ evaluatedCondition = POP();
	TEL_OBJ theUnevaluatedElseExpr = POP();
	TEL_OBJ theUnevaluatedIfExpr = POP();

	if (isTrue(evaluatedCondition))
	{
		PUSH(theUnevaluatedIfExpr);
	}
	else
	{
		PUSH(theUnevaluatedElseExpr);
	}
	return (VOIDPTRPTRFUNC)CP_eval;
}


#endif

//
// evaluator
//
#ifdef RECURSIVE

static int callDepth = 0;

void
tel_evalList(TEL_OBJ env, TEL_OBJ expr)
{
	TEL_OBJ unevaluatedFunc = car(expr);
	TEL_OBJ unevaluatedArgs = cdr(expr);
	TEL_OBJ func;

	func = tel_eval(env, unevaluatedFunc);
	switch (tagOf(func))
	{
		case T_BUILTINFUNC:
		{
			int numArgs = 0;

			while (! isNil(unevaluatedArgs))
			{
				TEL_OBJ nextUnevaluatedArg = car(unevaluatedArgs);

				unevaluatedArgs = cdr(unevaluatedArgs);
				PUSH( tel_eval(env, nextUnevaluatedArg) );
				numArgs++;
			}
			(*func->tel_builtinFunc.code)(numArgs);
			return;
		}

		case T_BUILTINSYNTAX:
			(*func->tel_builtinSyntax.code)(env, unevaluatedArgs);
			return;

		case T_USERDEFINEDFUNC:
		{
			TEL_OBJ funcEnv = allocateEnvironment(10);
			TEL_OBJ formalArgs = func->tel_userDefinedFunc.argList;
			TEL_OBJ bodyList = func->tel_userDefinedFunc.bodyList;

			funcEnv->tel_environment.parent = topEnvironment;
			{
				TEL_OBJ nextFormalArgCell = formalArgs;
				TEL_OBJ nextActualUnevaluatedArgCell = unevaluatedArgs;
				TEL_OBJ lastValue;

				while (!isNil(nextFormalArgCell))
				{
					TEL_OBJ argName = car(nextFormalArgCell);
					TEL_OBJ nextUnevaluatedArg;
					TEL_OBJ nextEvaluatedArg;
					nextFormalArgCell = cdr(nextFormalArgCell);

					if (!isCons(nextActualUnevaluatedArgCell))
					{
						tel_error("argument count mismatch", formalArgs);
					}
					nextUnevaluatedArg = car(nextActualUnevaluatedArgCell);
					nextActualUnevaluatedArgCell = cdr(nextActualUnevaluatedArgCell);

					nextEvaluatedArg = tel_eval(env, nextUnevaluatedArg);
					add_binding(funcEnv, argName, nextEvaluatedArg);
				}
				while (!isNil(bodyList))
				{
					TEL_OBJ nextBodyExpr = car(bodyList);
					bodyList = cdr(bodyList);
					lastValue = tel_eval(funcEnv, nextBodyExpr);
				}
				PUSH( lastValue );
				return;
			}
		}

		default:
			tel_error("non function in function slot: ", func);
	}
}

void
tel_eval2(TEL_OBJ env, TEL_OBJ expr)
{
	TEL_OBJ val;

	if (trace_flag)
	{
		int i;

		for (i = 0; i < callDepth; i++)
		{
			fprintf(stderr, "  ");
		}
		fprintf(stderr, ">>>> ");
		tel_print(stderr, expr, PRINT_WRITE);
		fprintf(stderr, "\n");
	}

	switch (tagOf(expr))
	{
		case T_SYMBOL:
			val = get_binding(env, expr);
			if (val == NULL)
			{
				tel_error("no binding for: ", expr);
			}
			PUSH( val );
			return;

		case T_CONS:
			callDepth++;
			tel_evalList(env, expr);
			callDepth--;
			return;

		default:
			PUSH( expr );
			return;
	}
}

TEL_OBJ
tel_eval(TEL_OBJ env, TEL_OBJ expr)
{
	tel_eval2(env, expr);
	return POP();
}

#else

// stack layout:
//
//   env
//   expr

static VOIDPTRPTRFUNC CP_eval2();

VOIDPTRPTRFUNC
CP_eval()
{
	TEL_OBJ expr = POP();
	TEL_OBJ env = POP();
	TEL_OBJ val;

	if (trace_flag && isCons(expr))
	{
		int callDepth = (int)(Return_Stack_P - Return_Stack);
		int i;

		for (i = 0; i < callDepth; i++)
		{
			fprintf(stderr, "  ");
		}
		fprintf(stderr, ">>>> ");
		print(stderr, expr, PRINT_WRITE);
		fprintf(stderr, "\n");
	}

	switch (tagOf(expr))
	{
		case T_SYMBOL:
			val = get_binding(env, expr);
			if (val == NULL)
			{
				tel_error("no binding for: ", expr);
			}
			PUSH( val );
			return POP_CP();

		case T_CONS:
			// remember those
			PUSH(env);
			PUSH(expr);
			// printf("sp: %d [%d]\n", (int)(evalSP - evalStack), __LINE__);
			// evaluate the car (func slot)
			PUSH(env);
			PUSH(car(expr));
			PUSH_CP( (VOIDPTRPTRFUNC)CP_eval2 );
			return (VOIDPTRPTRFUNC)CP_eval;

		default:
			PUSH( expr );
			return POP_CP();
	}
}

//
// continuation after the car of a list-expression
// has been evaluated;
// stack is:
//
//   env
//   expr
//   evaluated funcSlot i.e. evaluated car(expr)

VOIDPTRPTRFUNC CP_eval3();

static VOIDPTRPTRFUNC
CP_eval2()
{
	TEL_OBJ func, expr, env;
	TEL_OBJ nextUnevaluatedArgCell;

	func = POP(); // eval result

	// printf("sp: %d [%d]\n", (int)(evalSP - evalStack), __LINE__);
	expr = POP();
	env = POP();
	nextUnevaluatedArgCell = cdr(expr);

	switch (tagOf(func))
	{
		case T_USERDEFINEDFUNC:
		case T_BUILTINFUNC:
			// for functions, evaluate all args onto the stack...
			if (isNil(nextUnevaluatedArgCell))
			{
				// no args
				if (tagOf(func) == T_BUILTINFUNC)
				{
					(*func->tel_builtinFunc.code)(0);
					return POP_CP();
				}

				// no need for extra environment - but check if
				// func really wants no arg
				if (!isNil(func->tel_userDefinedFunc.argList))
				{
					tel_error("argument count mismatch (not enough args):", new_small_int(0));
				}
				ERROR("unimpl.");
			}

			PUSH(func);

			// remember those...
			PUSH(env);
			PUSH(expr);
			PUSH(cdr(nextUnevaluatedArgCell));
			PUSH((TEL_OBJ)0);  // numArgs

			// evaluate the first argument...
			PUSH(env);
			PUSH(car(nextUnevaluatedArgCell));
			PUSH_CP( (VOIDPTRPTRFUNC)CP_eval3 );
			return (VOIDPTRPTRFUNC)CP_eval;

		case T_BUILTINSYNTAX:
			return (VOIDPTRPTRFUNC)
			       (*func->tel_builtinSyntax.cpCode)(env, nextUnevaluatedArgCell);

		default:
			tel_error("non function in function slot: ", func);
			return NULL;
	}
}

//
// continuation after one argument a list-expression
// has been evaluated;
//
// stack is:
//
//   previousEvaluateArgs
//   func
//   env
//   expr
//   nextUnevaluatedArgCell
//   numArgs
//   evaluatedArg
//
VOIDPTRPTRFUNC CP_eval4();

VOIDPTRPTRFUNC
CP_eval3()
{
	TEL_OBJ evaluatedArg, nextUnevaluatedArgCell, expr, env;
	TEL_OBJ func;
	int numArgs;

	evaluatedArg = POP();

	numArgs = ((long)POP()) + 1;
	nextUnevaluatedArgCell = POP();
	expr = POP();
	env = POP();
	func = POP();

	PUSH(evaluatedArg);

	// stack now:
	//
	//   evaluatedArg(s)

	// printf("sp: %d [%d]\n", (int)(evalSP - evalStack), __LINE__);

	// more args?
	if (!isNil(nextUnevaluatedArgCell))
	{
		// remember those...
		PUSH(func);
		PUSH(env);
		PUSH(expr);
		PUSH(cdr(nextUnevaluatedArgCell));
		PUSH( (TEL_OBJ)(long)numArgs );

		// evaluate the next argument...
		PUSH(env);
		PUSH(car(nextUnevaluatedArgCell));
		PUSH_CP( (VOIDPTRPTRFUNC)CP_eval3 );
		return (VOIDPTRPTRFUNC)CP_eval;
	}

	// stack now:
	//
	//   evaluatedArgs
	if (tagOf(func) == T_BUILTINFUNC)
	{
		// pops args; pushes result.
		(*func->tel_builtinFunc.code)(numArgs);
		return POP_CP();
	}

	// pop values and bind in a new environment
	//
	{
		TEL_OBJ funcEnv = alloc_environment(10);
		TEL_OBJ formalArgs = func->tel_userDefinedFunc.argList;
		TEL_OBJ bodyList = func->tel_userDefinedFunc.bodyList;
		TEL_OBJ nextFormalArgCell = formalArgs;
		int slotIndex = numArgs - 1;

		funcEnv->tel_environment.parent = topEnvironment;

		while (!isNil(nextFormalArgCell))
		{
			TEL_OBJ nextEvaluatedArg;
			TEL_OBJ argName = car(nextFormalArgCell);
			nextFormalArgCell = cdr(nextFormalArgCell);

			if (slotIndex < 0)
			{
				tel_error("argument count mismatch (not enough args):", new_small_int(numArgs));
			}

			nextEvaluatedArg = STACK_SLOT(slotIndex);
			add_binding(funcEnv, argName, nextEvaluatedArg);
			slotIndex--;
		}
		if (slotIndex != -1)
		{
			tel_error("argument count mismatch (too many args):", new_small_int(numArgs));
		}


		// if there is only one bodyExpr,
		// evaluate with a tail call;
		// otherwise, evaluate with a call, continuaing in CP_eval4.
		if (!isCons(bodyList))
		{
			tel_error("invalid function body", NULL);
		}
		if (isNil(cdr(bodyList)))
		{
			// tail call
			DROP(numArgs);
			PUSH(funcEnv);
			PUSH(car(bodyList));
			return (VOIDPTRPTRFUNC)CP_eval;
		}
		// regular call
		DROP(numArgs);

		// remember those
		PUSH(funcEnv);
		PUSH(cdr(bodyList));

		PUSH(funcEnv);
		PUSH(car(bodyList));
		PUSH_CP( (VOIDPTRPTRFUNC)CP_eval4 );
		return (VOIDPTRPTRFUNC)CP_eval;
	}
	// not reached
	return NULL;
}

// one body expr has been evaluated;
// care for the next one
// stack is:
//      funcEnv
//      rest bodyList
//      lastValue

VOIDPTRPTRFUNC
CP_eval4()
{
	TEL_OBJ lastValue, restBodyList, funcEnv;

	lastValue = POP();
	restBodyList = POP();
	funcEnv = POP();

	if (!isCons(restBodyList))
	{
		tel_error("invalid function body", NULL);
	}
	if (isNil(cdr(restBodyList)))
	{
		// tail call
		PUSH(funcEnv);
		PUSH(car(restBodyList));
		return (VOIDPTRPTRFUNC)CP_eval;
	}

	// regular call
	// remember those
	PUSH(funcEnv);
	PUSH(cdr(restBodyList));

	PUSH(funcEnv);
	PUSH(car(restBodyList));
	PUSH_CP( (VOIDPTRPTRFUNC)CP_eval4 );
	return (VOIDPTRPTRFUNC)CP_eval;
}

TEL_OBJ
tel_eval(TEL_OBJ env, TEL_OBJ expr)
{
	PUSH(env);
	PUSH(expr);
	PUSH_CP( NULL );
	trampoline( (VOIDPTRPTRFUNC)CP_eval );
	return POP();
}

#endif

void
initializeBuiltinFunctions()
{
	TEL_OBJ fn;

	fn = create_built_func(bPlus);
	add_binding(topEnvironment, create_symbol("+"), fn);
	add_binding(topEnvironment, create_symbol("plus"), fn);

	fn = create_built_func(bTimes);
	add_binding(topEnvironment, create_symbol("*"), fn);

	fn = create_built_func(bQuotient);
	add_binding(topEnvironment, create_symbol("/"), fn);

	fn = create_built_func(bMinus);
	add_binding(topEnvironment, create_symbol("-"), fn);
	add_binding(topEnvironment, create_symbol("minus"), fn);

	// ----------

	fn = create_built_func(bCons);
	add_binding(topEnvironment, create_symbol("cons"), fn);

	fn = create_built_func(bCar);
	add_binding(topEnvironment, create_symbol("car"), fn);
	fn = create_built_func(bCdr);
	add_binding(topEnvironment, create_symbol("cdr"), fn);

	fn = create_built_func(bEq);
	add_binding(topEnvironment, create_symbol("eq?"), fn);

	fn = create_built_func(bEqNr);
	add_binding(topEnvironment, create_symbol("="), fn);

	fn = create_built_func(bDisplay);
	add_binding(topEnvironment, create_symbol("display"), fn);

	// fn = create_built_func(bTraceOn);
	// add_binding(topEnvironment, create_symbol("traceOn"), fn);
	//  fn = create_built_func(bTraceOff);
	//    add_binding(topEnvironment, create_symbol("traceOff"), fn);

	// ----------

	fn = create_built_syntax((VOIDPTRPTRFUNC)bDefine);
	add_binding(topEnvironment, create_symbol("define"), fn);

	fn = create_built_syntax((VOIDPTRPTRFUNC)bIf);
	add_binding(topEnvironment, create_symbol("if"), fn);

	fn = create_built_syntax((VOIDPTRPTRFUNC)bQuote);
	add_binding(topEnvironment, create_symbol("quote"), fn);

	fn = create_built_syntax((VOIDPTRPTRFUNC)bLambda);
	add_binding(topEnvironment, create_symbol("lambda"), fn);
}
