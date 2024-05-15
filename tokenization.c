#include "tokens.h"

t_token_type decode_type(int c)
{
    t_token_type type;

    if (c == '|')
        type = PIPE;
    else if (c == '<' || c == '>')
        type = REDIRECT;
    else if (c == '(')
        type = PAREN_L;
    else if (c == ')')
        type = PAREN_R;
    else
        type = WORD;
    return (type);
}

t_token *make_new_node(t_token_type type, char *start, size_t length)
{
    t_token *new;

    new = malloc(sizeof(t_token));
    if (!new)
        return (NULL);
    new->type = type;
    new->location = (t_slice){start, length};
    new->next = NULL;
	new->up = NULL;
    return (new);
}

void add_new_token(t_token **head , int c, char *start, size_t length)
{
    t_token *tmp;
    t_token *new;

    tmp = *head;
    t_token_type type = decode_type(c);
    if (*head == NULL)
    {
        *head = make_new_node(type, start, length);
        return ;
    }
    new = make_new_node(type, start, length);
    if (!new)
        return ;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
}
t_token  *lexer(char *str)
{
    char *start;
    size_t length;
    int word;
    //"     ls    -la      | echo    -la"
    /* skip spaces */
    t_token *head = NULL;
    char spaces[] = " \t\n\v\f\r";
    while (*str)
    {
        word = 0;
        length = 0;
        while (*str && (strchr(" \t\v\r", *str)))
            str = str + 1;
        start = str;
        if(*str && strchr("|<>()", *str))
        {
            add_new_token(&head, *str, start, 1);
            str = str + 1;
            start = str;   
        }
        while (*str && !(strchr(" \t\v\r|><)(", *str)))
        {
            word = 1;
            str = str + 1;
            length++;
        }
        if (word == 1)
            add_new_token(&head, *str, start, length);
    }
    return (head);
}

int main(int ac, char **av)
{

    char *str;
    if (ac != 2)
        return (0);
    str = av[1];
    t_token *tokens;
    t_token *tmp;

    char limiter[] = {"|<>()"};
    tokens = lexer(str);
    tmp = tokens;
    while (tmp != NULL)
    {
        printf("token type is %d and its lenght is %d\n", tmp->type, tmp->location.lenght);
        tmp = tmp->next;
    }
	tokens_v2(&tokens);
	printf("after token v2 \n");
	tmp = tokens;
	while (tmp != NULL)
    {
        printf("token type is %d and its lenght is %d\n", tmp->type, tmp->location.lenght);
        tmp = tmp->next;
    }
}