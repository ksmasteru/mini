#include "tokens.h"
#include "parser.h"
#include "pipes.h"
#include <fcntl.h>
#define READER 0
#define WRITER 1
#define NEUTER 2

int close_pipes(int **p, int index, int len)
{
     // close all pipes except the previous read and the current write
     int i;

     i = 0;
     // should add another pipe to get info

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
          if (dup2(p[0][1], 1) < 0)
               printf("error");
          close(p[0][1]);
     }
     else
     {
          if(dup2(p[index - 1][0], 0) < 0)
               printf("error");
          close(p[index - 1][0]);
          if (dup2(p[index][1] , 1) < 0)
               printf("error");
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

int custom_close_and_dup(t_tree *head, int **fdx , int index)
{
     if (index == 0)
     {
          close(fdx[0][0]);
          close(fdx[1][0]);
          close(fdx[1][1]);
          close(fdx[2][0]);
          close(fdx[2][1]);
          dup2(fdx[0][1], 1);
          close(fdx[0][1]);
          close(fdx[3][0]);
          close(fdx[3][1]);
     } 
     else if (index == 1)
     {
          close(fdx[0][1]);
          close(fdx[1][0]);
          close(fdx[2][0]);
          close(fdx[2][1]);
          close(fdx[3][0]);
          close(fdx[3][1]);
          dup2(fdx[0][0],0);
          close(fdx[0][0]);
          dup2(fdx[1][1],1);
          close(fdx[1][1]);
     }
     else if (index == 2)
     {
          close(fdx[3][0]);
          close(fdx[3][1]);    
          close(fdx[0][0]);
          close(fdx[0][1]);
          close(fdx[1][1]);
          close(fdx[2][0]);
          dup2(fdx[1][0],0);
          close(fdx[1][0]);
          dup2(fdx[2][1], 1);
          close(fdx[2][1]);
     }
     else if (index == 3)
     {
          int fd = open("new.txt", O_CREAT | O_RDWR, 0777);
          if (fd == -1)
               perror("open");
          close(fdx[0][0]);
          close(fdx[0][1]);
          close(fdx[1][0]);
          close(fdx[1][1]);
          close(fdx[2][1]);
          close(fdx[3][0]);
          close(fdx[3][1]);
          dup2(fdx[2][0],0);
          close(fdx[2][0]);
          dup2(fd, 1);
          close(fd);
     }
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
     if (index == 1)
     {
          args = (char*[]){"ls", "-la", NULL};
          cmd = "/bin/ls";
     }
     //if (index == 2)
     //{
      //    args = (char*[]){"ls", "-la", NULL};
        //  cmd = "/bin/ls";
     //}
     if (index == 3 || index == 2)
     {
          printf("reached index 3\n");
          args = (char*[]){"wc", "-w", NULL};
          cmd = "/usr/bin/wc";  
     }
     pids[index] = fork();     
     if (pids[index] == 0)
     {
          printf("process number %d running sir\n", getpid());
          if (index != 0)
          {
               waitpid(pids[index - 1],NULL, 0);
               printf("previous pid is %d\n", pids[index - 1]);
          }
          custom_close_and_dup(head , fdx, index);
          if (execve(cmd, args, NULL) < 0)
               perror("execve");
          exit(0);
     }
     else
     {
          if (index == 3)
          {
               wait(NULL);
               printf("3iw");
               close(fdx [0][0]);
               close(fdx[0][1]);
               close(fdx[1][0]);
               close(fdx[1][1]);
               close(fdx[2][0]);
               close(fdx[2][1]);
               close(fdx[3][0]);
               close(fdx[3][1]);
               //dup2(fdx[index][0], 0);
               // i think you need another pipe yep.
               //close(fdx[index][0]);
               //int d = read(0, buffer, 100);
               //buffer[d] = '\0';
               //printf("has read %d and it is %s", d, buffer);
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