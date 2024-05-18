#include "tokens.h"
#include "parser.h"
#include "pipes.h"
#include "executer.h"
#include <fcntl.h>
#define READER 0
#define WRITER 1
#define NEUTER 2



int ft_strlen(char *s1)
{
     int i;

     i = 0;
     while (s1[i])
          i++;
     return (i);
}

int close_pipes(int **p, int index, int len)
{
     int i;

     i = 0;
     while (i < len)
     {
          if (i != 0 && i != index - 1)
               close(p[i][0]);
          if (i != index)
               close(p[i][1]);
          i++;
     }
     return (0);
}

int	close_and_dup(int **p, int index, int len)
{
     int i;

     i = 0;
	close_pipes(p, index, len);
     if (index == 0)
     {
          if (dup2(p[0][1], 1) < 0)
               perror("error");
          close(p[0][1]);
     }
     else if(index == 2 || index == 1)
     {
          if(dup2(p[index - 1][0], 0) < 0)
               perror("error");
          close(p[index - 1][0]);
          if (dup2(p[index][1] , 1) < 0)
               perror("error");
          close(p[index][1]);
     }
     else if (index == 3)
     {
          dup2(p[index - 1][0], 0);
          close(p[index - 1][0]);
          dup2(p[index][1], 1);
          close(p[index][1]);
     }
	return (0);
}

int	fill_pipes(int ***p, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (pipe((*p)[i++]) != 0)
          {
               perror("pipe");
			return (-1);
          }
	}
	return (0);
}

char *slice_and_dice(t_slice slice)
{
     char *res;

     res = malloc(sizeof(char) * (slice.lenght + 1));
     if (!res)
          return (NULL);
     res[slice.lenght] = '\0';
     strncpy(res, slice.location, slice.lenght);
     return(res);
}
char **get_word_args(t_tree *head)
{
     int words_number;
     t_token *tmp;
     char **args;
     int i;

     i = 0;
     words_number = 0;
     tmp = head->token;
     while (tmp)
     {
          words_number++;
          tmp = tmp->up;   
     }
     args = (char **) malloc(sizeof(char *) * (words_number + 1));
     if (!args)
          perror("args");
     tmp = head->token;
     while (i < words_number)
     {
          args[i] = slice_and_dice(tmp->location);
          if (!args[i])
               perror("args[i]");
          i++;
          tmp = tmp->up;
     }
     args[i] = NULL;
     return  (args);
}

int execute_cmd(t_tree *head, int **fdx, int index, int len, int *pids, t_data *data)
{
     int pid;
     char buffer[1024];
     char **args;
     char *cmd;
     int i = 0;
     args = get_word_args(head);
     if (!args)
          perror("args");
     cmd = get_path(data->env, args[0]); // get paths and store it in a struct memeber
     pids[index] = fork();     
     if (pids[index] == 0)
     {
          if (index != 0)
               waitpid(pids[index - 1],NULL, 0);
          close_and_dup(fdx , index, len);
          if (execve(cmd, args, NULL) < 0)
               perror("execve");
          exit(0);
     }
     else
     {
          if (index == 3)
          {
               wait(NULL);
               close(fdx [0][0]);
               close(fdx[0][1]);
               close(fdx[1][0]);
               close(fdx[1][1]);
               close(fdx[2][0]);
               close(fdx[2][1]);
               close(fdx[3][1]);
               dup2(fdx[3][0],0);
               close(fdx[3][0]);
               int d = read(0, buffer, 100);
               buffer[d] = '\0';
               printf("has read %d and value is %s\n", d, buffer);               
          }
     }
     return (0);
}

void run_cmd(t_tree **head, int **fd, int index, int len, int *pids, t_data *data)
{
     if (*head == NULL)  
          return ;
     if ((*head)->type == WORD)
          return ;
     run_cmd(&((*head)->left), fd, index - 1, len, pids, data);
     if ((*head)->left->type == WORD) 
     {
          execute_cmd((*head)->left, fd, index - 1, len, pids, data);
          execute_cmd((*head)->right, fd, index, len, pids, data);
     }
     else 
          execute_cmd((*head)->right, fd, index, len, pids, data);
}