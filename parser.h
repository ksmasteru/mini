#ifndef PARSER_H
#define PARSER_H


#include "tokens.h"

typedef struct s_stack{
    t_tree *addr;
    struct s_stack *next;
}t_stack;


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
void bfs(t_tree **root);
t_queue *pop(t_queue **head);
t_queue *new_follower(t_queue **head, t_queue **prev, t_tree *addr);
void pre_order_traversal(t_tree **root);
t_stack *pop_stack(t_stack **head);
t_stack *new_node(t_stack **head, t_tree *addr);
#endif