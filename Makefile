# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: labderra <labderra@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/30 12:03:18 by labderra          #+#    #+#              #
#    Updated: 2024/10/23 14:30:03 by labderra         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = clang -Wall -Werror -Wextra -g -fsanitize=address
LIBFT = libft/libft.a
SRC = main.c \
initialize.c \
signals.c \
free.c \
lexer.c \
words.c \
parser.c \
heredoc.c \
redirections.c \
exec.c \
tools1.c \
tools2.c \
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
