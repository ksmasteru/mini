#include "tokens.h"
#include "parser.h"
#include "pipes.h"
#include "executer.h"
// < redirection id followed by the filename
// unless it is a heredoc << it is followed by a delimiter
t_token_type decode_type(char *start, int c)
{
    t_token_type type;

    if (c == '|')
        return  PIPE;
    else if (c == '<' && *(start + 1) == '<') // this might SEGv
        return (HEREDOC);
    else if (c == '<')
        return (FROM);
    else if (c == '>' && *(start + 1) == '>')
        return (APPEND);
    else if (c == '>')
        return (TO);
    else if (c == '(')
        return (PAREN_L);
    else if (c == ')')
        return (PAREN_R);
    else
        return (WORD);
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
    new->down = NULL;
    return (new);
}

void add_new_token(t_token **head , int c, char *start, size_t length)
{
    t_token *tmp;
    t_token *new;

    tmp = *head;
    t_token_type type = decode_type(start, c);
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
int is_quotes(int c)
{
    printf("c is %c", c);
    if (c == 34 || c == 39)
    {
        printf("found quotes !");
        return (1);
    }
    return (0);
}
t_token  *lexer(char *str)
{
    char *start;
    size_t length;
    int word;
    t_token *head = NULL;
    char spaces[] = " \t\n\v\f\r";
    while (*str)
    {
        word = 0;
        length = 0;
        while (*str && (strchr(" \t\v\r", *str)))
            str = str + 1;
        start = str;
        if (*str == 39)
            printf("3iw");
        if(*str && (strchr("|<>()", *str)))
        {
            //exit(0);
            add_new_token(&head, *str, start, 1); //need more paramters.
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
            add_new_token(&head, *(str - 1), start, length);
    }
    return (head);
}


/* SEG when there is no pipe*/
//< operations1.c wc -w
// LESS FILE CMD
// exception like cmd > outfile word will be handled later

int parse_cmd(char *line, char **envp)
{
    char *str;
    t_data data;
    str = line;
    data.envp = envp;
    data.words_count = 0;
    t_token *tokens;
    t_token *tmp;
    t_tree *temp;
    t_tree *tempo;
    t_tree *root;
    t_token *tempa;
    char limiter[] = {"|<>()"};
    tokens = lexer(str);
	tokens_v2(&tokens);
    tmp = tokens;
/*    while (tmp)
    {
        printf(" parent type %d\n", tmp->type);
        if (tmp->down)
            printf("  down is %d\n", tmp->down->type);
        if (tmp->up)
            printf("  child up is %d\n", tmp->up->type);
        tmp = tmp->next;
    }*/
    root = parser(tokens, &data);
    data.env = get_envp(data.envp);
    if (data.words_count > 1)
    {
      int *pids = (int *)malloc(sizeof(int) * (data.words_count - 1));
        int **pfd = (int **)malloc(sizeof(int *) * data.words_count - 1);
        for (int i = 0 ; i < data.words_count ; i++)
          pfd[i] = (int *)malloc(sizeof(int) * 2);
        fill_pipes(&pfd, data.words_count - 1);
        data.fdx = pfd;
        data.pids = pids;
        run_cmd(&root, data.words_count - 1, data.words_count, &data);
    }
    else
        execute_cmd(root, 0, 1, &data);
    exit(0);
}