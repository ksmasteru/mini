#include "tokens.h"
#include "parser.h"

// check if down and up attributes store data correctly in the word token
void merge_words(t_token **current, t_token **next)
{
    // change from 2 word to 1 word token
    t_token *new_next = (*next)->next;
    t_token *tmp;

    tmp = (*current); //change it to next ? on up and the other nexts
    while (tmp->up)
        tmp = tmp->up;
    tmp->up = *next;
    (*next)->up = NULL;
    (*current)->next = new_next;
}

// "hello wrold lol | daspipee | another big fucking pipe"

//"will handle redirections later"
// < file wc -w ---> make the word after < as infine the other as a signle word
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
   /* if (!tmp)
        (*token)->next = tmp;
    else
    {
        if (tmp->type == WORD && (*token)->prev =)) //what if word word word
    }*/
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
    tmp = *tokens;
    //*tokens = tokens_v4(tmp);
    tokens_v5(tokens);
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
    // w -> w -> w each can have ups.
    tmp = *head;
    while (tmp->next)
    {
        if (tmp->type == WORD && tmp->next->type == WORD)
            merge_words(&tmp, &(tmp->next));
        else
            break;
    }
}
// this function should check for parsing errors.
// linked list of redirections. then add that linked list as the  
void tokens_v3(t_token **tokens)
{
    t_token *temp;
    t_token *words_head;
    t_token *redirection_head;

    temp = *tokens;
    words_head = NULL;
    redirection_head = NULL;
    // the final *tokens should be of type word if there is atleast one work!
    while (temp && temp->type != PIPE) //protect seg double while
    {
        if (temp->type == WORD)
            ft_list_add_back(&words_head, temp);
        else if (temp->type == REIDRECTION)
            ft_list_add_back(&redirection_head, temp);
        temp = temp->next;        
    }
    if (words_head == NULL)
        *tokens = redirection_head;
    else
    {
        merge_all_words(&words_head);
        words_head->down = redirection_head;
        *tokens = words_head;
    }
    if (temp != NULL)
        ft_list_add_back(tokens, temp); //will be called by temp next
}
// should return linked list from a token start to pipe.

// tihs function tokenize from pipe to pipe
t_token *tokens_v4(t_token *start)
{
    t_token *temp;
    t_token *words_head;
    t_token *redirection_head;

    temp = start; // start is the token after pipe.
    if (temp == NULL)
    {
        //parsing error but not for the first time this iscalled
        // should handle parsing error if  | nothing
        return (NULL); 
    }
    if (temp->type == PIPE)
        temp = temp->next; // will add pipes if this works
    while (temp && temp->type != PIPE)
    {
        if (temp->type == WORD)
            ft_list_add_back(&words_head, temp);
        else if (temp->type == REIDRECTION)
            ft_list_add_back(&redirection_head, temp);
        temp = temp->next; 
    }
    if (words_head == NULL)
    {
        ft_list_add_back(&redirection_head, tokens_v4(temp));
        return (redirection_head);
    }
    else
    {
        merge_all_words(&words_head);
        words_head->down = redirection_head;
        ft_list_add_back(&words_head, tokens_v4(temp));
        return (words_head);
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
void *tokens_v5(t_token **tokens)
{
    t_token *temp;

    temp = *tokens;
    while (temp->next)
    {
        if (temp->type == WORD && temp->next->type == REIDRECTION)
        {
            word_add_down(&temp, temp->next);
            continue;
        }
        if (temp->type == WORD && temp->next->type == WORD)
        {
            merge_words(&temp, &(temp->next));
            continue;
        }
        temp = temp->next;
    }
}
