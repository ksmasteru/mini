#include "tokens.h"

t_queue *new_follower(t_queue **head, t_queue **prev, t_tree *addr)
{
    t_queue *new;

    if (*head == NULL)
    {
        new = malloc(sizeof(t_queue));
        if (!new)
            return (NULL);
        new->addr = addr;
        new->next = NULL;
        *head = new;
        *prev = new;
        return (new);
    }
    new = malloc(sizeof(t_queue));
    if (!new)
        return (NULL);
    (*prev)->next = new;
    new->addr = addr;
    new->next = NULL;
    *prev = new;
    return (new);
}
t_queue *pop(t_queue **head)
{
    t_queue *tmp;

    if (*head == NULL)
        return (NULL);
    tmp = (*head);
    (*head) = (*head)->next;
    return (tmp);
}


void bfs(t_tree **root)
{
    t_queue *head;
    t_queue *current;
    t_queue *prev;
    t_queue *new;
    t_tree *tmp;

    prev = NULL;
    head = NULL;
    if ((*root) != NULL)
    {
        new_follower(&head, &prev, *root);
        prev = head;
    }
    while (head != NULL)
    {
        current = pop(&head);
        if (current == NULL) 
            break;
        *root = current->addr;
        if (*root != NULL)
        {
            printf("%d\n", (*root)->type);
            new = new_follower(&head, &prev, (*root)->left);
            new = new_follower(&head, &prev, (*root)->right);
        }
    }
}