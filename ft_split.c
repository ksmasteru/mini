/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hes-saqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 18:05:37 by hes-saqu          #+#    #+#             */
/*   Updated: 2024/03/08 18:05:39 by hes-saqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"

int	count_words(char *cmd, char sep)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	if (cmd[0] == '\0')
		return (1);
	while (cmd[i])
	{
		while (cmd[i] == sep)
			i++;
		if (cmd[i] != sep && cmd[i])
			count++;
		while (cmd[i] != sep && cmd[i])
			i++;
	}
	return (count);
}

char	*make_words(char *str, char sep)
{
	int		i;
	char	*res;

	i = 0;
	while (str[i] != sep && str[i] != '\0')
		i++;
	res = (char *)malloc(sizeof(char) * (i + 1));
	if (!res)
		return (NULL);
	res[i] = '\0';
	i = 0;
	while (str[i] && str[i] != sep)
	{
		res[i] = str[i];
		i++;
	}
	return (res);
}

char	**ft_split(char *cmd, char sep)
{
	int		words_count;
	char	**args;
	int		i;
	int		x;

	i = 0;
	x = 0;
	words_count = count_words(cmd, sep);
	args = (char **)malloc(sizeof(char *) * (words_count + 1));
	if (!args)
		return (NULL);
	if (cmd[0] == '\0')
		args[x++] = ft_strdup2("");
	while (cmd[i])
	{
		while (cmd[i] == sep && cmd[i] != '\0')
			i++;
		if (cmd[i] != '\0')
			args[x++] = make_words(cmd + i, sep);
		while (cmd[i] != sep && cmd[i])
			i++;
	}
	args[x] = NULL;
	return (args);
}

void	free_splited(char ***res, int i)
{
	int	j;

	j = 0;
	while (j < i)
		free(res[j++]);
	free(res);
}

char	***split_all(int ac, char **av)
{
	char	***res;
	int		i;

	i = 0;
	res = (char ***)malloc(sizeof(char **) * (ac + 1));
	if (!res)
		return (NULL);
	res[ac] = NULL;
	while (i < ac)
	{
		res[i] = ft_split(av[i], 32);
		if (res[i] == NULL)
		{
			perror("malloc");
			free_splited(res, i);
			return (NULL);
		}
		i++;
	}
	return (res);
}