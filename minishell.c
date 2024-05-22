#include "minishell.h"
#include <asm-generic/signal-defs.h>
#include "parser.h"
#include "pipes.h"
#include "tokens.h"
#include "sys/wait.h"

char*	read_cmd(void);
void	handle_signals(void);


void	handler(int sig)
{
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_signals(void)
{
	signal(SIGQUIT, SIG_IGN); // ignore CTRL + '\'
	signal(SIGTERM, SIG_IGN); // ignore SIGTERM same as BASH 
	signal(SIGINT, handler);
}

char*	read_cmd(void)
{
	char *line;

	line = readline("\e[0;32m[minishell]$ \e[0;0m");
	if (!line)
		exit(-1);
	add_history(line);
	return (line);
}
int main(int ac, char **av, char **envp)
{
	char *line;
	int pid;

	handle_signals();
	while (1)
	{
		line = read_cmd();
		if (line[0] == 'c' && line[1] == 'd' && line[3] == 32)
		{
			printf("jo");
			if (chdir(line + 3) < 0)
				perror("chdir");
			continue;
		}
		if (strcmp(line, "exit") == 0)
			exit(0);
		pid = fork();
		if (pid == 0)			
			parse_cmd(line , envp);
		else
			waitpid(pid, NULL, 0);
	}
	rl_clear_history();
	return (0);
}