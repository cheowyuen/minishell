# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/31 13:46:50 by lsurco-t          #+#    #+#              #
#    Updated: 2025/07/31 14:05:46 by lsurco-t         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a
OBJ_DIR = obj
SRC = src/*.c
	
OBJ = $(SRC:src/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT)
	@echo "\033[32mMinishell built successfully!\033[0m"

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean
	@echo "\033[33mMinishell object files cleaned successfully!\033[0m"

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean
	@echo "\033[31mMinishell executable cleaned successfully!\033[0m"

re: fclean all

.PHONY: all clean fclean re