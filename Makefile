NAME = minishell
CC = cc
CFLAGS = -g

SRCS = minishell.c tokenization.c makers.c tokens_v2.c parser.c run_cmd.c get_paths.c \
		 ft_split.c get_envp.c ft_strjoin.c

OBJS = $(SRCS:.c=.o)


all : $(NAME)

%.o :	%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $^ -lreadline -o $@

clean :
	@rm -f $(OBJS)

fclean : clean
	@rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean