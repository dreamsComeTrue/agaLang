# agaLang
Compiler for my staticaly-typed programming language written in C++

Current language features:
- hand-written lexer
- top down parser 
- Abstract Syntax Tree generation
- intermediate assembly representation / possibly using LLVM

--	Language specification (constantly evolving):

```
::identifier							import statement
<identifier> <param1> <param n...> : <statements> =		basic block/function definition
&expression: statements =			 		loop
?expression: basic_block_1 basic_block_n =			condition block
<, <=, >, >=, ==, !=						comparison operators
identifier := expression					assignment
= expression							return expression value
= break_label							(in loop) break/continue
,								statement separator (optional at EOL)
function_name param1 param_n 					function call
//								single line comment
/*	*/							multiple line comments

Remarks:

 - all basic blocks starts with optional name and parameters, then
   obligatory ':', and end with '=' return statement

------------------------------------------------	Sample code

//  Import some libraries
::system
::console

ending:
&i = 1, i < 100:
    x += i

    ? i :
    	|2 : = 
        |1 :
            out "znalazlem"
            = ending
        |0 :
	    = out "again"
    =
=

my_or_var = |2 3 4
my_and_var = &"hello" "world"

//  Same code written using continuations
ending:
&i = 1, i < 100:
    x += i,
    ? i :=
    	|2 : =
        |1:
            out "znalazlem"
            = ending
        |0:
	    = out "again"


    ? i > 2:
    //  Functions can be nested
    Function d:    
        "do these steps":
            function_run 1
            repeat 2 4	    
	=
	
        myFunction minX maxX :
            ? minX > maxX:
            	1: =
            	0: = print myFunction 1 2
	    =
	    
    a = a + 2

// Function definition (retruning only statement which is condition)
my_function d :=
    ? d :=
    	| "ok" : = "hello", // Note comma at the end, denotes continuation (which results in one-liner)
	| "not_ok" : = "world"

//  One-line function definition with result as sum of two variables
fun x y: = x + y

var = input
ret = fun: var 2

//  Loops can have ranges
&i = 0..100..2 := print i

//  New types definition (just like functions)
NewType:
    //  Fields definitions
    x = 0
    y = 0
	
    NewType param:= x = y = param
    Method a b:= x = a, y = b
=

instance = NewType
instance.x = 4
instance.Method 2 1
