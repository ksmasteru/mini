#include "tokens.h"


// word->word->pipe = words->pipe

void merge_words(t_token **current, t_token **next)
{
    // change from 2 word to 1 word token
    t_token *new_next = (*next)->next;
    t_token *tmp;

    tmp = (*current);
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
}

void tokens_v2(t_token **tokens)
{
    t_token *new;
    t_token *tmp;

    tmp = *tokens;
    while (tmp)
    {
        /*join words*/
        /* thee lopp exited of this condition*/
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
}
