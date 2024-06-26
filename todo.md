DONE:
- assingment statements
- variable decl

TODO:
- better error printing
- rest of the grammar parsing
- function calls
- assembler






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

