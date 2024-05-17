#include "tokens.h"
#include "parser.h"
#include "pipes.h"
#define READER 0
#define WRITER 1
#define NEUTER 2

int close_pipes(int **p, int index, int len)
{
     // close all pipes except the previous read and the current write
     int i;

     i = 0;
     if (index == 0)
     {
          while(i < len)/* or equal ?*/
          {
               close(p[i][0]);
               if (i != 0)
                    close(p[i][1]);
               i++;
          }
     }
     else
     {
          while (i < len)
          {
               if (i != index - 1)
                    close(p[i][0]);
               if (i != index)
                    close(p[i][1]);
               i++;
          }
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
          dup2(p[0][1], 1);
          close(p[0][1]);
     }
     else
     {
          dup2(p[index - 1][0], 0);
          close(p[index - 1][0]);
          dup2(p[index][1] , 1);
          close(p[index][1]);
     }
	return (0);
}

int execute_cmd(t_tree *head, int **fdx, int index, int len)
{
     int pid;
     pid = fork();
     if (pid == 0)
     {
          char *args[] = {"ls", "-la", NULL};
          close_and_dup(fdx, index, len);
          printf("process number %d running sir\n", getpid());
          execve("/bin", args, NULL);
          exit(0);
     }
     else
     {
          // shoud also close unsed pipes here
          wait(NULL);
     }
     return (0);  
}

void run_cmd(t_tree **head, int **fd, int index, int len)
{
     if (*head == NULL)  
          return ;
     if ((*head)->type == WORD)
          return ;
     run_cmd(&((*head)->left), fd, index - 1, len);
     if ((*head)->left->type == WORD) 
     {
          execute_cmd((*head)->left, fd, index - 1, len);
          execute_cmd((*head)->right, fd, index, len);
     }
     else 
          execute_cmd((*head)->right, fd, index, len);
}