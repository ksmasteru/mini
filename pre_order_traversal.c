
#include "tokens.h"
#include "parser.h"
/* traverse the binary tree depth first search*/

t_stack *new_node(t_stack **head, t_tree *addr)
{
    t_stack *new;
    t_stack *tmp;

    if (*head == NULL)
    {
        new = malloc(sizeof(t_stack));
        if (!new)
            return (NULL);
        new->next = NULL;
        new->addr = addr;
        *head = new;
        return (new);
    }
    new = malloc(sizeof(t_stack));
    if (!new)
        return (NULL);
    new->next = *head;
    new->addr = addr;
    *head = new;
    return (new);
}

t_stack *pop_stack(t_stack **head)
{
    t_stack *tmp;

    if (*head == NULL)
        return (NULL);

    tmp = (*head);
    *head = (*head)->next;
    return (tmp);
}

void pre_order_traversal(t_tree **root)
{
    t_stack *head;
    t_stack *prev;
    t_stack  *current;
    t_stack *new;

    head = NULL;
    prev = NULL;
    if (*root != NULL)
        new_node(&head, *root);
    while (head != NULL)
    {
        current = pop_stack(&head);
        *root = current->addr;
        if ((*root) != NULL)
        {
            printf("%d\n", (*root)->type);
            new_node(&head, (*root)->right);
            new_node(&head, (*root)->left);            
        }
    }
}
