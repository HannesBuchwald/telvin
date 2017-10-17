/*
 * gloal.h
 *
 * Project:      Telvin Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    17.10.2017
 */



#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK_STACK

#define INIT_EV_STACK_S         256
#define INIT_RET_STACK_S       256

#ifdef __BORLANDC__
# define NO_INLINE_FUNCTIONS
# define INLINE /* nothing */
#else
# define INLINE inline
#endif

typedef int CBOOL;
#define C_TRUE      1
#define C_FALSE     0

typedef int TEL_CHAR;
typedef union tel_object *TEL_OBJ;
typedef TEL_OBJ (*TEL_OBJFUNC)();
// typedef int (*INTFUNC)();
typedef void (*VOIDFUNC)();
typedef void *(*VOIDPTRFUNC)();
typedef VOIDPTRFUNC (*VOIDPTRPTRFUNC)();

enum tel_tag_enum
{
#   define T_POINTER         0
    T_SMALLINTEGER = 1,
    T_FULLINTEGER = 2,
    T_FLOAT,
    T_SYMBOL,
    T_STRING,
    T_CONS,
    T_NIL,
    T_TRUE,
    T_FALSE,
    T_EOF,
    T_VOID,
    T_BUILTINFUNC,
    T_BUILTINSYNTAX,
    T_USERDEFINEDFUNC,
    T_ENVIRONMENT,
    MAX_T
};

typedef enum tel_tag_enum tel_tag;

struct tel_any
{
	tel_tag tag;
};

struct tel_integer
{
	tel_tag tag;
	long iVal;
};

struct tel_float
{
	tel_tag tag;
	double fVal;
};

struct tel_builtinFunc
{
	tel_tag tag;
	VOIDFUNC code;
};

struct tel_builtinSyntax
{
	tel_tag tag;
#ifdef RECURSIVE
	VOIDFUNC code;
#else
	VOIDPTRPTRFUNC cpCode;
#endif
};

struct tel_userDefinedFunc
{
	tel_tag tag;
	TEL_OBJ argList;
	TEL_OBJ bodyList;
};

struct tel_special
{
	tel_tag tag;
};

struct tel_symbol
{
	tel_tag tag;
	char chars[1];
};

struct tel_string
{
	tel_tag tag;
	char chars[1];
};

struct tel_string10
{
	tel_tag tag;
	char chars[10];
};

struct tel_cons
{
	tel_tag tag;
	TEL_OBJ car;
	TEL_OBJ cdr;
};

struct tableEntry
{
	TEL_OBJ key;
	TEL_OBJ value;
};

struct tel_environment
{
	tel_tag tag;
	TEL_OBJ parent;
	unsigned int numSlotsInUse;
	unsigned int numSlotsAllocated;
	struct tableEntry *entries;
};

union tel_object
{
	struct tel_any tel_any;
	struct tel_integer tel_integer;
	struct tel_float tel_float;
	struct tel_special tel_special;
	struct tel_symbol tel_symbol;
	struct tel_string tel_string;
	struct tel_string10 tel_string10;
	struct tel_cons tel_cons;
	struct tel_builtinFunc tel_builtinFunc;
	struct tel_builtinSyntax tel_builtinSyntax;
	struct tel_userDefinedFunc tel_userDefinedFunc;
	struct tel_environment tel_environment;
};

enum streamType
{
    FILESTREAM,
    STRINGSTREAM
};

struct tel_stream_struct
{
	enum streamType type;
	FILE *fileStream;
	char *theString;
	int indexInString;
	int peekChar;
};
typedef struct tel_stream_struct *tel_stream;


#define ERROR(msg) fprintf(stderr, "%s:%d check failed: %s", \
		__FILE__, __LINE__, msg);\
		abort();

#define CHECK(cond, msg) if (! (cond) ) { \
		fprintf(stderr, "%s:%d check failed: %s", \
				__FILE__, __LINE__, msg); \
		abort(); \
	    }

#define ASSERT_SYMBOL(obj) \
    CHECK(isSymbol(obj), "not a symbol")

#define ASSERT_CONS(obj) \
	CHECK(isCons(obj), "not a cons")

#define ASSERT_T(obj,t) \
	CHECK(hasTag(obj,t), "unexpected tag")


//
// wellknown objects
//
extern TEL_OBJ TEL_NIL;
extern TEL_OBJ TEL_TRUE;
extern TEL_OBJ TEL_FALSE;
extern TEL_OBJ TEL_VOID;
extern TEL_OBJ TEL_EOF;
extern TEL_OBJ TEL_quoteSymbol;

//
// globals
//
TEL_OBJ topEnvironment;
extern CBOOL trace_flag;

extern void tel_error(char *, TEL_OBJ);

//
// macros / inline functions
//
#define signOf(l) ((l) < 0)

#define isTaggedAsPOINTER(obj)  \
    ((((long)obj) & T_INTEGER) == 0)

#define isTaggedAsSMALLINTEGER(obj)  \
    ((((long)obj) & T_SMALLINTEGER) != 0)


static inline tel_tag
tagOf(TEL_OBJ obj)
{
	return (isTaggedAsSMALLINTEGER(obj) ? T_SMALLINTEGER : obj->tel_any.tag);
}

static inline CBOOL
hasTag(TEL_OBJ obj, tel_tag whichTag)
{
	return (tagOf(obj) == whichTag);
}

static inline CBOOL
isSmallInteger(TEL_OBJ obj)
{
	return hasTag(obj, T_SMALLINTEGER);
}

static inline CBOOL
isFullInteger(TEL_OBJ obj)
{
	return hasTag(obj, T_FULLINTEGER);
}

static inline CBOOL
isInteger(TEL_OBJ obj)
{
	return isSmallInteger(obj) || isFullInteger(obj);
}

static inline CBOOL
isFloat(TEL_OBJ obj)
{
	return hasTag(obj, T_FLOAT);
}

static inline CBOOL
isNil(TEL_OBJ obj)
{
	return obj == TEL_NIL;      // hasTag(obj, T_NIL);
}

static inline CBOOL
isVoid(TEL_OBJ obj)
{
	return obj == TEL_VOID;     // hasTag(obj, T_VOID);
}

static inline CBOOL
isTrue(TEL_OBJ obj)
{
	return obj == TEL_TRUE;     // hasTag(obj, T_TRUE);
}

static inline CBOOL
isFalse(TEL_OBJ obj)
{
	return obj == TEL_FALSE;    // hasTag(obj, T_FALSE);
}

static inline CBOOL
isSymbol(TEL_OBJ obj)
{
	return hasTag(obj, T_SYMBOL);
}

static inline CBOOL
isString(TEL_OBJ obj)
{
	return hasTag(obj, T_STRING);
}

static inline CBOOL
isCons(TEL_OBJ obj)
{
	return hasTag(obj, T_CONS);
}

static inline long
fullIntegerValue(TEL_OBJ obj)
{
	CHECK(isFullInteger(obj), "not a full integer");
	return obj->tel_integer.iVal;
}

static inline long
smallIntegerValue(TEL_OBJ obj)
{
	CHECK(isSmallInteger(obj), "not a small integer");
	return ((long)obj >> 1);
}

static inline long
integerValue(TEL_OBJ obj)
{
	return ( isSmallInteger(obj)
	         ? smallIntegerValue(obj)
	         : (isFullInteger(obj)
	            ? fullIntegerValue(obj)
	            : (tel_error("integer expected", NULL), 0)));
}

static inline TEL_OBJ
car(TEL_OBJ obj)
{
	CHECK(isCons(obj), "not a cons cell");
	return (obj->tel_cons.car);
}

static inline TEL_OBJ
cdr(TEL_OBJ obj)
{
	CHECK(isCons(obj), "not a cons cell");
	return (obj->tel_cons.cdr);
}


#ifdef RECURSIVE
extern TEL_OBJ new_builtinSyntax(VOIDFUNC);
#else
//extern TEL_OBJ new_builtinSyntax(VOIDPTRPTRFUNC);
#endif

extern void tel_selftest(void);
extern void fatal(char *msg, char *fileName, int lineNr);
//extern long a2l(char *);

extern void trampoline(VOIDPTRPTRFUNC fn);

//
// stack_______________________________________________________________________________________________________________
//
TEL_OBJ *Input_Stack;
TEL_OBJ *Input_Stack_P;
TEL_OBJ *Input_Stack_Top;
extern void append_eval_stack();

static inline void
PUSH(TEL_OBJ val)
{
	*Input_Stack_P++ = val;
	if (Input_Stack_P >= Input_Stack_Top)
	{
		append_eval_stack();
	}
}

static inline TEL_OBJ
POP()
{
#ifdef CHECK_STACK
	if (Input_Stack_P <= Input_Stack)
	{
		ERROR("stack underflow");
	}
#endif
	return *--Input_Stack_P;
}

// return the n'th stack slot;
// index is 0 for TOP-of-stack,
// 1 for NEXT-on-stack,
// 2 for THIRD-on-stack...
//
static inline TEL_OBJ
STACK_SLOT(int index)
{
	return Input_Stack_P[-index - 1];
}

//
// drop n stack elements
//
static inline void
DROP(int n)
{
	Input_Stack_P -= n;
#ifdef CHECK_STACK
	if (Input_Stack_P < Input_Stack)
	{
		ERROR("stack underflow");
	}
#endif
}

//
// continuation stack
//
VOIDPTRPTRFUNC *Return_Stack;
VOIDPTRPTRFUNC *Return_Stack_P;
VOIDPTRPTRFUNC *Return_Stack_Top;
extern void append_return_stack();

static inline void
PUSH_CP(VOIDPTRPTRFUNC val)
{
	*Return_Stack_P++ = val;
	if (Return_Stack_P >= Return_Stack_Top)
	{
		append_return_stack();
	}
}

static inline VOIDPTRPTRFUNC
POP_CP()
{
#ifdef CHECK_STACK
	if (Return_Stack_P <= Return_Stack)
	{
		ERROR("continuation stack underflow");
	}
#endif
	return *--Return_Stack_P;
}

#endif /* GLOBAL_H */
