#include "tokens.h"
#include "parser.h"

void merge_words(t_token **current, t_token **next)
{
    t_token *new_next = (*next)->next;
    t_token *tmp;

    tmp = (*current);
    while (tmp->up)
        tmp = tmp->up;
    tmp->up = *next;
    (*next)->up = NULL;
    (*current)->next = new_next;
}

void make_redirection_token (t_token **token, t_token *next)
{
    t_token *tmp;

    tmp = (*token)->next->next;
    (*token)->up = next;
    if ((*token)->type == TO || (*token)->type == APPEND)
        (*token)->up->type = OUT_FILE;
    else if ((*token)->type == FROM)
        (*token)->up->type = IN_FILE;
    else if ((*token)->type == HEREDOC)
        ((*token)->up->type = DELIMETER);
    (*token)->type = REIDRECTION;
    (*token)->up->up = NULL;
    (*token)->next = tmp;
}

void tokens_v2(t_token **tokens)
{
    t_token *new;
    t_token *tmp;

    tmp = *tokens;
    while (tmp)
    {
        if (tmp->next != NULL)
        {
            if ((tmp->type >= FROM && tmp->type <= APPEND) && tmp->next->type == WORD)
            {
                make_redirection_token(&tmp, tmp->next);
                continue;
            }
           if (tmp->type == WORD && tmp->next->type == WORD)
            {
                merge_words(&tmp, &(tmp->next));
                continue;
            }
            tmp = tmp->next;
        }
        else
            tmp = tmp->next;
    }
    improve_tokens(tokens);
}

void ft_list_add_back(t_token **head, t_token *new)
{
    t_token *temp;
    
    temp = *head;
    if (temp == NULL)
    {
        *head = new;
        return ;
    }
    while (temp->next)
        temp = temp->next;
    temp->next = new;
    if (new != NULL)
        temp->next->next = NULL;
}

void merge_all_words(t_token **head)
{
    t_token *tmp;
    tmp = *head;
    while (tmp->next)
    {
        if (tmp->type == WORD && tmp->next->type == WORD)
            merge_words(&tmp, &(tmp->next));
        else
            break;
    }
}

void word_add_down(t_token **word, t_token *redir)
{
    t_token *tmp;
    t_token *prev_down;
    t_token *new_next;
    (*word)->next = redir->next;
    tmp = (*word)->down;
    if (tmp == NULL)
    {
        (*word)->down = redir;
        redir->next = NULL;
    }
    else
    {
        while (tmp->down)
            tmp = tmp->down;
        tmp->down = redir;
        tmp->down->down = NULL;
    }
}

void swap_redir_word(t_token **redir, t_token *word)
{
    t_token *temp;
    t_token *tmp_dwn;
    t_token *word_next;

    word_next = word->next;
    temp = *redir;
    tmp_dwn = word->down;
    if (tmp_dwn == NULL)
        word->down = temp;
    else
    {
        while (tmp_dwn->down)
            tmp_dwn = tmp_dwn->down;
        tmp_dwn->down = temp;
    }
    *redir = word;
    (*redir)->next = word_next;
}
// makes a linked list of words with the up atrribute.
void merge_the_words(t_token **words_list, t_token *new_word)
{
    t_token *tmp;
    t_token *last_up;
   // printf("added a word to list\n");
    tmp = *words_list;
    if (!tmp)
    {
        *words_list = new_word;
        return ;
    }
    if ((tmp->up) == NULL)
        tmp->up = new_word;
    else
    {
        while (tmp->up)
            tmp = tmp->up;
        tmp->up = new_word;
        tmp->up->up = NULL;
    }
}
// makes a linked list of redirect with the down atrribute
void merge_the_redirections(t_token **redirection_list, t_token *new_red)
{
    t_token *tmp;

    tmp = *redirection_list;
    if (!tmp)
    {
        *redirection_list = new_red;
        return ;
    }
    if (!tmp->down)
        tmp->down = new_red;
    else
    {
        while (tmp->down)
            tmp = tmp->down;
        tmp->down = new_red;
        tmp->down->down = NULL;
    }
}

t_token *merge_simple_command(t_token **words_list, t_token  **redirection_list)
{
    t_token *simple_command;
    
    if (*words_list != NULL)
    {
        (*words_list)->down = *redirection_list;
        (*words_list)->next = NULL;
        simple_command = *words_list;
    }
    else
        simple_command = *redirection_list;
    return (simple_command);
}
t_token *tokens_v6(t_token **tokens)
{
    t_token *tmp;
    t_token *redirection_list;
    t_token *words_list;
    t_token *shunk;

    t_token *tempo;
    redirection_list = NULL;
    words_list = NULL;
    tmp = *tokens;
    while (tmp->type != PIPE)
    {
        if (tmp->type == WORD)
            merge_the_words(&words_list, tmp);
        else if (tmp->type == REIDRECTION)
            merge_the_redirections(&redirection_list, tmp);
        tmp = tmp->next;
        if (tmp == NULL)
            break;
    }
    shunk = merge_simple_command(&words_list, &redirection_list);
    tempo = shunk;
    *tokens = tmp;//checked
    return (shunk);
}
// links using the next attribute
void ft_list_addback(t_token **head, t_token *new)
{
    t_token *tmp;
    t_token *new_next;

    if (new != NULL)
        new_next = new->next;
    tmp = *head;
    if (!new)
        return ;//hmm
    if (!tmp)
    {
        *head = new;
        return ;
    }
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
    tmp->next->next = new_next;
}
void improve_tokens(t_token **tokens)
{
    t_token *tmp;
    t_token *shunk;
    t_token *final_list;

    final_list = NULL;
    while ((*tokens) != NULL)
    {
        shunk = tokens_v6(tokens);
        shunk->next = NULL;
        ft_list_addback(&final_list, shunk);
        if (*tokens == NULL)
           break;
        if ((*tokens)->type == PIPE)
        {
            ft_list_addback(&final_list, (*tokens));  // adding the pipe
            *tokens = (*tokens)->next;
        }
    }
    *tokens = final_list;
}
