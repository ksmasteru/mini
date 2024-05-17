/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hes-saqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 22:44:58 by hes-saqu          #+#    #+#             */
/*   Updated: 2024/03/06 22:45:01 by hes-saqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <tokens.h>

int	close_all_pipes(int **p, int z)
{
	int	a;
	int	b;
	int	y;

	a = 0;
	b = 0;
	y = 0;
	while (y < z)
	{
		a = close(p[y][0]);
		b = close(p[y][1]);
		if (a == -1 || b == -1)
		{
			perror("pipe");
			return (-1); 
		}
		y++;
	}
	return (0);
}

int	fill_pipes(int ***p, int n, int **pids)
{
	int	i;

	i = 0;
	*p = (int **)malloc(sizeof(int *) * n);
	if (*p == NULL)
		return (-1);
	while (i < n)
	{
		(*p)[i] = (int *)malloc(sizeof(int) * 2);
		if (!(*p)[i])
			return (-1);
		i++;
	}
	i = 0;
	*pids = (int *)malloc(sizeof(int) * (n + 1));
	if (!(*pids))
		return (-1);
	while (i < n)
	{
		if (pipe((*p)[i++]) != 0)
			return (-1);
	}
	return (0);
}

int	close_pipes(int **p, int n, int z)
{
	int	y;
	int	a;
	int	b;

	a = 0;
	b = 0;
	y = 0;
	while (y < z)
	{
		if (y != n - 1)
			if (!(y == 0 && n == 0))
				a = close(p[y][0]);
		if (y != n)
			b = close(p[y][1]);
		if (a == -1 || b == -1)
		{
			perror("pipe");
			return (-1);
		}
		y++;
	}
	return (0);
}

int	close_and_dup(int **p, int i, int n, int *pids)
{
	close_pipes(p, i, n - 1);
	if (i != 0)
	{
		waitpid(pids[i - 1], NULL, 0);
		if (dup2(p[i - 1][0], 0) == -1)
		{
			perror("dup2");
			return (-1);
		}
		close(p[i - 1][0]);
	}
	if (i != n - 1)
	{
		if (dup2(p[i][1], 1) == -1)
		{
			perror("dup2");
			return (-1);
		}
		close(p[i][1]);
	}
	return (0);
}

int	ends_handler(int fd1, int fd2, int i, int n)
{
	if (i == 0)
	{
		if (dup2(fd1, 0) == -1)
		{
			perror("dup2");
			return (-1);
		}
	}
	if (i == n - 1)
	{
		if (dup2(fd2, 1) == -1)
		{
			perror("dup2");
			return (-1);
		}
	}
	return (0);
}