#
# Macros 
#

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror 
SRC = src/
INCLUDE = include/
HEADER = minishell.h executor.h parser.h lexer.h
HEADER := $(addprefix $(INCLUDE), $(HEADER))

UTILS_FILES = utils/io_utils1.c \
		utils/io_utils2.c \
		utils/error_utils.c \
		utils/env_utils1.c \
		utils/env_utils2.c \
		utils/env_utils3.c \
		utils/global_utils1.c \
		utils/global_utils2.c \
		utils/global_utils3.c \
		utils/global_utils4.c \
		utils/display_utils.c

BUILTINS_FILES = builtins/cd_cmd.c \
		builtins/pwd_cmd.c \
		builtins/echo_cmd.c \
		builtins/exit_cmd.c \
		builtins/unset_cmd.c \
		builtins/env_cmd.c \
		builtins/export_cmd.c

TOKENZIER_FILES = tokenizer/tokenizer.c \
		tokenizer/lexer.c \
		tokenizer/list_constructor_utils.c \
		tokenizer/list_destructor_utils.c

EXECUTOR_FILES = executor/ft_execvp.c \
		executor/exec_utils.c \
		executor/executor.c \
		executor/executor_utils1.c \
		executor/executor_utils2.c

PARSER_FILES = parser/astree_constructors.c \
		parser/parser_helpers.c \
		parser/parser_utils1.c \
		parser/parser_utils2.c \
		parser/parser.c \
		syntax_analyser/syntax_analyser.c

EXPANDER_FILES = expander/expander.c \
		expander/expander_utils1.c \
		expander/expander_utils2.c \
		expander/wildcard_matching.c

FILES =	minishell.c \
	$(UTILS_FILES) \
	$(BUILTINS_FILES) \
	$(TOKENZIER_FILES) \
	$(EXECUTOR_FILES) \
	$(PARSER_FILES) \
	$(EXPANDER_FILES) \
		
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

# libft
$(LIBFT): $(addprefix $(LIBFT_PATH), libft.h)
	make -C $(LIBFT_PATH) all clean

# libgc
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
