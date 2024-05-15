#include "minishell.h"
#include "tokens.h"

t_tree *make_pipe_cmd(t_tree *left, t_tree *right)
{
    t_tree *cmd;

    cmd = malloc(sizeof(t_tree));
    if (!cmd)
        return (NULL);
    cmd->type = PIPE;
    cmd->left = left;
    cmd->right = right;
    return (cmd);
}

t_tree *make_and_cmd(t_tree *left, t_tree *right)
{
    t_tree *cmd;

    cmd = malloc(sizeof(t_tree));
    if (!cmd)
        return (NULL);
    cmd->type = AMPER;
    cmd->left = left;
    cmd->right = right;
    return (cmd);
}

t_tree *make_or_command(t_tree *left, t_tree *right)
{
    t_tree *cmd;
    
    cmd = malloc(sizeof(t_tree));
    if (!cmd)
        return (NULL);
    cmd->type = OR;
    cmd->left = left;
    cmd->right = right;
    return (cmd);
}

t_tree *make_great_command(t_tree *left, t_tree *right)
{
    t_tree *cmd;
    
    cmd = malloc(sizeof(t_tree));
    if (!cmd)
        return (NULL);
    cmd->type = GREAT;
    cmd->left = left;
    cmd->right = right;
    return (cmd);
}

/* exec are leafs*/
t_tree *make_exec_command(t_node **head, t_node *new)
{
    t_tree *cmd;

    cmd = malloc(sizeof(t_tree));
    if (!cmd)
        return (NULL);
    cmd->type = WORD;
    cmd->next = NULL; //ls of words!
    cmd->left = NULL;
    cmd->right = NULL;
    return (cmd);
}