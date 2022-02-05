NAME=minishell
CC=cc
CFLAGS=-Wall -Wextra -Werror 
INCLUDE=include/
HEADER=$(addprefix $(INCLUDE), minishell.h)
SRC=src/
FILES=minishell.c \
	termios_utils.c \
	error_utils.c \
	env_utils1.c \
	env_utils2.c \
	global_utils.c \
	list_utils.c \
	tokenizer.c \
	gc.c 
OBJS=$(FILES:%.c=%.o)
OBJS:=$(addprefix $(SRC), $(OBJS))
RL_DIR=$(addprefix $(shell brew --prefix readline), /)

all: $(NAME)

debug: CFLAGS+=-g
debug: clean $(NAME)

$(NAME): $(OBJS) libft/libft.a
	$(CC) $(CFLAGS) $^ -o $@ -lreadline -L$(addprefix $(RL_DIR), lib)

libft/libft.a:
	make -C libft/ all
	make -C libft/ clean

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@  -I $(INCLUDE) -I $(addprefix $(RL_DIR), include) -I libft/

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all