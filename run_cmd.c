#include "tokens.h"
#include "parser.h"
#include "pipes.h"
#include <fcntl.h>
#define READER 0
#define WRITER 1
#define NEUTER 2

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

int execute_cmd(t_tree *head, int **fdx, int index, int len, int *pids)
{
     int pid;
     char buffer[1024];
     char **args;
     char *cmd;
     if (index == 0)
     {
          args = (char*[]){"ls", "-la", NULL};
          cmd = "/bin/ls";
     }
     if (index == 1 || index == 2 || index == 3)
     {
          args = (char*[]){"ls", "-la", NULL};
          cmd = "/bin/ls";
     }
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

void run_cmd(t_tree **head, int **fd, int index, int len, int *pids)
{
     if (*head == NULL)  
          return ;
     if ((*head)->type == WORD)
          return ;
     run_cmd(&((*head)->left), fd, index - 1, len, pids);
     if ((*head)->left->type == WORD) 
     {
          execute_cmd((*head)->left, fd, index - 1, len, pids);
          execute_cmd((*head)->right, fd, index, len, pids);
     }
     else 
          execute_cmd((*head)->right, fd, index, len, pids);
}