#ifndef TOKENS_H
#define TOKENS_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum s_token_type{
    ERROR = 0,
    WORD = 1,
    SPECIAL = 2,
    PIPE = 3,
    AMPER = 4,
    GREAT = 6,
    PAREN_L = 7,
    PAREN_R = 8,
    OR = 9,
    AND = 10,
    WORDS = 11,
    HEREDOC = 12,
    LESS = 13,
    FILE_NAME = 14,
    GREATGREAT = 15,
}t_token_type;

typedef struct s_slice{
    char *location;
    size_t lenght;
    struct s_slice *next;
}t_slice;

typedef struct s_token{
    t_token_type type;
    t_slice location;
    struct s_token *next;
    struct s_token *up;
}t_token;

typedef struct s_tree{
    t_token_type type;
    t_token *token;
    struct s_tree *left;
    struct s_tree *right;
}t_tree;

typedef struct s_queue{
    t_tree *addr;
    struct s_queue *next;
}t_queue;

typedef struct s_data{
    char *str;
    t_tree **head;
    int **fdx;
    int index;
    int len;
    int words_count;
    int *pids;
    char **envp;
    char **env; 
}t_data;

t_token *make_new_node(t_token_type type, char *start, size_t length);
void add_new_token(t_token **head , int c, char *start, size_t length);
t_token  *lexer(char *str);
t_tree *make_pipe_cmd(t_tree *left, t_tree *right);
t_tree *make_and_cmd(t_tree *left, t_tree *right);
t_tree *make_or_command(t_tree *left, t_tree *right);
t_tree *make_great_command(t_tree *left, t_tree *right);
t_tree *make_exec_command();
void    tokens_v2(t_token **tokens);
void	*m_alloc(size_t __size, char todo);
void merge_words(t_token **current, t_token **next);
void token_overdrive(t_token **tokens);
void left_root_right(t_tree **root);
#endif