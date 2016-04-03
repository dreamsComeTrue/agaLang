# agaLang
Compiler for my staticaly-typed programming language written in C++

Current language features:
- own written lexer
- top down parser 
- Abstract Syntax Tree generation
- intermediate assembly representation / possibly using LLVM

--	Language specification (constantly evolving):

```
::identifier											import statement
<identifier> <param1> <param n...> : <statements> =		basic block/function definition
&expression:expression:statements =			 			loop
?expression basic_block_1 basic_block_n =				condition block
identifier := expression								assignment
= expression											return expression value
=& block_name											(in loop) break/continue
,														statement separator (optional at EOL)
function_name param1 param_n 							function call
//														single line comment
/*	*/													multiple line comments

Remarks:

 - all basic blocks starts with optional name and parameters, then
   obligatory ':', and end with '=' return statement

--	Sample code

::file
::system

=& label

break:
&i = 1 : i < 100 :
    x += i

    ? i % 2 == 0
        1:
            out "znalazlem"
            &= break
        0:= out "again"
   :=
:=

    ? i > 2
    : Function d, sin 90 80 =
    :
        "do these steps":
            function_run 1
            repeat 2 4

        myFunction minX maxX :
            ? minX > maxX
            : minX =
            : maxX
              print myFunction 1 2 .

    a = a + 2

    Function d:
        ? d > 2 : 2 =
        d + 5
        =
=

fun x y: x + y

var = input
ret = fun: var 2

NewType:
	x = 0
	y = 0
	
    NewType: x = y = 0.
    Method a b: x = a, y = b.
=

instance = NewType
instance.x = 4
instance.Method 2 1
