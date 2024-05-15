#include "minishell.h"

/* a parser is divided into two parts : lexer --- > generate words into tokens.
 * and a parser ---> process the tokens according to a grammar and build the command table.
 *
 *  the tokens will be described in a file shell.l USING REGULAR EXPRESSIONS
 *
*   the grammer rules used by the parser are described in shell.y
 */

// Termianl expressions represnts acual symbols of our language <ALPHA> <DIGIT> <WORDS> <COMMAND>
// non terminal expressions are like '|' '&' 
<ALPHA> ::= [a-z] | [A-Z]
<DIGIT> ::= [0-9]
:WQ
:
