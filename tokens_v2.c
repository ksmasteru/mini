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
    improve_tokens(tokens);
    //tokens_v5(tokens);
    //tmp = *tokens;
    //*tokens = tokens_v4(tmp);
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
// make word the parent of redirection and swap it
// 18 -> 1 ==> 1    printf("tmp token is %d and adress is %p", tmp->type, (*tokens));
             //18

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
        tmp_dwn->down = temp; // first problem it should be at the top!
    }
    *redir = word;
    (*redir)->next = word_next;
}
void *tokens_v5(t_token **tokens)
{
    t_token *temp;
// 18 -> 1 === 1
    temp = *tokens;
    t_token *head;

    head = NULL;
    while (temp->next)
    {
        if (temp->type == REIDRECTION && temp->next->type == WORD)
        {
            swap_redir_word(&temp, temp->next);
            if (head == NULL)
                head = temp;
            continue;
        }
        if (temp->type == WORD && temp->next->type == REIDRECTION)
        {
            word_add_down(&temp, temp->next);
            if (head == NULL)
                head = temp;
            continue;
        }
        if (temp->type == WORD && temp->next->type == WORD)
        {
            merge_words(&temp, &(temp->next));
            if (head == NULL)
                head = temp;
            continue;
        }
        temp = temp->next;
    }
}
// makes a linked list of words with the up atrribute.
void merge_the_words(t_token **words_list, t_token *new_word)
{
    t_token *tmp;
    t_token *last_up;

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

// makes two lists
t_token *merge_simple_command(t_token **words_list, t_token  **redirection_list)
{
    t_token *simple_command;
    
    if (*words_list != NULL)
    {
        (*words_list)->down = *redirection_list;
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

    *tokens = tmp;// ????
    return (shunk);
}
// links using the next attribute
void ft_list_addback(t_token **head, t_token *new)
{
    t_token *tmp;

    tmp = *head;
    if (!new)
        return ;
    if (!tmp)
    {
        *head = new;
        return ;
    }
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
    tmp->next->next = NULL;
}
void improve_tokens(t_token **tokens)
{
    t_token *tmp;
    t_token *shunk;
    t_token *final_list;

    final_list = NULL;
    tmp = *tokens;
    while (tmp != NULL)
    {
        shunk = tokens_v6(tokens);
        ft_list_addback(&final_list, shunk);
        if (*tokens == NULL)
            break;
         if ((*tokens)->type == PIPE)
            tmp = (*tokens)->next;
    }
    *tokens = final_list; //should all free all used nods
}