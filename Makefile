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

TOKENZIER_FILES = tokenizer/lexer.c \
		tokenizer/tokenizer.c \
		tokenizer/tokenizer_utils.c \
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
		parser/parser.c

SYNTAX_UTILS = syntax_analyser/syntax_analyser.c \
		syntax_analyser/syntax_utils.c

EXPANDER_FILES = expander/expander.c \
		expander/expander_utils1.c \
		expander/expander_utils2.c \
		expander/wildcard_matching.c

FILES =	minishell.c \
	$(UTILS_FILES) \
	$(BUILTINS_FILES) \
	$(TOKENZIER_FILES) \
	$(SYNTAX_UTILS) \
	$(EXPANDER_FILES) \
	$(PARSER_FILES) \
	$(EXECUTOR_FILES) \
		
OBJS = $(FILES:%.c=%.o)
OBJS := $(addprefix $(SRC), $(OBJS))

# readline: on macOS use the Homebrew prefix, on Linux rely on system paths
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	RL_PREFIX := $(shell brew --prefix readline 2>/dev/null)
	RL_INC := -I$(RL_PREFIX)/include
	RL_LIB := -L$(RL_PREFIX)/lib
else
	RL_INC :=
	RL_LIB :=
endif

LIBFT_PATH = libft/
LIBFT = $(addprefix $(LIBFT_PATH), libft.a)


LIBARENA_PATH = libarena/
LIBARENA = $(addprefix $(LIBARENA_PATH), libarena.a)

#
# Rules
#

# All
all: $(NAME)

# Debug
debug: CFLAGS+=-g
debug: clean $(NAME)

# minishell
$(NAME): $(LIBFT) $(LIBARENA)
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lreadline -lm $(RL_LIB)

# libft
$(LIBFT): $(addprefix $(LIBFT_PATH), libft.h)
	make -C $(LIBFT_PATH) all clean

# libarena
$(LIBARENA): $(addprefix $(LIBARENA_PATH), include/arena.h)
	make -C $(LIBARENA_PATH) all clean

# object files
%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INCLUDE) $(RL_INC) -I $(LIBFT_PATH) -I $(addprefix $(LIBARENA_PATH), include)

# tests
test: $(NAME)
	bash tests/run_tests.sh

# parser fuzz (override via FUZZ_ITER / FUZZ_LINES / FUZZ_LINELEN)
fuzz: $(NAME)
	ITERATIONS=$(FUZZ_ITER) LINES_PER_RUN=$(FUZZ_LINES) LINE_LEN=$(FUZZ_LINELEN) \
		bash tests/fuzz.sh
FUZZ_ITER ?= 100
FUZZ_LINES ?= 30
FUZZ_LINELEN ?= 80

# clean
clean:
	rm -f $(OBJS)

# full clean
fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_PATH) fclean
	make -C $(LIBARENA_PATH) fclean

# remake
re: fclean all

.PHONY: all debug test fuzz clean fclean re
