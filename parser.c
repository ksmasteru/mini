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
t_tree *parse_word(t_token **head, t_data *data)
{
    t_tree *cmd;
    t_token *tmp;

    tmp = (*head)->next;
    /*char *buffer = (char *)malloc((*  head)->location.lenght + 1);
    if (!buffer)
        return (NULL);*/
    cmd = make_exec_command(*head);
    data->words_count++;
    *head = tmp;
    return (cmd);
}

// will be handled later
t_tree *parse_redirect(t_token **head, t_data *data)
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
        cmd = parse_word(head, data);
    else
        printf ("parsing error");
    return (cmd);
}

t_tree *parse_simple_command(t_token **head, t_data *data)
{
    t_tree   *cmd;
    t_tree   *tmp;

    /*check for rederection REDIRECTIONS LATER*/
    //if (((*head)->type == LESS || (*head)->type == GREAT)) redictions +
        //cmd = parse_redirect(head);
    if ((*head)->type == WORD)
        cmd = parse_word(head, data);
    /* here is the trickiest part*/
   // while ((*head)->type == LESS) // will handle redirection later
     //   exit(0);
    return (cmd);
}

t_tree *parse_command(t_token **head, t_data *data)
{
    t_tree *cmd;
    t_tree *tmp;

    /* if there is non >?*/
    /*simple command case  wron*/
    if ((*head)->type == WORD || (*head)->type == GREAT || (*head)->type == LESS) /* just for illustaration*/
        cmd = parse_simple_command(head, data);
    // it is either one or the other
    else if ((*head)->type == PAREN_L || (*head)->type == PAREN_R) /*here will be handleed later*/ 
        cmd = parse_and_or(head, data); 
    return (cmd);
}

t_tree *parse_pipe_line(t_token **head, t_data *data)
{
    t_tree *cmd;
    t_tree *tmp;

    cmd = parse_command(head, data); // if here another pipe line it should be
                                // at the left of this
    while ((*head)->type == PIPE) // what is is another --> stopped at a token
    {
        //seems logic, but it stops for one iteration need a ew
        tmp = cmd;
        *head = (*head)->next;
        cmd = make_pipe_cmd(tmp, parse_command(head, data));
        if ((*head) == NULL)
            break;
    }
    return (cmd);
}
t_tree *parse_and_or(t_token **head, t_data *data)
{
    t_tree *cmd;
    t_tree *tmp;
    cmd = parse_pipe_line(head, data);
    // this part seems logical so far would return a tree of pipes and execs
    /*this the building block*/   
    // if && or || is found what was parased before should be
    // left and what next should be right.
    /* there can be mulptiple and and ors probable use a while or recursive?*/

    // didnt return here
    if (*head != NULL)
    {
        if ((*head)->type == AMPER && (*head)->next->type == WORD)/* should handle |*/
        {
            tmp = cmd;
            // this should be right node
            cmd = NULL; /* 3iw*/
            cmd->right = tmp;
            cmd->left = parse_pipe_line(head, data);
        }
    }
    
   // else
   // {
     //   cmd->left = NULL;
       // cmd->right = NULL;
    //}
    return (cmd);
}
 
t_tree *parse_complete_cmd(t_token **head, t_data *data)
{
    t_tree *cmd;
    
    cmd = parse_and_or(head, data);
    return (cmd);
}

t_tree *parser(t_token *head, t_data *data)
{
    t_tree *cmd;
    cmd = parse_complete_cmd(&head, data);   
    return (cmd);
}