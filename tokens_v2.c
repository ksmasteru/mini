#include "tokens.h"


// word->word->pipe = words->pipe

t_token *merge_words(t_token **curent, t_token **next)
{
    // change from 2 word to 1 words token
    t_token *new_next;
    t_token *slices;
    t_token *words;

    words = malloc(sizeof(t_token));
    
    
}
void token_overdrive(t_token **tokens)
{
    t_token *new;
    t_token *tmp;

    tmp = *tokens;
    while (tmp)
    {
        /*SIGV ?*/
        if (tmp->type == WORD && tmp->next->next == WORD)
            merge_words(&(tmp), &(tmp->next));
    }
}