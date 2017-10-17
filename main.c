/*
 * main.c
 *
 * Project:      Telvin Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    17.10.2017
 */


//#include "global.h"
#include "setjmp.h"
#include "headers/main.h"


#ifdef DEBUG
# define DBG_PRINT(x) fprintf x
#else
# define DBG_PRINT(x) /* as nothing */
#endif

static jmp_buf getMeBackToRepl;

TEL_OBJ TEL_NIL = NULL;
TEL_OBJ TEL_TRUE = NULL;
TEL_OBJ TEL_FALSE = NULL;
TEL_OBJ TEL_VOID = NULL;
TEL_OBJ TEL_EOF = NULL;
TEL_OBJ TEL_quoteSymbol = NULL;
static tel_stream stdInputStream = NULL;



void
tel_error(char *msg, TEL_OBJ optionalArg)
{
	fprintf(stderr, "%s", msg);
	if (optionalArg != NULL)
	{
		print(stderr, optionalArg, PRINT_WRITE);
	}
	fprintf(stderr, "\n");
	DBG_PRINT((stderr, "going back to REPL...\n"));
	longjmp(getMeBackToRepl, 1);
}

long
millisecondTime()
{
	long millis;

#ifdef USE_GETTIMEOFDAY
	struct timeval tv;
	//struct timezone tz;

	if (gettimeofday(&tv, NULL) < 0)
	{
		ERROR("gettimeofday failed");
	}
	millis = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
#else
# ifdef __BORLANDC__
# else
	struct timeb timebuffer;

	ftime(&timebuffer);
	millis = (timebuffer.time * 1000) + timebuffer.millitm;
# endif
#endif
	return millis;
}

void init_system()
{
	stdInputStream = new_fileStream(stdin);
	TEL_NIL = create_nil();
	TEL_TRUE = create_true();
	TEL_FALSE = create_false();
	TEL_VOID = create_void();
	TEL_EOF = create_eof();

	init_symbol_table();

	TEL_quoteSymbol = create_symbol("quote");

	initi_return_stack();
	init_eval_stack();
	init_environment();
	initializeBuiltinFunctions();
}

void
trampoline(VOIDPTRPTRFUNC fn)
{
	while (fn != NULL)
	{
		fn = (VOIDPTRPTRFUNC)((*fn)());
	}
}

#ifdef RECURSIVE
static void
repl()
{
	TEL_OBJ expr, rslt;

	while(1)
	{
		fprintf(stdout, "> "); // fflush(stdout);
		expr = tel_read(stdInputStream);
		rslt = tel_eval(topEnvironment, expr);
		if (! isVoid(rslt))
		{
			tel_print(stdout, rslt, PRINT_WRITE);
			fprintf(stdout, "\n");
		}
	}
}
#endif


extern VOIDPTRPTRFUNC CP_eval();

static long tStart, tEnd;

VOIDPTRPTRFUNC
CP_repl()
{
	TEL_OBJ expr;

	fprintf(stdout, "> "); // fflush(stdout);
	expr = tel_read(stdInputStream);

	tStart = millisecondTime();

	PUSH( topEnvironment );
	PUSH( expr );
	PUSH_CP( (VOIDPTRPTRFUNC)CP_repl1 );
	return (VOIDPTRPTRFUNC)CP_eval;
}

VOIDPTRPTRFUNC
CP_repl1()
{
	TEL_OBJ rslt = POP();

	tEnd = millisecondTime();
	fprintf(stdout, "t = %dms\n", (int)(tEnd - tStart));

	if (! isVoid(rslt))
	{
		print(stdout, rslt, PRINT_WRITE);
		fprintf(stdout, "\n");
	}
	return (VOIDPTRPTRFUNC)CP_repl;
}


int
main()
{
	init_system();

	//Testing
	tel_selftest();

	//Welcone message
	fprintf(stderr, "Welcome to Telvin\n");


	if (setjmp(getMeBackToRepl))
	{
		DBG_PRINT((stderr, "back in REPL.\n"));
	}
	else
	{
		DBG_PRINT(("after setjmp.\n"));
	}
#ifdef RECURSIVE
	repl();
#else
	trampoline((VOIDPTRPTRFUNC)CP_repl);
#endif

	exit(0);
	return 0;
}
