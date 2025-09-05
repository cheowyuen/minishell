# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/31 13:46:50 by lsurco-t          #+#    #+#              #
#    Updated: 2025/09/05 11:46:59 by lsurco-t         ###   ########.fr        #
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
	@echo "📚 Building libft..."
	@make --no-print-directory -C $(LIBFT_DIR)

$(NAME): $(OBJ) $(LIBFT)
	@echo "🔗 Linking minishell..."
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) -lreadline
	@echo "\033[32m🎉 Minishell built successfully!\033[0m"

$(OBJ_DIR)/%.o: src/%.c | $(OBJDIRS)
	@echo "\033[36m⚙️  Compiling $(notdir $<)\033[0m"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@make --no-print-directory -C $(LIBFT_DIR) clean
	@echo "\033[33m🧹 Minishell object files cleaned successfully!\033[0m"

fclean: clean
	@rm -f $(NAME)
	@make --no-print-directory -C $(LIBFT_DIR) fclean
	@echo "\033[31m🗑️  Minishell executable cleaned successfully!\033[0m"

re: fclean all

.PHONY: all clean fclean re