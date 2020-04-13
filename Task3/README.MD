Usage:
  ./modelLang [options] file

Options:
-t' - print interpreted text (your program)
-l' - print processed lexemes
-i' - print processed in lexical and syntax alalyze table of identifiers
-r' - print reverse polish notation of code
-s' - print stack during code execution
-ss' - print stack during code execution step by step
-a' - print all available information
-e' - execute interpreted program


Additional information about language and syntax rules:

Data types:
    'int' - integer data type
    'real' - real data type
    'boolean' - boolean data type
    'string' - string data type

Keywords:
    'program' - template keyword for source

    'read' - read to identifier(s) values from standart input stream
    'write' - write expression(s) to standart output stream

    'if' - conditional operator (if)
    'else' - else branch of conditional operator (else)

    'while' - cycle operator (while)
    'do' - cycle operator (do)
    'for' - cycle operator (for)

    'false' - boolean constant (false)
    'true' - boolean constant (true)

Ariphmetics operations:
    '+' - addition operation (only for integer, real and string data types)
    '-' - subtraction operation (only for integer and real data types)
    '*' - multiplication operation (only for integer and real data types)
    '/' - division operation (only for integer and real data types)

Logical operations:
    'not' - not operation (only for boolean data type)
    'and' - and operation (only for boolean data type)
    'or' - or operation (only for boolean data type)

Comparison operations:
    '==' - operation 'equal'
    '!=' - operation 'not equal'
    '<' - operation 'less' (only for integer, real and string data types)
    '>' - operation 'greater' (only for integer, real and string data types)
    '<=' - operation 'less or equal' (only for integer and real data types)
    '>=' - operation 'greater ot equal' (only for integer and real data types)

Template of the interpreted program (source code):
  program {
      [declaration(s)]
      [operator(s)]
  }
