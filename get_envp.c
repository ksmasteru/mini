/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_envp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hes-saqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 13:32:34 by hes-saqu          #+#    #+#             */
/*   Updated: 2024/03/05 13:32:35 by hes-saqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"

int	ft_strcmp(char *s1, char *s2)
{
	unsigned int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0')
	{
		i++;
	}
	if (s1[i] == s2[i])
	{
		return (0);
	}
	else
	{
		return (s1[i] - s2[i]);
	}
}

int	ft_strncmp(char *s1, char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n <= 0 || (s1[i] == '\0' && s2[i] == '\0'))
		return (0);
	while (i < n)
	{
		if (s1[i] != s2[i] || s1[i] == '\0' || s2[i] == '\0')
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

int	ft_strncmp_n(char *str, char *str2)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '\n' && str2[i])
	{
		if (str[i] != str2[i])
			return (-1);
		i++;
	}
	if (str[i] == '\n' && str2[i] == '\0')
		return (0);
	return (-1);
}

char	**get_envp(char **env)
{
	int		i;
	char	**envp;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH", 4) == 0)
			break ;
		i++;
	}
	if (env[i] == NULL)
	{
		write(1, "no available environment\n", 27);
		return (NULL);
	}
	envp = ft_split(env[i] + 5, 58);
	return (envp);
}