# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/30 12:03:18 by labderra          #+#    #+#              #
#    Updated: 2024/10/24 00:42:59 by labderra         ###   ########.fr        #
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
extern_cmd.c \
tools1.c \
tools2.c \
environment.c \
echo.c \
pwd.c \
exit.c \
env.c \
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
	rm -f $(OBJ)
	make -C libft clean

fclean : clean
	rm -f $(NAME)
	make -C libft fclean

re : fclean all

.PHONY : all clean fclean re 
