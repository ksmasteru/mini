
/*https://stackoverflow.com/questions/71769632/making-a-shell-grammar-for-a-recursive-descent-parser*/

<complete_cmd> ::=  <and_or> <newline>
<and_or>       ::=  <pipeline> { ('&&' | '||') <pipeline> }        
<pipeline>     ::=  <command> { '|' <command> }
<command>      ::=  <simple_cmd> | '(' <and_or> ')'
<simple_cmd>   ::=  { <redirect> } <word> { ( <redirect> | <word> ) }
<redirect>     ::=  ( '<' | '>' | '<<' | '>>' ) <word>