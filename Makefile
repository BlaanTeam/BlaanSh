NAME=minishell
CC=cc
CFLAGS=-Wall -Wextra -Werror 
INCLUDE=include/
HEADER=$(addprefix $(INCLUDE), minishell.h)
SRC=src/
FILES=minishell.c \
	gc.c
OBJS=$(FILES:%.c=%.o)
OBJS:=$(addprefix $(SRC), $(OBJS))
RL_DIR=$(addprefix $(shell brew --prefix readline), /)

all: $(NAME)

debug: CFLAGS+=-g
debug: clean $(NAME)

$(NAME): $(OBJS) 
	$(CC) $(CFLAGS) $^ -o $@ -lreadline -L$(addprefix $(RL_DIR), lib)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@  -I $(INCLUDE) -I $(addprefix $(RL_DIR), include)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all