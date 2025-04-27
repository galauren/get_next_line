# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: glaurent <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/25 00:00:00 by glaurent          #+#    #+#              #
#    Updated: 2025/04/26 00:00:00 by glaurent         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = get_next_line.a
NAME_BONUS = get_next_line_bonus.a
NAME_TEST = test_gnl
NAME_TEST_BONUS = test_gnl_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror
AR = ar rcs

SRC = get_next_line.c get_next_line_utils.c
SRC_BONUS = get_next_line_bonus.c get_next_line_utils_bonus.c

OBJ = $(SRC:.c=.o)
OBJ_BONUS = $(SRC_BONUS:.c=.o)

MAIN = main.c
MAIN_BONUS = main_bonus.c
MAIN_OBJ = $(MAIN:.c=.o)
MAIN_BONUS_OBJ = $(MAIN_BONUS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $(NAME) $(OBJ)

bonus: $(OBJ_BONUS)
	$(AR) $(NAME_BONUS) $(OBJ_BONUS)

test: $(NAME) $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $(NAME_TEST) $(MAIN_OBJ) $(NAME)

test_bonus: $(NAME_BONUS) $(MAIN_BONUS_OBJ)
	$(CC) $(CFLAGS) -o $(NAME_TEST_BONUS) $(MAIN_BONUS_OBJ) $(NAME_BONUS)

clean:
	rm -f $(OBJ) $(OBJ_BONUS) $(MAIN_OBJ) $(MAIN_BONUS_OBJ)

fclean: clean
	rm -f $(NAME) $(NAME_BONUS) $(NAME_TEST) $(NAME_TEST_BONUS)

re: fclean all

.PHONY: all bonus clean fclean re test test_bonus
