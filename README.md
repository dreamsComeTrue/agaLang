# agaLang
Compiler for my staticaly-typed programming language written in C++

Current language features:
- own written lexer
- top down parser 
- Abstract Syntax Tree generation
- intermediate assembly representation

--	Language specification (constantly evolving):

@LITERAL 												import statement
[IDENTIFIER STATEMENTS]									basic block
[IDENTIFIER &EXPRESSION:EXPRESSION:STATEMENTS] 			loop
[IDENTIFIER?EXPRESSION: STATEMENTS] 					condition block
IDENTIFIER = EXPRESSION              					assignment
,														statement separator (optional at EOL)
[IDENTIFIER IDENTIFIER_LIST : STATEMENTS]				function definition
IDENTIFIER: EXPRESSION_LIST								function call
//														single line comment
/*	*/													multiple line comments

--	Sample code

@file//.aga

[&i = 1 : i < 100 :
	x += i
	
	[? i > 2 
		Function: d,
		x++

		[Function d: d]
	]
]

[fun x y: x + y]

var = input:
ret = fun: var 2

[NamedCondition1 ? ret > 3:
	[NamedBlock 2+2]
	[OtherBlock par: par + "_kot"]

	[NamedLoop & i=0:i<ret:++i,
		print: "Hello " i
		OtherFunc: 2 4

		list = [NamedBlock, NamedBlock, NamedBlock]
		OtherBlock: "hey"
	]
]

[NewType:
	x = 0
	y = 0
	
	[NewType: x = y = 0]
	[Method a b: x = a, y = b]
]

instance = NewType:
instance.x = 4
instance.Method: 2 1
