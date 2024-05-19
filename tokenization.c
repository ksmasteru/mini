#include "tokens.h"
#include "parser.h"
#include "pipes.h"
#include "executer.h"

t_token_type decode_type(char *start, int c)
{
    t_token_type type;

    if (c == '|')
        return  PIPE;
    else if (c == '<' && *(start + 1) == '<')
        return (GREATGREAT);
    else if (c == '<')
        return (GREAT);
    else if (c == '>' && *(start + 1) == '>')
        return (HEREDOC);
    else if (c == '>')
        return (LESS);
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
            add_new_token(&head, *str, start, length);
    }
    return (head);
}


/* SEG when there is no pipe*/
int main(int ac, char **av, char **envp)
{
    char *str;
    t_data data;
    if (ac != 2)
        return (0);
    str = av[1];
    data.envp = envp;
    data.words_count = 0;
    t_token *tokens;
    t_token *tmp;
    t_tree *temp;
    t_tree *tempo;
    char limiter[] = {"|<>()"};
    tokens = lexer(str);
	tokens_v2(&tokens);
    tmp = tokens;
    t_tree *root = parser(tokens, &data);
    /*while (tempo != NULL)
    {
        printf("tree type is %d\n", tempo->type);
        printf("temp left is %d\n", tempo->left->type);
        printf("temp right is %d\n", tempo->right->type);
        tempo = tempo->left;
        printf("down level left\n");
        printf("tree type is %d\n", tempo->type);
        printf("temp left type is %d\n", tempo->left->type);
        printf("temp right type is %d\n", tempo->right->type);
        
        break;
    }*/
    //bfs(&root);
     //pre_order_traversal(&root);
    //left_root_right(&root);
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
 //send in number of pipes good so far it has to be 1
                                // so the left can be 0
    //printf("token data is %s", root->right->token->location.location);
}