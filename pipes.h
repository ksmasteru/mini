#ifndef PIPES_H
#define PIPES_H

#include "tokens.h"
#include "sys/wait.h"
int close_pipes(int **p, int index, int len);
int	close_and_dup(int **p, int index, int len);
int execute_cmd(t_tree *head, int **fdx, int index, int len, int *pids, t_data *data);
void run_cmd(t_tree **head, int **fd, int index, int len, int *pids, t_data *data);
int	fill_pipes(int ***p, int n);
#endif