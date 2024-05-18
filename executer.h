#ifndef EXECUTER_H
#define EXECUTER_H

#include "parser.h"
#include "tokens.h"

char    *ft_strdup2(char *s1);
char	**get_paths(int ac, char ***ultimate, char **envp);
char	*get_path(char **paths, char *cmd);
void	free_it(char ***ultimate, char **paths, int **p, int n);
void	free_it_2(int **p, int n);
int	count_words(char *cmd, char sep);
char	*make_words(char *str, char sep);
char	**ft_split(char *cmd, char sep);
void	free_splited(char ***res, int i);
char	***split_all(int ac, char **av);
char	**get_envp(char **env);
int	ft_strncmp_n(char *str, char *str2);
int	ft_strncmp(char *s1, char *s2, size_t n);
int	ft_strcmp(char *s1, char *s2);
int ft_strlen(char *str);
int	ft_putstr_fd(int fd, char *str);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_strjoin2(char *s1, char *s2, char *str);

#endif