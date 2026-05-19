NAME = libarena.a
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC = src
INCLUDE = include
CFILES = arena.c
OBJ = $(CFILES:%.c=$(SRC)/%.o)

all : $(NAME)

$(NAME) : $(OBJ)
	ar rcs $@ $^

$(SRC)/%.o : $(SRC)/%.c $(INCLUDE)/arena.h
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INCLUDE)

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY: all clean fclean re
