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
    REDIRECT = 5,
    GREAT = 6,
    PAREN_L = 7,
    PAREN_R = 8,
    OR = 9,
}t_token_type;

typedef struct s_slice{
    char *location;
    size_t lenght;
}t_slice;

typedef struct s_token{
    t_token_type type;
    t_slice *location;
    struct s_token *next;
}t_token;

typedef struct s_tree{
    t_token *cmd;
    struct s_tree *left;
    struct s_tree *right;
}t_tree;


t_token *make_new_node(t_token_type type, char *start, size_t length);
void add_new_token(t_token **head , int c, char *start, size_t length);
t_token  *lexer(char *str);
t_tree *make_pipe_cmd(t_tree *left, t_tree *right);
t_tree *make_and_cmd(t_tree *left, t_tree *right);
t_tree *make_or_command(t_tree *left, t_tree *right);
t_tree *make_great_command(t_tree *left, t_tree *right);
t_tree *make_exec_command();

