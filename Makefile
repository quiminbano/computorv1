# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: corellan <corellan@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/27 22:14:38 by corellan          #+#    #+#              #
#    Updated: 2024/03/02 16:44:59 by corellan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = computor

SRC = main.cpp InputParser.cpp PolynomConverter.cpp

SRC_BONUS = $(SRC:.cpp=_bonus.cpp)

OBJ = $(SRC:.cpp=.o)

OBJ_BONUS = $(SRC_BONUS:.cpp=.o)

CC = c++

FLAGS = -Wall -Wextra -Werror -std=c++14

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) $(FLAGS) $(OBJ) -o $(NAME)

bonus: .bonus

.bonus: $(OBJ_BONUS)
		$(CC) $(FLAGS) $(OBJ_BONUS) -o $(NAME)
		@touch .bonus

%.o: %.cpp
		$(CC) $(FLAGS) -I. -c $< -o $@

clean:
		rm -f $(OBJ) $(OBJ_BONUS)
		@rm -f .bonus

fclean: clean
		rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re