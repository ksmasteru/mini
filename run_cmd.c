#include "tokens.h"
#include "parser.h"

// in which order the execution ?
// traverse tree to the leaf
// left and right words at that should
// left into right

int run_cmd(t_tree **tree)
{
     if ((*tree)->type == PIPE)
            run_exec((*tree)->left, (*tree)->right);
     }
}

// stack ?

// go to leaf

// pop1 pop2.
//  