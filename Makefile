NAME = libftprintf.a

CFLAGS = -Wall -Werror -Wextra
OBJS = ft_printf.o

all: $(NAME)
clean:
	rm -f $(OBJS)
fclean: clean
	rm -f $(NAME)
re:
	$(MAKE) fclean
	$(MAKE) all
.PHONY: all clean fclean re

$(NAME): $(OBJS)
	ar cr $@ $^
