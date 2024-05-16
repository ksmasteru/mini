#ifndef PARSER_H
#define PARSER_H


#include "tokens.h"

t_tree *parser(t_token *head);
t_tree *parse_complete_cmd(t_token **head);
t_tree *parse_and_or(t_token **head);
t_tree *parse_pipe_line(t_token **head);
t_tree *parse_command(t_token **head);
t_tree *parse_simple_command(t_token **head);
t_tree *parse_redirect(t_token **head);
t_tree *parse_word(t_token **head);
t_tree *parse_word(t_token **head);
t_tree *make_exec_command(t_token *word);
t_tree *make_great_command(t_tree *left, t_tree *right);
t_tree *make_or_command(t_tree *left, t_tree *right);
t_tree *make_and_new(t_tree *left, t_tree *right);
t_tree *make_pipe_cmd(t_tree *left, t_tree *right);

#endif