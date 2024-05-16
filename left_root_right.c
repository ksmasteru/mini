#include "tokens.h"
#include "parser.h"

void left_root_right(t_tree **root)
{
    if (*root == NULL)
        return ;
    left_root_right(&((*root)->left));
    printf("data is %d\n", (*root)->type);
    left_root_right(&((*root)->right));
}