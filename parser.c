// parses tokens into an exection tree
/*
*           Based on the following BNF Grammar
*       <complete_cmd> ::= <and_or> <newline>
*        <and_or>        ::= <pipeline> [('&&') <pipeline>  | ('||') <pipeline>]*
*       <pipeline>      ::= <command> ['|' <command>]]*
*        <command>       ::= <simple_cmd> | '(' <and_or> ')'
*        <simple_cmd>    ::= [<redirect>]* <word> [<redirect> | <word>]*
*        <redirect>      ::= ('<' | '>' | '<<' | '>>') <word>
*/


#include "tokens.h"
#include "parser.h"
// will start testing with pipes, redirections later
// when to change the plan ? special characters ?
t_tree *parse_word(t_token **head)
{
    t_tree *cmd;
    t_token *tmp;

    tmp = (*head)->next;
    printf("tmp value is %p\n", tmp);
    /*char *buffer = (char *)malloc((*  head)->location.lenght + 1);
    if (!buffer)
        return (NULL);*/
    cmd = make_exec_command(*head);
    printf("cmd type is %d\n", cmd->token->type);
    *head = tmp;
    return (cmd);
}

// will be handled later
t_tree *parse_redirect(t_token **head)
{
    t_tree   *cmd;
    t_tree   *tmp;
/*
    if ((*head)->type == LESS)
    else if (*head->type == GREAT)
    else if (data == ">>")
    else if (data == "<<")
    MUST be followed by a word*/
    if ((*head)->type == WORD)
        cmd = parse_word(head);
    else
        printf ("parsing error");
    return (cmd);
}

t_tree *parse_simple_command(t_token **head)
{
    t_tree   *cmd;
    t_tree   *tmp;

    printf("in parse simple command head add %p\n and its type is %d\n", (*head), (*head)->type);
    /*check for rederection REDIRECTIONS LATER*/
    //if (((*head)->type == LESS || (*head)->type == GREAT)) redictions +
        //cmd = parse_redirect(head);
    if ((*head)->type == WORD)
    {
        cmd = parse_word(head);
    }
    /* here is the trickiest part*/
    while ((*head)->type == LESS) // will handle redirection later
        exit(0);
    return (cmd);
}

t_tree *parse_command(t_token **head)
{
    t_tree *cmd;
    t_tree *tmp;

    /* if there is non >?*/
    /*simple command case  wron*/
    if ((*head)->type == WORD || (*head)->type == GREAT || (*head)->type == LESS) /* just for illustaration*/
        cmd = parse_simple_command(head);
    // it is either one or the other
    else if ((*head)->type == PAREN_L || (*head)->type == PAREN_R) /*here will be handleed later*/ 
        cmd = parse_and_or(head); 
    return (cmd);
}

t_tree *parse_pipe_line(t_token **head)
{
    t_tree *cmd;
    t_tree *tmp;

    cmd = parse_command(head); // if here another pipe line it should be
                                // at the left of this
    if ((*head)->type == PIPE) // what is is another --> stopped at a token
    {
        //seems logic
        tmp = cmd;
        *head = (*head)->next;
        cmd = make_pipe_cmd(tmp, parse_command(head));
    }
    return (cmd);
}
t_tree *parse_and_or(t_token **head)
{
    t_tree *cmd;
    t_tree *tmp;
    cmd = parse_pipe_line(head);
    // this part seems logical so far would return a tree of pipes and execs
    /*this the building block*/   
    // if && or || is found what was parased before should be
    // left and what next should be right.
    /* there can be mulptiple and and ors probable use a while or recursive?*/

    // didnt return here
    printf("cmd was allocated and it is address is %p, and it data is %d\n", cmd, cmd->token->type);
    if ((*head)->type == AMPER && (*head)->next->type == WORD)/* should handle |*/
    {
        tmp = cmd;
        // this should be right node
        cmd = NULL; /* 3iw*/
        cmd->right = tmp;
        cmd->left = parse_pipe_line(head);
    }
    else
    {
        cmd->left = NULL;
        cmd->right = NULL;
    }
    return (cmd);
}
 
t_tree *parse_complete_cmd(t_token **head)
{
    t_tree *cmd;
    
    cmd = parse_and_or(head);
    return (cmd);
}

t_tree *parser(t_token *head)
{
    t_tree *cmd;
    cmd = parse_complete_cmd(&head);   
    return (cmd);
}