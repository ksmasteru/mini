
#include "tokens.h"

t_token_type decode_type(char c1, char c2)
{
	if (c1 == '|' && c2 != '|')
		return (PIPE);
	else if (c1 == '|' && c2 == '|')
		return (OR);
	else if (c1 == '<' && c2 != '<')
		return (LESS);
	else if (c1 == '<' && c2 == '<')
		return (HEREDOC);
	else if (c1 == '>' && c2 != '>')
		return (GREAT);
	else if (c1 == '>' && c2 == '>')
		return (REDIRECT);
	else if (c1 == '&' && c2!= '&')
		return (AMPER);
	else if (c1 == '&' && c2 == '&')
		return (AND);
	else if (c1 == '(')
		return (PAREN_L);
	else if (c1 == ')')
		return (PAREN_R);
	return (WORD);
}

t_token	*make_new_node(t_token_type type, char *start, size_t length)
{
	t_token *new;

	new = malloc(sizeof(t_token));
	if (!new)
		exit(EXIT_FAILURE);
	new->type = type;
	new->location = (t_slice){start, length};
	new->next = NULL;
	return (new);
}

int	add_op_token(t_token **head, int c1, int c2, char *start)
{
	t_token_type			type;
	t_token         *new;
	t_token         *last;
	size_t          length;

	last = *head;
	type = decode_type(c1, c2);
	length = (1 * (type == HEREDOC || type == AND \
		|| type == OR || type == REDIRECT)) + 1;
	if (!*head)
	{
		*head = make_new_node(type, start, length);
		return (length);
	}
	new = make_new_node(type, start, length);
	if (!new)
		exit(EXIT_FAILURE);
	while (last->next)
		last = last->next;
	last->next = new;
	return (length);
}

void	add_word_token(t_token **head, char *start, size_t word$ize)
{
	t_token_type			type;
	t_token         *new;
	t_token         *last;

	last = *head;
	if (!*head)
	{
		*head = make_new_node(WORD, start, word$ize);
		return ;
	}
	new = make_new_node(WORD, start, word$ize);
	if (!new)
		exit(EXIT_FAILURE);
	while (last->next)
		last = last->next;
	last->next = new;
}
int is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\v' || c == '\r');
}
int is_op(char c)
{
	return (c == '|' || c == '>' || c == '<' || c == '&' || c == '(' || c == ')');
}
t_token  *lexer(char *input)
{
	char	*start;
	t_token	*head;
	size_t	word$ize;

	head = NULL;
	while (*input)
	{
		word$ize = 0;
		while (*input && is_space(*input))
			input++;
		start = input;
		if (*input && is_op(*input))
			input += add_op_token(&head, *input, *(input + 1), start);
		start = input;
		while (*input && (!is_space(*input) && !is_op(*input)))
		{
			input++;
			word$ize++;
		}
		if ((word$ize != 0))
			add_word_token(&head, start, word$ize);
	}
	return (head);
}

 int main(int ac, char **av)
 {
 	char *input;
 	if (ac != 2)
 		return (0);
 	input = av[1];
 	t_token *tokens;
 	t_token *tmp;

 	tokens = lexer(input);
	tokens_v2(tokens);
	
 }