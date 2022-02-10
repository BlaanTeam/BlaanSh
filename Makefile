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
	env_utils3.c \
	global_utils.c \
	list_utils.c \
	tokenizer.c \
	builtin_commands/cd_cmd.c \
	builtin_commands/pwd_cmd.c \
	builtin_commands/echo_cmd.c
OBJS=$(FILES:%.c=%.o)
OBJS:=$(addprefix $(SRC), $(OBJS))
RL_DIR=$(addprefix $(shell brew --prefix readline), /)
LIBFT_PATH=libft/
LIBFT=$(addprefix $(LIBFT_PATH), libft.a)
LIBGC_PATH=libgc/
LIBGC=$(addprefix $(LIBGC_PATH), libgc.a)

all: $(NAME)

debug: CFLAGS+=-g
debug: clean $(NAME)

$(NAME): $(LIBFT) $(LIBGC)
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lreadline -L$(addprefix $(RL_DIR), lib)

$(LIBFT): $(addprefix $(LIBFT_PATH), libft.h)
	make -C $(LIBFT_PATH) all clean
$(LIBGC): $(addprefix $(LIBGC_PATH), include/gc.h)
	make -C $(LIBGC_PATH) all clean

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@  -I $(INCLUDE) -I $(addprefix $(RL_DIR), include) -I $(LIBFT_PATH) -I $(addprefix $(LIBGC_PATH), include)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_PATH) fclean
	make -C $(LIBGC_PATH) fclean

re: fclean all