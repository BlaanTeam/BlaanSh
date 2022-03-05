NAME := libft.a

SRCS := ft_atoi.c ft_isalpha.c ft_isspace.c ft_memcmp.c ft_strchr.c ft_strjoin.c ft_strlen.c ft_strtok.c ft_memmove.c \
		ft_isalnum.c ft_isdigit.c ft_itoa.c ft_putstr_fd.c ft_strdup.c ft_strlcpy.c ft_strncmp.c ft_substr.c


OBJS := $(SRCS:.c=.o)

CFLAGS := -Wall -Wextra -Werror

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	ar -crs $@ $^

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
