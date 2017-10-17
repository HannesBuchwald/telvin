/*
 * reader.c
 *
 * Project:      Telvin Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    17.10.2017
 */



//#include "global.h"

#include "headers/reader.h"

#define EOF_CHAR    ((TEL_CHAR)-1)
#define INITIAL_BUFFER_SIZE 32

static
tel_stream
new_stringStream(char *inString)
{
	tel_stream s = (tel_stream)malloc(sizeof(struct tel_stream_struct));
	s->type = STRINGSTREAM;
	s->theString = (char *)malloc( strlen(inString) + 1);
	strcpy(s->theString, inString);
	s->indexInString = 0;
	return s;
}

tel_stream
new_fileStream(FILE *inFile)
{
	tel_stream s = (tel_stream)malloc(sizeof(struct tel_stream_struct));
	s->type = FILESTREAM;
	s->fileStream = inFile;
	s->peekChar = 0;
	return s;
}

typedef struct bufferStruct
{
	char *memory;
	int bufferSize;
	int fillCount;
} buffer;

static inline void
allocBuffer(buffer *b, int initialSize)
{
	b->memory = malloc((size_t)initialSize);
	b->bufferSize = initialSize;
	b->fillCount = 0;
}

static inline void
freeBuffer(buffer *b)
{
	if(b->memory!=NULL)
	{
		free(b->memory);
		b->memory = NULL;
		b->bufferSize = b->fillCount = 0;
	}
}

static inline void
growBuffer(buffer *b)
{
	int newSize = b->bufferSize * 2;
	char *tmp = NULL;
	tmp = realloc(b->memory, (size_t)newSize);
	if(tmp!=NULL)
	{
		b->memory = tmp;
		b->bufferSize = newSize;
	}


}

static inline void
putBuffer(buffer *b, TEL_CHAR ch)
{
	if ((b->fillCount + 1) == b->bufferSize)
	{
		growBuffer(b);
	}
	b->memory[b->fillCount++] = (char)ch;
	b->memory[b->fillCount] = '\0';
}

static CBOOL
isSeparator(TEL_CHAR aChar)
{
	switch (aChar)
	{
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			return 1;
		default:
			return 0;
	}
}

static CBOOL
allDigits(char *cp)
{
	char c;

	if (*cp == '\0')
	{
		return 0;
	}

	while ((c = *cp++) != '\0')
	{
		if ((c < '0') || (c > '9'))
		{
			return 0;
		}
	}
	return 1;
}

static TEL_CHAR
readCharacter(tel_stream inStream)
{
	switch (inStream->type)
	{
		case FILESTREAM:
		{
			TEL_CHAR nextChar;

			if (inStream->peekChar != 0)
			{
				nextChar = inStream->peekChar;
				inStream->peekChar = 0;
				return nextChar;
			}

			nextChar = fgetc(inStream->fileStream);
			if (nextChar < 0)
			{
				return EOF_CHAR;
			}
			return nextChar;
		}

		case STRINGSTREAM:
		{
			int index = inStream->indexInString;
			TEL_CHAR next_ch = inStream->theString[index];
			if (next_ch == '\0')
			{
				return EOF_CHAR;
			}
			inStream->indexInString = ++index;
			return next_ch;
		}

		default:
			ERROR("unhandled stream type");
	}
	// only to make the compiler happy
	return 0;
}

static void
unreadCharacter(tel_stream inStream, TEL_CHAR charToUnread)
{
	switch (inStream->type)
	{
		case FILESTREAM:
			CHECK(inStream->peekChar == 0, "can only unread one char");

			inStream->peekChar = charToUnread;
			return;

		case STRINGSTREAM:
			inStream->indexInString--;
			return;

		default:
			ERROR("unhandled stream type");
	}
}

static TEL_CHAR
skipSeparators(tel_stream inStream)
{
	TEL_CHAR next_ch;

	do
	{
		next_ch = readCharacter(inStream);
		if (next_ch == EOF_CHAR)
		{
			return EOF_CHAR;
		}
	}
	while (isSeparator(next_ch));
	return next_ch;
}

long
a2l(char *cp)
{
	long value = 0;
	char c;

	while ((c = *cp++) != '\0')
	{
		value = value * 10 + (c - '0');
	}
	return value;
}


TEL_OBJ
tel_readFromCString(char *input)
{
	return tel_read( new_stringStream(input) );
}

static TEL_OBJ
tel_readList(tel_stream inStream)
{
	TEL_CHAR next_ch;
	TEL_OBJ theCar[10], theCdr;

	int i=0;
   while(C_TRUE){
	   next_ch = skipSeparators(inStream);
		if (next_ch == END_LIST)
		{
			theCdr=TEL_NIL;
			for(i--;i>=0;i--)
			{
				theCdr= create_cons(theCar[i], theCdr);
			}
		return theCdr;
		}

		unreadCharacter(inStream, next_ch);
		theCar[i++] = tel_read(inStream);
	}
}

TEL_OBJ
tel_readString(tel_stream inStream)
{
	buffer b;
	TEL_CHAR next_ch;

	allocBuffer(&b, INITIAL_BUFFER_SIZE);

	while(C_TRUE)
	{
		next_ch = readCharacter(inStream);
		if (next_ch == EOF_CHAR)
		{
			fprintf(stderr, "unterminated string");
			freeBuffer(&b);
			return TEL_EOF;
		}

		if (next_ch == QUOTE2_CH)
		{
			TEL_OBJ stringObj = create_string(b.memory);

			freeBuffer(&b);
			return stringObj;
		}
		if (next_ch == SLASH_CH)
		{
			TEL_CHAR escapedChar;

			escapedChar = readCharacter(inStream);
			switch (escapedChar)
			{
				case 'n':
					next_ch = '\n';
					break;
				default:
					break;
			}
		}
		putBuffer(&b, next_ch);
	}
}

TEL_OBJ tel_def(char * in_token)
{
	TEL_OBJ result;
	if (strcmp(in_token, TRUE_CH	) == 0)
	{

	result= TEL_TRUE;
	}
	if (strcmp(in_token, FALSE_CH) == 0)
	{

		result= TEL_FALSE;
	}
	if (strcmp(in_token, VOID_CH) == 0)
	{

		result= TEL_VOID;
	}
	return result;
}

TEL_OBJ tel_quoted(tel_stream inStream)
{

	TEL_OBJ quotedExpr = tel_read(inStream);
	return create_cons(TEL_quoteSymbol, create_cons(quotedExpr, TEL_NIL));
}

TEL_OBJ
tel_read(tel_stream inStream)
{
	buffer b;
	TEL_CHAR next_ch;
	char *inputToken;

	next_ch = skipSeparators(inStream);
	if (next_ch == EOF_CHAR)
	{
		return TEL_EOF;
	}

	else if (next_ch == START_LIST)
	{
		return tel_readList(inStream);
	}

	else if (next_ch == QUOTE2_CH)
	{
		return tel_readString(inStream);
	}
	else if (next_ch == QUOTE1_CH)
	{
		TEL_OBJ quotedExpr;

		quotedExpr = tel_read(inStream);
		return create_cons(TEL_quoteSymbol, create_cons(quotedExpr, TEL_NIL));
	}

	allocBuffer(&b, INITIAL_BUFFER_SIZE);

	while(C_TRUE)
	{
		if (next_ch == EOF_CHAR)
		{
			break;
		}
		if (isSeparator(next_ch)
		        || (next_ch == START_LIST)
		        || (next_ch == END_LIST))
		{
			break;
		}
		putBuffer(&b, next_ch);
		next_ch = readCharacter(inStream);
	}
	unreadCharacter(inStream, next_ch);

	if (allDigits(b.memory))
	{
		long iVal = a2l(b.memory);
		freeBuffer(&b);
		return new_integer(iVal);
	}
	inputToken = b.memory;

	if (inputToken[0] == MINUS_CH)
	{
		if (allDigits(inputToken + 1))
		{
			long iVal = a2l(inputToken + 1);

			freeBuffer(&b);
			return new_integer(-iVal);
		}
	}

	if (inputToken[0] == SHARP_CH)
	{
		TEL_OBJ result=tel_def(inputToken);
		freeBuffer(&b);
		return result;
	}
	if (strcmp(inputToken, NULL_CH) == 0)
	{
		freeBuffer(&b);
		return TEL_NIL;
	}


	TEL_OBJ symbolObj;
	symbolObj = create_symbol(inputToken);
	freeBuffer(&b);
	return symbolObj;

}
