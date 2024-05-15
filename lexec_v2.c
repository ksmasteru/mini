#include "minishell.h"
#include "tokens.h"

/* WORD->WORD->PIPE->WORD->WORD->PIPE->WORD->GREAT->WORD
*   WORD->PIPE-WORD->PIPE->WORD>GREAT>FILENAME
*
*
*/
void merge_words(t_token **current_token, t_token **next_token)
{
    t_token *new_next_token;

    new_next_token = (*next_token)->next;
     
    
}
t_token *lexer_v2(t_token **tokens)
{
    
    t_token *tmp = *tokens;
    t_toekn *head = *tokens;

    while (tmp)
    {
        if (tmp->type == WORD && tmp->next->type == WORD)
            merge_words(&tmp, &(tmp->next)); //tmp->next will skip
        
    }
}