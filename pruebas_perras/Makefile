NAME = pruebas
CC = clang -Wall -Werror -Wextra -g #-fsanitize=address
LIBFT = ../libft/libft.a
SRC = pruebas_perras.c

OBJ = $(SRC:.c=.o)

all : $(NAME)

%.o : %.c 
	$(CC) -c $< -o $@

$(LIBFT):
	make -C ../libft 
	make -C ../libft bonus

$(NAME) : $(LIBFT) $(OBJ)
	$(CC) $(OBJ) $(LIBFT) -lreadline -o $@

clean :
	rm $(OBJ)
	make -C ../libft clean

fclean : clean
	rm $(NAME)
	make -C ../libft fclean

re : fclean all

test : all clean
	./pruebas

.PHONY : all clean fclean re test
