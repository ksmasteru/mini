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
void tokens_v2(t_token **tokens)
{
    t_token *new;
    t_token *tmp;

    tmp = *tokens;  
    while (tmp)
    {
        /*join words*/
        if (tmp->next != NULL)
        {
            if (tmp->type == FROM && tmp->next->type == WORD)
            {
                tmp->next->type = IN_FILE;
                tmp = tmp->next->next;
                continue;
            }
            if (tmp->type == TO && tmp->next->type == WORD)
            {
                tmp->next->type = OUT_FILE;
                tmp = tmp->next->next;
                continue;
            }

            if (tmp->type == WORD && tmp->next->type == WORD)
            {
                merge_words(&tmp, &(tmp->next));
                continue;
            }
            else if (tmp->type == HEREDOC && tmp->next->type == WORD)
                tmp->next->type = DELIMETER; // will be handled later
            tmp = tmp->next;
        }
        else
            tmp = tmp->next;
    }
}