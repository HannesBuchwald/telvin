# telvin

This is a Scheme interpreter which is based on the lecture of Mr. Gittinger at the Stuttgart Media University in the Master course of "Design und Implementierung fortgeschrittener Programmiersprachen".

Programming language is C

Description:
Telvin's is an interpreter language project. It has some basic mathematical functions like +, -, * and /, also supports variable defining, creating functions, operator if etc.
Variable types are: integer, boolean, symbol, string, void and nil. All variable types are cast from string, so we have huge of casters: string to int etc.

Telvin has specific syntax.

1. Variable defining:
```
(define a 12)
```
This means that we have variable named *a*, it is an integer variable
```
(define a “some string”)
```
We have string variable named *a* etc..

2. Arithmetic operations:
All operations is done from left to right.
```
(+ 1 2)
```
Shows answer 1+2, so 3.

Telvin has some specific feature, so you can write something like this:
```
(+ 1 2 3)
```
Telvin will add all numbers which are given, so answer is 1+2+3, 6. (- 1 2 3), 1 – 2 – 3 , answer is -4. (/ 4 2), 4/2=2

3. Defining our function:
```
(define func
(lambda (a b c)
(+ a b c ) ) )
```
Now we have function named <func> which accepts three parameters and adds them.

4. Using defined function.
```
(func 1 2 3)
```
Answer will 6.

5. Recursive function calls.
Telvin also supports function recursive calls, here is an example of 1+2+...+n
```
(define sum
(lambda (n)
(if (= n 0) 0 (+ n (sum (- n 1))))))
```
Usage:
```
(sum 3)
```
Answer is 6.

### Project structure:
All operations are saved in global structure object **topEnvironment**.

In project used *stack* mechanism, when function must be called all variables are pushed to stack and function takes variables using pop from stack.

In Telvin we have two streams filestream and stringstream. Filestream uses with stdin for input and stringstream uses for string lines.

At first all global values are *NULL* or *0*.

In main function we call **init_system()** function, this function initializes all Telvin variables environment. For variable creation we use *create_nil*, *create_true* etc (in function *init_system()*), that uses **create_singleton()** function, which returns needed object type.

After it is called  **init_symbol_table()** function, here we allocate memory for *symbl_list* global variable (This variable contains all symbols during execution Telvin).

As we say that we have stacks in next step initializes our stacks. We just allocate memory for variables for future use. We have two stacks first one is *return_stack* second one is *eval_stack*. With first one we will return our variables after function execution. With second one we will pass values to functions.

We are allocating environment, and after it is one important function **initializeBuiltinFunctions()**. In this function we will define our mathematical functions and that functions through which we will define specific functions. In *eval.c* file we have all native functions there are separated.

#### Mathematical functions.

- +/plus
- -/minus
- \*
- /

#### Basic functions.
- Internal use
  - *cons*
  - *cdr*
  - *car*
- *eq?* Checks equation of two objects
- *=* assign operator,
- *display* displays message to console
#### Built in syntax.
- *define* used for defining function in Telvin
- *if* checks condition
- *quote*  for internal use with *car* and *cdr*
- *lambda* used for create function with *define*.

Here is used **create_built_func()**, it accepts function pointer and returns *TEL_OBJ*, which is containing tag *T_BUILTINFUNC* and function name as funcptr.

**Add_binding()** function used for add function object to top environment. It also takes argument function name as symbol to match symbol and function.
**add_built_syntax()** function works like **create_built_func()** function, they differ only by returning object tag, in **add_built_syntax()** tag is *T_BUILTINSYNTAX*.

init_symbol_table()

initializeBuiltinFunctions()
