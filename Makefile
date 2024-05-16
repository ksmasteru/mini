NAME = pars
CC = cc
#CFLAGS = -Wall -Wextra -Werror

SRCS = tokenization.c makers.c tokens_v2.c parser.c

OBJS = $(SRCS:.c=.o)


all : $(NAME)

.%o :	%.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME) : $(OBJS)
	@$(CC) $(CFLAGS) $^ -o $@

clean :
	@rm -f $(OBJS)

fclean : clean
	@rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean