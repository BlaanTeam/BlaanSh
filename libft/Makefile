NAME := libft.a

SRCS := ft_atoi.c    ft_isalpha.c ft_memchr.c  ft_memset.c  ft_strlcat.c ft_strnstr.c	\
		ft_bzero.c   ft_isascii.c ft_memcmp.c  ft_strchr.c  ft_strlcpy.c ft_strrchr.c	\
		ft_calloc.c  ft_isdigit.c ft_memcpy.c  ft_strdup.c  ft_strlen.c  ft_substr.c	\
		ft_isalnum.c ft_isprint.c ft_memmove.c ft_strncmp.c ft_tolower.c ft_toupper.c	\
		ft_strjoin.c ft_strtrim.c ft_split.c  ft_strtok.c ft_itoa.c ft_strmapi.c ft_striteri.c		\
		ft_isspace.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c

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
