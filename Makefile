# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: corellan <corellan@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/27 22:14:38 by corellan          #+#    #+#              #
#    Updated: 2024/02/27 22:18:56 by corellan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = computor

SRC = main.cpp InputParser.cpp

OBJ = $(SRC:.cpp=.o)

CC = c++

FLAGS = -Wall -Wextra -Werror -std=c++14

