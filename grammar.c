#include "minishell.h"

/* Grammer for shell -- Backus-Naur Forn -- */


/*
 *
 * cmd [arg]* [|cmd[args]*]*
 * 
 *
* [ [> filename] [< filename] [ >& filename] [>> filename] [>>& filename] ]* */

<CHAR> ::= [a-z] | [A-Z]
<DIGIT> ::= [0-9]
<NUMBER> ::= [0-9][0-9]*
<WORD> ::= <CHAR> | <WORD> <CHAR>
            | <WORD> '_'
<WORD-LIST> ::= <WORD> | <WORD-LIST> <WORD>
<REDIRECTION> ::=   '>' <WORD>
                |   '<' <WORD>
<REDIRECTION> ::=  '>>' <WORD>
                |   '<<' <WORD>

