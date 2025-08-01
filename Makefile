# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/31 13:46:50 by lsurco-t          #+#    #+#              #
#    Updated: 2025/08/01 11:12:28 by lsurco-t         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a
OBJ_DIR = obj
SRC = $(shell find src -name '*.c') #DO NOT FORGET TO CHANGE TO REAL SRC PATH AFTER

OBJ = $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRC))

OBJDIRS := $(sort $(dir $(OBJ)))

all: $(NAME)

$(OBJDIRS):
	@mkdir -p $@

$(LIBFT):
	@make --no-print-directory -C $(LIBFT_DIR)

$(NAME): $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) -lreadline
	@echo "\033[34mCompiling Minishell...\033[0m"
	@echo "\033[32mMinishell built successfully!\033[0m"

$(OBJ_DIR)/%.o: src/%.c | $(OBJDIRS)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@make --no-print-directory -C $(LIBFT_DIR) clean
	@echo "\033[33mMinishell object files cleaned successfully!\033[0m"

fclean: clean
	@rm -f $(NAME)
	@make --no-print-directory -C $(LIBFT_DIR) fclean
	@echo "\033[31mMinishell executable cleaned successfully!\033[0m"

re: fclean all

.PHONY: all clean fclean re