// parses tokens into an exection tree
/*
*           Based on the following BNF Grammar
*       <complete_cmd> ::= <and_or> <newline>
*        <and_or>        ::= <pipeline> [('&&') <pipeline>  | ('||') <pipeline>]*
*       <pipeline>      ::= <command> ['|' <command>]*
*        <command>       ::= <simple_cmd> | '(' <and_or> ')'
*        <simple_cmd>    ::= [<redirect>]* <word> [<redirect> | <word>]*
*        <redirect>      ::= ('<' | '>' | '<<' | '>>') <word>
*/


#include "tokens.h"

// will start testing with pipes, redirections later
// when to change the plan ? special characters ?
t_tree *parse_word(t_token **head)
{
    /* leaks handeled using justice*/
    char *buffer = (char *)malloc(*head->location.lenght + 1);
    if (!buffer)
        return (NULL);
    if (*head->prev == WORD)
    {
        
    }
    cmd = make_exec_command();
    cmd->data = strncpy(buffer, *head->slice.start, *head->slice.length);
    cmd->data[*head->location.lenght + 1] = '\0';
    head = *head->next;
    return (cmd);
}

/* will be handled later*/
t_tree *parse_redirect(t_token **head)
{
    t_cmd   *cmd;
    t_cmd   *tmp;

    if (head->token == '<') /*token type ?*/
    else if (data == '>')
    else if (data == ">>")
    else if (data == "<<")
    /* MUST be followed by a word*/
    if (head->token.type == WORD)
        cmd = parse_word(head);
    else
        printf ("parsing error");
    return (cmd);
}

t_tree *parse_simple_command(t_token **head)
{
    t_cmd   *cmd;
    t_cmd   *tmp;

    /*check for rederection REDIRECTIONS LATER*/
    if ((*head->type == REDIRECT || *head->type == GREAT)) /*redictions +*/
        cmd = parse_redirect(head);
    if (*head->type == WORD)
    {
        cmd = parse_word(head);
    }
    /* how to join words as an array of args ?*/
    while (head->type == REDIRECT || head->type == WORD)
    {
        cmd->next = parse_word(*head);
    }
    return (cmd);
}

t_tree *parse_command(t_token **head)
{
    t_cmd *cmd;
    t_cmd *tmp;

    /* if there is non >?*/
    /*simple command case  wron*/
    if (*head->type == WORD || *head->type == GREAT || *head->type == REDIRECT) /* just for illustaration*/
        cmd = parse_simple_cmd(head);
    // it is either one or the other
    else if (*head->type == PAREN_L || *head->type == PAREN_R) /*here will be handleed later*/ 
        cmd = parser_and_or(head); 
    return (cmd);
}

t_tree *parse_pipe_line(t_token **head)
{
    t_cmd *cmd;
    t_cmd *tmp;

    cmd = parse_command(head); // if here another pipe line it should be
                                // at the left of this
    if (*head->next.type == PIPE) // what is is another --> stopped at a token
    {
        //seems logic
        tmp = cmd;
        *head = *head->next;
        cmd = make_pipe_cmd(tmp, parse_command(*head));
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
    if (*head->data == AMPER && *head ->next.type == WORD)/* should handle |*/
    {
        tmp = cmd;
        // this should be right node
        cmd = make_amper_node();
        cmd->right = tmp;
        cmd->left = parse_pipe_line(*head);
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
}

t_tree *parser(t_token *head)
{
    t_tree *cmd;
    cmd = parse_complete_cmd(&head);   
}
