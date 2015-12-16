# CalcInterp
## A lexical scanner and semantic interpreter for a simple calculator based language

### Overview
CalcInterp, short for Calculator Interpreter, is a program written in C++ designed to perform several of the functions of a programming language compiler/interpreter. The code itself shows a number of programming techniques, including the use of applicable libraries, enumeration, and string manipulation. The 'lex-only' branch contains only the lexical analyzer (token scanner) portion of the project. The output for the lexical analyzer differs from that of the syntactic analyzer/semantic interpreter. The master branch contains the full interpreter. 

### The Language
The 'calculator' language used consists of three statement types:
 1. Assignment statement (ID := expression)
 2. Read from 'memory' (READ ID)
 3. Write to 'memory' (WRITE expression)

Statments (1) and (3) involve 'expressions'. These expressions are numerical expressions that may contain numerical constants ('decimal+' or 'decimal+.decimal+') and identifiers (variables of the form: letter(letter | digit | \_ )\*). The language also contains _single line_ comments of the form: /* ... */

Sample language code:

    /* Read values */
    read A          /* Fetch values from memory (in this case the user) */
    read B
    
    /* Perform computation */
    sum := A + B    /* Assign the value of 'A + B' to 'sum' */
    write sum       /* Write the value of sum to memory (in this case the screen) */
    write sum/2.0   /* Write the value of (sum/2.0) to memory */

The calculator language has 12 unique tokens (keywords/symbols): 
 1. EOFSY - 'End of File' symbol, denotes the end of the input stream
 2. ID - 'identifier', a variable/identifier name
 3. NUMCONST - a numerical constant
 4. READSY - `read` symbol, fetch a value from memory
 5. WRITESY - `write` symbol, write a value to memory
 6. ASSIGNSY - `:=`, assignment symbol for expressions
 7. LPAREN - `(`, left parenthesis symbol
 8. RPAREN - `)`, right parenthesis symbol
 9. ADDOP - `+`, addition operator
 10. MINOP - `-`, subtraction operator
 11. MULTOP - `*`, multiplication operator
 12. DIVOP - `/`, division operator

### The Grammar
The calculator grammar is a [context free grammar (CFG)](https://en.wikipedia.org/wiki/Context-free_grammar). The [Extended Backus-Naur Form](https://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_Form) for this grammar is as follows:

    <program>      --> <stmt_list> $$
    <stmt_list>    --> <stmt> <stmt_list> | ε
    <stmt>         --> id := <expr> | read id | write <expr>
    <expr>         --> <term> <term_tail>
    <term_tail>    --> <add_op> <term> <term_tail> | ε
    <term>         --> <factor> <factor_tail>
    <factor_tail>  --> <mul_op> <factor> <factor_tail> | ε
    <factor>       --> ( <expr> ) | id | number
    <add_op>       --> + | -
    <mul_op>       --> * | /

### How it works 
As described above, the `yylex` function parses through the input stream and identifies the next token. Based on which token is identified, the program will select which correct semantic path to take. Exceptions are thrown for lexical errors, syntactic errors, and semantic errors (addressed in that order). Exceptions will hault the interpreter, as the interpreter cannot continue its operations with improper code (this behavior is typical of language interpreters). The program is written such that the input stream is derived via a top down recursive parser. Certain recursive functions return values, and these values may be passed back up through the recursion chain to determine expression values. 

Sample output for the above sample code:
```
Read: Enter value for A> 14
Read: Enter value for B> 3
Assign: SUM = 17
Write: 17
Write: 8.5
```

#### Lexical analyzer output
The output from the 'lex-only' branch is not used in the final program, but provides insight into how the token recognizer works. 

**Note**: No semantic operations are performed during this stage.

Sample token recognizer output for the above sample code:
```
====SCANNER OUTPUT====
Token: 03 READYSY (read)
Token: 01 ID (A)
Token: 03 READYSY (read)
Token: 01 ID (B)
Token: 01 ID (sum)
Token: 05 ASSIGNSY (:=)
Token: 01 ID (A)
Token: 08 ADDOP (+)
Token: 01 ID (B)
Token: 04 WRITESY (write)
Token: 01 ID (sum)
Token: 04 WRITESY (write)
Token: 01 ID (sum)
Token: 11 DIVOP (/)
Token: 02 NUMCONST (2.0)
Token: 00 EOFSY ($$)

Number of tokens: 16
```
