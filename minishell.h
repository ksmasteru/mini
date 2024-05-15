#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
# include <signal.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "./libs/libft/libft.h"

# define ALLOC 'A'
# define FREE 'F'

/* The Struct used in the garbage collector */
typedef struct s_gc
{
	void		*ptr;
	struct s_gc	*next;
}				t_gc;

z

void	*m_alloc(size_t __size, char todo);

#endif /*	MINISHELL_H	*/
