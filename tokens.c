
#include "minishell.h"
#include <stdbool.h>

/* Grammar
 * <words> ::= [a-z][A-Z]
 * <item> ::= word | input-output
 * <simple_command> ::= item
 * <command> ::= simple_command
 * <pipeline> ::= <command> '|' <command> ('|'pipeline)?
 * 
 */

bool has_next(char *str) {return *str != '\n' && *itr != '\0';}
char peek (char *str) {return *str;}
char next(char **str) {char next == *itr; *itr +=1; return next;}


/* helper functions take an adress and increment it and return a toke */
t_token take_cmd(char **str);
t_token take_pipeline(char **str);
void print_token(t_token *token);
// review incremetantion in each function

t_token parse_word(char **str)
{
    // max hour == 11;
    // 2 hours la facfa
    t_slice location;
    t_token token;
    t_size length = 1;
    char *start = *str;
    char c;
    location.start = start;
    c = peek(*str);
    while (is_aplha(c) || is_digit(c)) /* handle those starting with digit */
    {
        *str = *str + 1;
        length += 1;
        c = peek(*str); //error handle here ? 
    }
    location.length = length;
    token = (t_token){token_type.WORD, location};
    return (token);
}

t_token parse_redirect(char **str)
{
    t_slice location;
    t_token token;
    size_t length = 1;
    location.start = *str;
    char c;

    c = peek(*str);
    // each token make it into a struct which shoudd have sub structs
    if (c == '<' || c == '>')
    {
        *str = *str + 1;
        c = peek(*str);
    }
    while ((*str == 32) || (*str >= 9 && *str <= 13))
    {
        *str = *str + 1;
        length++;
    }
    if (is_aplha(*str))
        pars_word();
    else
    {
        write (2, "parrsing error near redirection\n", 18);
        exit (1);
    }
    location.lenght = length;
    token = (t_token){token_type.REDIRECT, location}
    return token;
}

t_token parse_simple_cmd(char **str)
{
    char c;
    *str = *str + 1; //peeked before
    c = peek(*str);
    if (c == '<' || c == '>') //handle << >> too somehow
        parse_redirect(str);
    if (is_alpha(c))
        parse_word(str); 
    else
    {
        printf("error parse simple command\n");
        exit (1); 
    }
    if (c == '<' || c == '>')
        parse_redirect(str);
    
}

t_token parse_command(char **str)
{
    t_slice slice;
    t_token token;
    char *start = *str;
    slice.start = start;
    char c = *str;
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
    {
        parse_simple_cmd(char **str);
    }
    else if (c == '(')
    {
        printf("parsing and or"\n);
        exit (0);
    }
    else
    {
        printf("parsing error\n");
        exit(1);
    }
    return (token);
}

t_token parse_pipe_line(char **str)
{
    char *start = *str;
    if (*str)
        parse_command(str);
    if (has_next(*str))
    {
        while (*str == 32 || *str >= 9 && *str <= 13)
            *str++;
        if (*str  = )
    }
 
        //TODO
}
int main(int ac, char **av)
{
    if (ac != 2)
        return (0);
    // parse av[1]
    char c;
    t_token token;
    char *str = av[1];
    char **str_add = &str;    
    while (has_next(str))
    {
        c = *str;
        if (c)
            token = parse_pipe_line(str);
    }
}
