#
# Names 
#
NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror 
INCLUDE = include/
HEADER = $(addprefix $(INCLUDE), minishell.h)
SRC = src/
FILES =	minishell.c \
		utils/termios_utils.c \
		utils/error_utils.c \
		utils/env_utils1.c \
		utils/env_utils2.c \
		utils/env_utils3.c \
		utils/global_utils1.c \
		utils/global_utils2.c \
		utils/global_utils3.c \
		tokenizer/tokenizer.c \
		tokenizer/lexer.c \
		tokenizer/list_constructor_utils.c \
		tokenizer/list_destructor_utils.c \
		syntax_analyser/syntax_analyser.c \
		expander/expander.c \
		builtin_commands/cd_cmd.c \
		builtin_commands/pwd_cmd.c \
		builtin_commands/echo_cmd.c \
		builtin_commands/exit_cmd.c \
		builtin_commands/unset_cmd.c \
		builtin_commands/env_cmd.c \
		builtin_commands/export_cmd.c \
		executor/ft_execvp.c \
		executor/exec_utils.c \
		utils/display_utils.c
OBJS = $(FILES:%.c=%.o)
OBJS := $(addprefix $(SRC), $(OBJS))
RL_DIR = $(addprefix $(shell brew --prefix readline), /)
LIBFT_PATH = libft/
LIBFT = $(addprefix $(LIBFT_PATH), libft.a)
LIBGC_PATH = libgc/
LIBGC = $(addprefix $(LIBGC_PATH), libgc.a)

#
# Rules
#

# All
all: $(NAME)

# Debug
debug: CFLAGS+=-g
debug: clean $(NAME)

# minishell
$(NAME): $(LIBFT) $(LIBGC)
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lreadline -L$(addprefix $(RL_DIR), lib)

# libft and libgc
$(LIBFT): $(addprefix $(LIBFT_PATH), libft.h)
	make -C $(LIBFT_PATH) all clean
$(LIBGC): $(addprefix $(LIBGC_PATH), include/gc.h)
	make -C $(LIBGC_PATH) all clean

# object files
%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@  -I $(INCLUDE) -I $(addprefix $(RL_DIR), include) -I $(LIBFT_PATH) -I $(addprefix $(LIBGC_PATH), include)

# clean
clean:
	rm -f $(OBJS)

# full clean
fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_PATH) fclean
	make -C $(LIBGC_PATH) fclean

# remake
re: fclean all
