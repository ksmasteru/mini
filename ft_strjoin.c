/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hes-saqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 13:31:53 by hes-saqu          #+#    #+#             */
/*   Updated: 2024/03/05 13:31:54 by hes-saqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"


int	ft_putstr_fd(int fd, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write(fd, str + i, 1);
		i++;
	}
	return (i);
}

char	*ft_strdup2(char *s1)
{
	char	*str;
	int		size;
	int		i;

	i = 0;
	size = ft_strlen(s1);
	str = (char *)malloc(sizeof(char) * (size + 1));
	if (!str)
		return (NULL);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_strjoin(char *s1, char *s2)
{
	int		s1_lent;
	int		s2_lent;
	char	*str;

	if (s1 == NULL)
		return (ft_strdup2(s2));
	if (s2 == NULL)
		return (ft_strdup2(s1));
	s1_lent = ft_strlen(s1);
	s2_lent = ft_strlen(s2);
	str = (char *)malloc(sizeof(char) * (s1_lent + s2_lent + 1));
	if (!str)
		return (NULL);
	return (ft_strjoin2(s1, s2, str));
}

char	*ft_strjoin2(char *s1, char *s2, char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		str[i + j] = s2[j];
		j++;
	}
	str[i + j] = '\0';
	return (str);
}