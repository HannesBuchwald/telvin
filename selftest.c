/*
 * selftest.c
 *
 * Project:      Telvin Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    17.10.2017
 */





//#include "global.h"
#include "headers/main.h"
// #define PRINT_TIMES

void
tel_selftest()
{

	/***** test case object definition *****/

	TEL_OBJ rslt, l2, l3,
					sym_abcde1, sym_abcde2, sym_abcde3,
					sym_bbb1, sym_bbb2, symx1, symx2;


	fprintf(stderr, "sizeof(long) == %d\n", (int)(sizeof(long)));
	fprintf(stderr, "\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "**** Start (UnitTest) ****\n");



	/***** string to integer conversion Test *****/

	// positive test
	CHECK( a2l("0") == 0, "a2l(0) error");
	CHECK( a2l("1") == 1, "a2l(1) error");
	CHECK( a2l("123456") == 123456, "a2l(123456) error");
	CHECK( a2l("999999") == 999999, "a2l(999999) error");
	if (sizeof(long) > 4) {
		CHECK( a2l("2432902008176640000") == 2432902008176640000, "a2l(longnum) error");
	}

	// negative test
	CHECK( a2l("0") != 1, "a2l error");
	CHECK( a2l("1") != 0, "a2l error");
	CHECK( a2l("123456") != 12345644, "a2l error");
	CHECK( a2l("999999") != 9999999, "a2l error");
	CHECK( a2l("2432902008176640000") != 243290200817664000, "a2l error");


	fprintf(stderr, "(Done) string to integer test \n");




	/***** TAG and Check Test *****/


	// create tag - positive test
	CHECK(create_nil() == TEL_NIL, "nil check failed")
	CHECK(create_true() == TEL_TRUE, "true check failed")
	CHECK(create_false() == TEL_FALSE, "false check failed")
	CHECK(create_void() == TEL_VOID, "void check failed")
	CHECK(create_eof() == TEL_EOF, "eof check failed")

	// create tag - negative test
	CHECK(create_nil() != TEL_TRUE, "nil check failed")
	CHECK(create_true() != TEL_FALSE, "true check failed")
	CHECK(create_false() != TEL_TRUE, "false check failed")
	CHECK(create_void() != TEL_NIL, "void check failed")
	CHECK(create_eof() != TEL_VOID, "eof check failed")



	// positive test
	CHECK(tagOf(TEL_NIL) == T_NIL, "nil tag check failed")
	CHECK(tagOf(TEL_TRUE) == T_TRUE, "true tag check failed")
	CHECK(tagOf(TEL_FALSE) == T_FALSE, "false tag check failed")
	CHECK(tagOf(TEL_EOF) == T_EOF, "eof tag check failed")
	CHECK(tagOf(TEL_VOID) == T_VOID, "void tag check failed")

  // negative test
	CHECK(tagOf(TEL_NIL) != T_EOF, "nil tag check failed")
	CHECK(tagOf(TEL_TRUE) != T_FALSE, "true tag check failed")
	CHECK(tagOf(TEL_FALSE) != T_TRUE, "false tag check failed")
	CHECK(tagOf(TEL_EOF) != T_VOID, "eof tag check failed")
	CHECK(tagOf(TEL_VOID) != T_EOF, "void tag check failed")



	// positive test
	CHECK(hasTag(TEL_NIL, T_NIL), "nil tag check failed")
	CHECK(hasTag(TEL_TRUE, T_TRUE), "true tag check failed")
	CHECK(hasTag(TEL_FALSE, T_FALSE), "false tag check failed")
	CHECK(hasTag(TEL_EOF, T_EOF), "eof tag check failed")
	CHECK(hasTag(TEL_VOID, T_VOID), "void tag check failed")

	CHECK(!hasTag(TEL_NIL, T_TRUE), "nil tag check failed")
	CHECK(!hasTag(TEL_TRUE, T_FALSE), "true tag check failed")
	CHECK(!hasTag(TEL_FALSE, T_TRUE), "false tag check failed")
	CHECK(!hasTag(TEL_EOF, T_VOID), "eof tag check failed")
	CHECK(!hasTag(TEL_VOID, T_NIL), "void tag check failed")


	fprintf(stderr, "(Done) tag and checks test\n");






	/***** True/False TAG Check Test *****/

	// positive test
	CHECK(isTrue(TEL_TRUE), "true tag check failed")
	CHECK(isFalse(TEL_FALSE), "false tag check failed")
	CHECK(isNil(TEL_NIL), "nil tag check failed")

	// negative test
	CHECK(!isTrue(TEL_FALSE), "true tag check failed")
	CHECK(!isFalse(TEL_TRUE), "false tag check failed")
	CHECK(!isNil(TEL_VOID), "nil tag check failed")

	fprintf(stderr, "(Done) true/false tag test\n");





	/***** Reader Test *****/


	/***** Reader number check test *****/
	// empty
	rslt = tel_readFromCString("");
	CHECK(rslt != NULL, "empty read check failed (NULL result)")
	CHECK(rslt == TEL_EOF, "empty read check failed")


	// blank
	rslt = tel_readFromCString("    ");
	CHECK(rslt != NULL, "empty read check failed (NULL result)")
	CHECK(rslt == TEL_EOF, "empty read check failed")


	// number positive check
	rslt = tel_readFromCString("321");
	CHECK(rslt != NULL, "int check failed (NULL result)")
	CHECK(tagOf(rslt) == T_SMALLINTEGER, "int tag check failed")
	CHECK(isInteger(rslt), "int tag check failed")
	CHECK(integerValue(rslt) == 321, "int check failed")


	//number negative check
	CHECK(!isSymbol(rslt), "int tag check failed")
	CHECK(!isCons(rslt), "int tag check failed")
	CHECK(!isString(rslt), "int tag check failed")


	// bank + numbers
	rslt = tel_readFromCString("    123");
	CHECK(isInteger(rslt), "int tag check failed")
	CHECK(integerValue(rslt) == 123, "int check failed") // positive check
	CHECK(integerValue(rslt) != 321, "int check failed") // negative check


	// big number
	rslt = tel_readFromCString("99999999");
	CHECK(isInteger(rslt), "int tag check failed")
	CHECK(integerValue(rslt) == 99999999, "int check failed") // positive check
	CHECK(integerValue(rslt) == 99999999, "int check failed") // negative check


	// negative number
	rslt = tel_readFromCString("-99999999");
	CHECK(isInteger(rslt), "int tag check failed")
	CHECK(integerValue(rslt) == -99999999, "int check failed") // positive check
	CHECK(integerValue(rslt) != -99999998, "int check failed") // negative check


	fprintf(stderr, "(Done) reader - number check test\n");






	/***** Reader string check test *****/

	rslt = tel_readFromCString(" \"hello\" ");
	CHECK(tagOf(rslt) == T_STRING, "string tag check failed")
	CHECK(isString(rslt), "string tag check failed")
	CHECK( strcmp(rslt->tel_string.chars, "hello") == 0, "string check failed")

	rslt = tel_readFromCString(" \"hel\\nlo\" ");
	CHECK(isString(rslt), "string tag check failed")
	CHECK( strcmp(rslt->tel_string.chars, "hel\nlo") == 0, "string check failed")

	rslt = tel_readFromCString(" #t ");
	CHECK(rslt == TEL_TRUE, "true tag check failed")

	rslt = tel_readFromCString(" #f ");
	CHECK(rslt == TEL_FALSE, "false tag check failed")

	rslt = tel_readFromCString(" nil ");
	CHECK(rslt == TEL_NIL, "nil tag check failed")

	rslt = tel_readFromCString(" #void ");
	CHECK(rslt == TEL_VOID, "void tag check failed")


	fprintf(stderr, "(Done) reader - string check test\n");





	/***** Symbol TAG Test *****/

	rslt = tel_readFromCString(" abc ");
	CHECK(rslt->tel_any.tag == T_SYMBOL, "symbol tag check failed")
	CHECK(isSymbol(rslt), "symbol tag check failed")
	CHECK(strcmp(rslt->tel_symbol.chars, "abc") == 0, "symbol check failed")
	CHECK(strcmp(symbolValue(rslt), "abc") == 0, "symbol check failed")

	rslt = tel_readFromCString(" if ");
	CHECK(isSymbol(rslt), "symbol tag check failed")
	CHECK(strcmp(symbolValue(rslt), "if") == 0, "symbol check failed")

	rslt = tel_readFromCString(" define ");
	CHECK(isSymbol(rslt), "symbol tag check failed")
	CHECK(strcmp(symbolValue(rslt), "define") == 0, "symbol check failed")

	rslt = tel_readFromCString(" 1+ ");
	CHECK(isSymbol(rslt), "symbol tag check failed")
	CHECK(strcmp(symbolValue(rslt), "1+") == 0, "symbol check failed")

	rslt = tel_readFromCString(" + ");
	CHECK(isSymbol(rslt), "symbol tag check failed")
	CHECK(strcmp(symbolValue(rslt), "+") == 0, "symbol check failed")

	rslt = tel_readFromCString(" - ");
	CHECK(isSymbol(rslt), "symbol tag check failed")
	CHECK(strcmp(symbolValue(rslt), "-") == 0, "symbol check failed")

	rslt = tel_readFromCString(" --->---<--- ");
	CHECK(isSymbol(rslt), "symbol tag check failed")
	CHECK(strcmp(symbolValue(rslt), "--->---<---") == 0, "symbol check failed")


	fprintf(stderr, "(Done) reader - symbol table check test\n");





	/***** Compare same Tag Test *****/

	// same string
	sym_abcde1 = tel_readFromCString("abcde ");
	sym_abcde2 = tel_readFromCString("abcde ");
	CHECK(sym_abcde1 == sym_abcde2, "symbol identity check failed")

	// same string + blank
	sym_bbb1 = tel_readFromCString("   bbb ");
	sym_bbb2 = tel_readFromCString("bbb ");
	CHECK(sym_bbb1 == sym_bbb2, "symbol identity check failed")

	// same string
	sym_abcde3 = tel_readFromCString("abcde ");
	CHECK(sym_abcde3 == sym_abcde1, "symbol identity check failed")

	// different string negative test
	symx1 = tel_readFromCString("abcde ");
	symx2 = tel_readFromCString(" bbb ");
	CHECK(symx1 != symx2, "symbol identity check failed")


	fprintf(stderr, "(Done) reader - compare same tag test\n");




	/***** Read list Test *****/

	// empty list
	rslt = tel_readFromCString("()");
	CHECK(rslt == TEL_NIL, "empty list read check failed")

	// number list
	rslt = tel_readFromCString(" (123) ");
	CHECK(tagOf(rslt) == T_CONS, "empty list read check failed")
	CHECK(isCons(rslt), "empty list read check failed")
	CHECK(isInteger(rslt->tel_cons.car), "car list read check failed")
	CHECK(rslt->tel_cons.cdr == TEL_NIL, "cdr list read check failed")
	CHECK(integerValue(car(rslt)) == 123, "inval list read check failed")

	// variable list
	rslt = tel_readFromCString(" (a b c) ");
	CHECK(tagOf(rslt) == T_CONS, "empty list read check failed")
	CHECK(isCons(rslt), "empty list read check failed")
	CHECK(isSymbol(car(rslt)), "car list read check failed")
	CHECK(isCons(cdr(rslt)), "car list read check failed")
	CHECK(isSymbol(car(cdr(rslt))), "car list read check failed")
	CHECK(isCons(cdr(cdr(rslt))), "car list read check failed")
	CHECK(isSymbol(car(cdr(cdr(rslt)))), "car list read check failed")
	CHECK(isNil(cdr(cdr(cdr(rslt)))), "car list read check failed")

	// inner list
	rslt = tel_readFromCString(" (a (b (c))) ");
	CHECK(isCons(rslt), "empty list read check failed")
	CHECK(isSymbol(car(rslt)), "car list read check failed")
	CHECK(isCons(cdr(rslt)), " read check failed")
	CHECK(isNil(cdr(cdr(rslt))), " read check failed")
	l2 = car(cdr(rslt));
	CHECK(isSymbol(car(l2)), "read check failed")
	CHECK(isCons(cdr(l2)), " read check failed")
	l3 = car(cdr(l2));
	CHECK(isSymbol(car(l3)), "read check failed")


	fprintf(stderr, "(Done) reader - list test\n");




	/***** Environment Test *****/

	add_binding(topEnvironment, create_symbol("a"), new_integer(120));
	add_binding(topEnvironment, create_symbol("b"), new_integer(200));

	rslt = get_binding(topEnvironment,  create_symbol("a") );
	CHECK(isInteger(rslt), "getbinding error");
	CHECK(integerValue(rslt) == 120, "getbinding error");

	rslt = get_binding(topEnvironment,  create_symbol("b") );
	CHECK(isInteger(rslt), "getbinding error");
	CHECK(integerValue(rslt) == 200, "getbinding error");

	rslt = get_binding(topEnvironment,  create_symbol("c") );
	CHECK( rslt == NULL, "getbinding undefined error");

	{
		int i;
		for (i = 0; i < 10000; i++)
		{
			char name[32];
			TEL_OBJ k;
			snprintf(name, sizeof(name), "s%d", i);
			k = create_symbol(name);
			add_binding(topEnvironment, k, new_integer(i));
		}
	}

	rslt = get_binding(topEnvironment,  create_symbol("s500") );
	CHECK( integerValue(rslt) == 500, "binding error");

	fprintf(stderr, "(Done) environment test\n");



	fprintf(stderr, "**** Done (UnitTest) ****\n");







	fprintf(stderr, "\n");
	fprintf(stderr, "**** Start (PerformanceTest)**** \n");


	// create symbols
	{
		int i;
		char symName[32];
		TEL_OBJ sym1, sym2;
		long tStart, tEnd;

		tStart = millisecondTime();

		for (i = 0; i < 10000; i++)
		{
			snprintf(symName, sizeof(symName), "abc%d", i);
			// fprintf(stderr, "create symbol %s...\n", symName);
			create_symbol(symName);
		}
		tEnd = millisecondTime();

		fprintf(stderr, "time to add 10000 new symbols: %dms\n", (int)(tEnd - tStart));

		tStart = millisecondTime();
		for (i = 0; i < 10000; i++)
		{
			snprintf(symName, sizeof(symName), "abc%d", i);
			create_symbol(symName);
		}
		tEnd = millisecondTime();

		fprintf(stderr, "time to lookup 10000 existing symbols: %dms\n", (int)(tEnd - tStart));

		// check identity
		for (i = 0; i < 10000; i++)
		{
			snprintf(symName, sizeof(symName), "abc%d", i);
			sym1 = create_symbol(symName);
			sym2 = create_symbol(symName);
			CHECK(sym1 == sym2, "symbol table check");
		}
	}

	fprintf(stderr, "**** Done (PerformanceTest) ****\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "\n");


}
