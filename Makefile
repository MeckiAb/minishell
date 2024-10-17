# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/30 12:03:18 by labderra          #+#    #+#              #
#    Updated: 2024/10/18 00:30:06 by labderra         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = clang -Wall -Werror -Wextra -g #-fsanitize=address
LIBFT = libft/libft.a
SRC = main.c \
initialize.c \
signals.c \
lexer.c \
parser.c \
heredoc.c \
exec.c \
tools.c \
environment.c \
builtin.c \
unset.c \
cd.c \
export.c

OBJ = $(SRC:.c=.o)

all : $(NAME)

%.o : %.c 
	$(CC) -c $< -o $@

$(LIBFT):
	make -C libft 
	make -C libft bonus

$(NAME) : $(LIBFT) $(OBJ)
	$(CC) $(OBJ) $(LIBFT) -lreadline -ltermcap -o $@

clean :
	rm $(OBJ)
	make -C libft clean

fclean : clean
	rm $(NAME)
	make -C libft fclean

re : fclean all

test : all clean
	./minishell

.PHONY : all clean fclean re test
