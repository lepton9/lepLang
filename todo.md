DONE:
- check if variable has initialized value before use

TODO:
- memory address of the value
- type casting
- tests for hashtab
- better error printing
    - error header to handle all errors across the compiling process
- rest of the grammar parsing
- semantic analysis
    - type checking
    - scope resolution
        - Variables and functions declares before use, scope rules
    - symbol table
        - Keep track of variable and function declarations and their attributes
    - other semantic rules
- intermediate code generation





- Lex into tokens
- Parse tokens to AST
- Compile AST to assembly
- Assembler -> machine code

- lexer
    - tokenization
    - int:a = 1; -> to tokens
        - keyword, colon, identifier, operator/equals/assign, int_literal, semicolon
    - Tokens maybe in a dynamic list
- parser
    - parsing the tokens
- compiler
    - compile to machine language

- # This is a comment until \n
- functions are linear transformations
- lines end in semicolons
- function syntax
    - function {type -> type: logic}
- example:
"""
    sum : (a:int, b:int) -> int {
      int c = a + b;
      return c;
    }

    main {void -> int : return 0;}
    main : (void) -> int {
        return 0;
    }
"""

- Types:
    - int
    - float
    - char
    - str
    - bool
    - void
    - f (function)



sum {a:int, b:int -> int : {
    int c = a+b;
    return c;
}}

sum {a:int, b:int -> int : 
  int c = a + b;
  return c;
}

sum : a:int, b:int -> int {
  int c = a + b;
  return c;
}

sum : (a:int, b:int -> int) {
  int c = a + b;
  return c;
}

sum(a:int, b:int) -> int {
  int c = a + b;
  return c;
}

sum : (a:int, b:int) -> int {
    c:int = a + b;
    return c;
}

sum : (int:a, int:b) -> int {
    int:c = a + b;
    return c;
}

f:sum : (int:a, int:b) -> int {
    int:c = a + b;
    return c;
}

f:sum = (int:a, int:b) -> int {
    int:c = a + b;
    ret c;
}

f:sum(int:a, int:b) -> int {
    int:c = a + b;
    return c;
}

