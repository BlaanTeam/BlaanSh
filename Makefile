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

UTILS_FILES = utils/terminal.c \
		utils/io_redir.c \
		utils/error.c \
		utils/env_init.c \
		utils/env_export.c \
		utils/env_lookup.c \
		utils/runtime.c \
		utils/xalloc.c \
		utils/status.c \
		utils/posix_wrap.c \
		utils/debug_print.c

BUILTINS_FILES = builtins/cd_cmd.c \
		builtins/pwd_cmd.c \
		builtins/echo_cmd.c \
		builtins/exit_cmd.c \
		builtins/unset_cmd.c \
		builtins/env_cmd.c \
		builtins/export_cmd.c

TOKENIZER_FILES = tokenizer/lexer.c \
		tokenizer/tokenizer.c \
		tokenizer/tokenizer_quotes.c \
		tokenizer/list_create.c \
		tokenizer/list_delete.c

EXECUTOR_FILES = executor/ft_execvp.c \
		executor/exec_command.c \
		executor/executor.c \
		executor/run_connectors.c \
		executor/run_commands.c

PARSER_FILES = parser/astree_constructors.c \
		parser/parser_helpers.c \
		parser/grammar.c \
		parser/parse_redir.c \
		parser/parser.c

SYNTAX_FILES = syntax_analyser/syntax_analyser.c \
		syntax_analyser/token_neighbors.c

EXPANDER_FILES = expander/expander.c \
		expander/expand_tokens.c \
		expander/expand_groups.c \
		expander/wildcard_matching.c

FILES =	minishell.c \
	$(UTILS_FILES) \
	$(BUILTINS_FILES) \
	$(TOKENIZER_FILES) \
	$(SYNTAX_FILES) \
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
