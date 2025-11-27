NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = main.c \
       utils.c \
       write.c \
       synchro_utils.c \
       dinner.c \
       safe_functions.c \
       getter_setter.c \
       init.c \
       parse.c \
       monitor.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re