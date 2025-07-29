MAKEFLAGS += --no-print-directory

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g3

SRC_DIR = source
OBJ_DIR = objects
LIB_DIR = library

LIBFT_DIR	= $(LIB_DIR)/libft
GNL_DIR		= $(LIB_DIR)/gnl
LEXICAL		= $(SRC_DIR)/lexical_analysis
SYNTAX		= $(SRC_DIR)/syntax_analysis
PARSER		= $(SRC_DIR)/parser
BUILTIN		= $(SRC_DIR)/builtin
ENV			= $(SRC_DIR)/environment
EXECUTION	= $(SRC_DIR)/execution

INCLUDES = -Iincludes -I$(LIBFT_DIR) -I$(GNL_DIR)

SRCS = \
	$(GNL_DIR)/get_next_line.c \
	$(GNL_DIR)/get_next_line_utils.c \
	$(SRC_DIR)/main.c \
	$(LEXICAL)/tokenizer.c \
	$(LEXICAL)/init.c \
	$(LEXICAL)/tokenizer_utils.c \
	$(LEXICAL)/token_quoted.c \
	$(LEXICAL)/utils.c \
	$(SYNTAX)/strict_errors.c \
	$(SYNTAX)/secondary_errors.c \
	$(SYNTAX)/syntax.c \
	$(SYNTAX)/absent_operand.c \
	$(SYNTAX)/quotes_handle.c \
	$(SYNTAX)/heredoc_utils.c \
	$(SYNTAX)/heredoc.c \
	$(SYNTAX)/utils.c \
	$(PARSER)/ast_logic.c \
	$(PARSER)/ast_redirection.c \
	$(PARSER)/ast_parenthesis.c \
	$(BUILTIN)/builtins.c \
	$(BUILTIN)/export_builtin.c \
	$(BUILTIN)/unset_builtin.c \
	$(BUILTIN)/env_builtin.c \
	$(BUILTIN)/pwd_builtin.c \
	$(BUILTIN)/cd_builtin.c \
	$(BUILTIN)/echo_builtin.c \
	$(BUILTIN)/exit_builtin.c \
	$(BUILTIN)/history.c \
	$(ENV)/env_init_utils.c \
	$(ENV)/env_init.c \
	$(ENV)/utils.c \
	$(EXECUTION)/execute_ast.c \
	$(EXECUTION)/execute_command.c \
	$(EXECUTION)/execute_utils.c \
	$(EXECUTION)/execute_convert.c \
	$(EXECUTION)/execute_cmd_path.c \
	$(EXECUTION)/signals.c \
	$(EXECUTION)/wildcard.c \
	$(EXECUTION)/execute_pipe.c \
	$(EXECUTION)/execute_subshell.c \
	$(EXECUTION)/open_redirect.c \
	$(EXECUTION)/wildcard_utils.c 

OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

LIBS = -lreadline

$(NAME): $(OBJS) $(LIBFT_DIR)/libft.a Makefile
	@echo Linking $(NAME)
	@$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBFT_DIR)/libft.a $(LIBS) -o $(NAME)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@echo Compiling $<
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(LIBFT_DIR)/libft.a:
	@$(MAKE) -C $(LIBFT_DIR) bonus

all: $(NAME)

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	@rm -rf $(OBJ_DIR)

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
