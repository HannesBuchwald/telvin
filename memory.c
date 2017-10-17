/*
 * memory.c
 *
 * Project:      Telvin Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    17.10.2017
 */


//#include "global.h"

#include "headers/memory.h"

#define USE_REAL_HASH


static TEL_OBJ *symbl_list = NULL;
static int symbl_list_size = 0;
static int symbl_known_size = 0;

static INLINE long
H(char *cp)
{
	unsigned long hVal = 0;
	char c;

#ifdef USE_REAL_HASH
	while ((c = *cp++) != '\0')
	{
		hVal = (hVal * 63) + (c & 0xFF);
	}
#endif
	return (hVal);
}

TEL_OBJ
create_user_def_func(TEL_OBJ argList, TEL_OBJ bodyList)
{
	TEL_OBJ new_obj = (TEL_OBJ)malloc(sizeof(struct tel_userDefinedFunc));

	new_obj->tel_userDefinedFunc.tag = T_USERDEFINEDFUNC;
	new_obj->tel_userDefinedFunc.argList = argList;
	new_obj->tel_userDefinedFunc.bodyList = bodyList;
	return new_obj;
}

TEL_OBJ
create_built_func(VOIDFUNC funcPtr)
{
	TEL_OBJ new_obj = (TEL_OBJ)malloc(sizeof(struct tel_builtinFunc));

	new_obj->tel_builtinFunc.tag = T_BUILTINFUNC;
	new_obj->tel_builtinFunc.code = funcPtr;
	return new_obj;
}

#ifdef RECURSIVE

TEL_OBJ
new_builtinSyntax(VOIDFUNC syntaxPtr)
{
	TEL_OBJ new_obj = (TEL_OBJ)malloc(sizeof(struct tel_builtinSyntax));

	new_obj->tel_builtinSyntax.tag = T_BUILTINSYNTAX;
	new_obj->tel_builtinSyntax.code = syntaxPtr;
	return new_obj;
}

#else

TEL_OBJ
create_built_syntax(VOIDPTRPTRFUNC syntaxPtr)
{
	TEL_OBJ new_obj = (TEL_OBJ)malloc(sizeof(struct tel_builtinSyntax));

	new_obj->tel_builtinSyntax.tag = T_BUILTINSYNTAX;
	new_obj->tel_builtinSyntax.cpCode = syntaxPtr;
	return new_obj;
}

#endif

TEL_OBJ
new_int(long value)
{
	TEL_OBJ new_obj = (TEL_OBJ)malloc(sizeof(struct tel_integer));

	new_obj->tel_integer.tag = T_FULLINTEGER;
	new_obj->tel_integer.iVal = value;
	return new_obj;
}

#ifdef REMEMBER_WELLKNOWN_INTEGERS

static TEL_OBJ wellKnownIntegers[200 + 1];

void
initializeWellKnownIntegers()
{
	for (i = -100; i <= 100; i++)
	{
		wellKnownIntegers[i + 100] = really_new_integer(i);
	}
}

TEL_OBJ
new_integer(long value)
{
	if ((value >= -100) && (value <= 100))
	{
		return wellKnownIntegers[i + 100];
	}
	return really_new_integer(value);
}

#else

TEL_OBJ
create_new_int(long value)
{
	// (iVal < 0) is sign bit
	// ((iVal << 1) < 0) is next after sign bit

	if ((value < 0) == ((value << 1) < 0))
	{
		return (TEL_OBJ)((value << 1) | T_SMALLINTEGER);
	}
	return new_int(value);
}

#endif

TEL_OBJ
create_cons(TEL_OBJ theCar, TEL_OBJ theCdr)
{
	TEL_OBJ new_obj = (TEL_OBJ)malloc(sizeof(struct tel_cons));

	new_obj->tel_cons.tag = T_CONS;
	new_obj->tel_cons.car = theCar;
	new_obj->tel_cons.cdr = theCdr;
	return new_obj;
}

static TEL_OBJ
new_symbol(char *chars)   //really_a_new_symbol
{
	unsigned int len = (unsigned int) strlen(chars);

	unsigned int nBytes = (unsigned int)sizeof(struct tel_symbol)
	                      - 1 /* eins zuviel in struct*/
	                      + len
	                      + 1 /*0-byte*/;

	TEL_OBJ new_obj = (TEL_OBJ)malloc(nBytes);

	new_obj->tel_symbol.tag = T_SYMBOL;
	strcpy(new_obj->tel_symbol.chars, chars);
	new_obj->tel_symbol.chars[len] = '\0';
	return new_obj;
}



void
init_symbol_table()
{
	symbl_list = (TEL_OBJ *)malloc(sizeof(TEL_OBJ) * INITIAL_SYMBOLTABLE_SIZE);
	symbl_list_size = INITIAL_SYMBOLTABLE_SIZE;
	symbl_known_size = 0;
	memset(symbl_list, 0, (sizeof(TEL_OBJ) * INITIAL_SYMBOLTABLE_SIZE));
}

void
append_symbol_table()
{
	if (symbl_known_size > (symbl_list_size * 3 / 4))
	{
		int newSize = (symbl_list_size * 2) + 1;
		TEL_OBJ *newTable = (TEL_OBJ *)malloc(sizeof(TEL_OBJ) * newSize);
		int i;

		memset(newTable, 0, (sizeof(TEL_OBJ) * newSize));

		for (i = 0; i < symbl_list_size; i++)
		{
			TEL_OBJ slotValue = symbl_list[i];

			if (slotValue != NULL)
			{
				int newIdx, newIdx0;

				newIdx = newIdx0 = H(slotValue->tel_symbol.chars) % newSize;
				while(1)
				{
					if (newTable[newIdx] == NULL)
					{
						newTable[newIdx] = slotValue;
						break;
					}
					newIdx++;
					if (newIdx == newSize)
					{
						newIdx = 0;
					}
					if (newIdx == newIdx0)
					{
						// cannot happen
						ERROR("new table full");
					}
				}
			}
		}
		free(symbl_list);
		symbl_list = newTable;
		symbl_list_size = newSize;

	}
}

static void
save_symbol(TEL_OBJ theSymbolToRemember)   //remember_symbol
{
	int i, j;

	ASSERT_SYMBOL(theSymbolToRemember);
	i = j = H(theSymbolToRemember->tel_symbol.chars) % symbl_list_size;
	if(i==0)
	{
		return;
	}

	while(1)
	{
		TEL_OBJ slotValue = symbl_list[j];

		if (slotValue == NULL)
		{
			symbl_list[j] = theSymbolToRemember;
			symbl_known_size++;
			append_symbol_table();
			return;
		}
		j++;
		if (j == symbl_list_size)
		{
			j = 0;
		}
		if (j == i)
		{
			// cannot happen
			ERROR("symboltable full");
		}
	}

	if (symbl_known_size == symbl_list_size)
	{
		if (symbl_list_size == 0)
		{
			symbl_list = (TEL_OBJ *)malloc(sizeof(TEL_OBJ) * INITIAL_SYMBOLTABLE_SIZE);
			symbl_list_size = INITIAL_SYMBOLTABLE_SIZE;
		}
		else
		{
			int newSize = symbl_list_size * 2;
			symbl_list = (TEL_OBJ *)realloc(symbl_list, sizeof(TEL_OBJ) * newSize);
			symbl_list_size = newSize;
		}
	}
	symbl_list[symbl_known_size++] = theSymbolToRemember;
}

static TEL_OBJ
get_symbolOrNULL(char *chars)
{
	int  i, j;

	j = i = H(chars) % symbl_list_size;
	if (j < 0)
	{
		return NULL;
	}
	while(1)
	{
		TEL_OBJ existingAtI = symbl_list[j];

		if (existingAtI == NULL)
		{
			return NULL;
		}
		if (strcmp(existingAtI->tel_symbol.chars, chars) == 0)
		{
			return existingAtI;
		}
		j++;
		if (j == symbl_list_size)
		{
			j = 0;
		}
		if (j == i)
		{
			return NULL;
		}
	}
	return NULL;
}

TEL_OBJ
create_symbol(char *chars)
{
	TEL_OBJ exist = get_symbolOrNULL(chars);

	if (exist == NULL)
	{
		TEL_OBJ newSym = new_symbol(chars);

		save_symbol(newSym);
		return newSym;
	}
	return exist;
}

TEL_OBJ
create_string(char *chars)
{
	unsigned int len = (unsigned int) strlen(chars);

	unsigned int bytes = (unsigned int)(sizeof(struct tel_string)) + len ;

	TEL_OBJ new_obj = (TEL_OBJ)malloc(bytes);

	new_obj->tel_string.tag = T_STRING;
	strcpy(new_obj->tel_string.chars, chars);
	new_obj->tel_string.chars[len] = '\0';
	return new_obj;
}

static TEL_OBJ
create_singleton(tel_tag tag)   //new_singleton
{
	static TEL_OBJ singleton[MAX_T];

	if (singleton[tag] == NULL)
	{
		TEL_OBJ new_obj = (TEL_OBJ)malloc(sizeof(struct tel_special));

		new_obj->tel_integer.tag = tag;
		singleton[tag] = new_obj;
	}
	return singleton[tag];
}

TEL_OBJ
create_eof()
{
	return create_singleton(T_EOF);
}

TEL_OBJ
create_true()
{
	return create_singleton(T_TRUE);
}

TEL_OBJ
create_false()
{
	return create_singleton(T_FALSE);
}

TEL_OBJ
create_void()
{
	return create_singleton(T_VOID);
}

TEL_OBJ
create_nil()
{
	return create_singleton(T_NIL);
}
