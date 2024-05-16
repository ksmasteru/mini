#include "minishell.h"
#include "tokens.h"

t_tree *make_pipe_cmd(t_tree *left, t_tree *right)
{
    t_tree *new;

    new = malloc(sizeof(t_tree));
    if (!new)
        return (NULL);
    new->token = NULL;
    new->left = left;
    new->right = right;
    return (new);
}

t_tree *make_and_new(t_tree *left, t_tree *right)
{
    t_tree *new;

    new = malloc(sizeof(t_tree));
    if (!new)
        return (NULL);
    new->token = NULL;
    new->left = left;
    new->right = right;
    return (new);
}

t_tree *make_or_command(t_tree *left, t_tree *right)
{
    t_tree *new;
    
    new = malloc(sizeof(t_tree));
    if (!new)
        return (NULL);
    new->token = NULL;
    new->left = left;
    new->right = right;
    return (new);
}

t_tree *make_great_command(t_tree *left, t_tree *right)
{
    t_tree *new;
    
    new = malloc(sizeof(t_tree));
    if (!new)
        return (NULL);
    new->token = NULL;
    new->left = left;
    new->right = right;
    return (new);
}

/* exec are leafs*/
t_tree *make_exec_command(t_token *word)
{
    t_tree *new;

    new = malloc(sizeof(t_tree));
    if (!new)
        return (NULL);
    new->token = word;
    new->left = NULL;
    new->right = NULL;
    return (new);
}