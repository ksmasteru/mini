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
     else if (index == len - 1)
     {
          dup2(p[index - 1][0], 0);
          close(p[index - 1][0]);
          dup2(p[index][1], 1);
          close(p[index][1]);
     }
     else
     {
          if(dup2(p[index - 1][0], 0) < 0)
               perror("error");
          close(p[index - 1][0]);
          if (dup2(p[index][1] , 1) < 0)
               perror("error");
          close(p[index][1]);
     }
	return (0);
}

int	fill_pipes(int ***p, int n)
{
	int	i;

	i = 0;
     printf("number o pipes is %d", n);
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

int close_all_pipes(int **pfx, int len)
{
     int i;

     //printf("len - 1 is %d", len - 1);
     i = 0;
     while (i < len - 1)
     {
          close(pfx[i][0]);
          close(pfx[i][1]);
          i++;
     }
     return (0);
}
int close_and_dup2(int **pfx, int index, int len)
{
     if (index == 0)
          dup2(pfx[0][1], 1);
     else if (index == len - 1)
          dup2(pfx[index - 1][0], 0);
     else if (index != 0 && index != len - 1)
     {
          dup2(pfx[index - 1][0], 0);
          dup2(pfx[index][1], 1);
     }
     close_all_pipes(pfx, len);
     return (0);
}
int run_cmd_main(char **args, char *cmd, int index, t_data *data)
{
     int i;

     i = 0;
     //waitpid(data->pids[index - 1], NULL, 0); not waiting always work
     //printf("succesugly running run_cmd_main\n");
     // if there is a redirection.
     if (data->words_count != 1)
          close_and_dup2(data->fdx, index, data->words_count);
     if (execve(cmd, args, data->envp) < 0)
          perror("execve");
     exit(0);
}

char *get_file_name(char *location, size_t lent)
{
     char *file_name;

     file_name = (char *)malloc(sizeof(char) * (lent + 1));
     if (!file_name)
          return (NULL);
     file_name[lent] = '\0';
     strncpy(file_name, location, lent);
     return (file_name);
}
int do_redirect(t_token *redirection)
{
     t_token *temp;
     char *file_name;
     int fd;

     temp = redirection;
     if (temp == NULL)
          return (1);
     file_name = get_file_name(temp->location.location, temp->location.lenght);
     if (!file_name)
          perror("filename");
     printf("filname is %s and temp type is %d\n", file_name, temp->type);
     if (temp->type == IN_FILE)
     {
          fd = open(file_name, O_RDONLY);
          if (fd == -1)
          {
               perror("open");
               exit (1);
          }
          dup2(fd, 0);
          close(fd);  
     }
     else if (temp->type == OUT_FILE)
     {
          fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0644);
          if (fd == -1)
               perror("open");
          dup2(fd, 1);
          close(fd);
     }
     return (0);
}

int manage_redirections(t_token *redirection)
{
     // WORD 
     // down
     // REDIRECTION --> UP TOKEN OF WHICH FILE IT HOLDS .// APPEND ?
     //
    t_token *temp;

    temp = redirection;
    while (temp)
    {
          if (do_redirect(temp->up) != 0)
               return (1);
          temp = temp->down;
    }
     return (0);
}
//" < infile cmd segv"
int execute_cmd(t_tree *head, int index, int len, t_data *data)
{
     char **args;
     char *cmd;
     int i = 0;
     args = get_word_args(head); 
     if (!args)
          perror("args");
     cmd = get_path(data->env, args[0]);
     if (head->token->down != NULL)
          manage_redirections(head->token->down); //return 1 on error
     if (index == len - 1 || len == 1)
          return (run_cmd_main(args, cmd, index, data));
     data->pids[index] = fork();
     if (data->pids[index] == 0)
     {
          if (index != 0)
          {
                while (i <= index - 1)
                    waitpid(data->pids[i++], NULL, 0);
          }
          //printf("process of index %d and pid %d\n", index, getpid());
          close_and_dup2(data->fdx, index, data->words_count);
          if (execve(cmd, args, data->envp) < 0)
               perror("execve");
          exit(0);
     }
     else
          return (0);
}

void run_cmd(t_tree **head, int index, int len, t_data *data)
{
  
     if (*head == NULL)  
          return ;
     if ((*head)->type == WORD)
          return ;
     run_cmd(&((*head)->left), index - 1, len, data);
     if ((*head)->left->type == WORD) 
     {
          execute_cmd((*head)->left, index - 1, len, data);
          execute_cmd((*head)->right, index, len, data);
     }
     else 
          execute_cmd((*head)->right, index, len,data);
}