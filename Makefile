MAKEFLAGS += --no-print-directory

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g

SRC_DIR = source
OBJ_DIR = objects
LIB_DIR = library

LIBFT_DIR = $(LIB_DIR)/libft
GNL_DIR   = $(LIB_DIR)/gnl
LEXICAL   = $(SRC_DIR)/lexical_analysis
SYNTAX    = $(SRC_DIR)/syntax_analysis
PARSER    = $(SRC_DIR)/parser
BUILTIN   = $(SRC_DIR)/builtin

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
	$(PARSER)/env_init.c \
	$(PARSER)/env_init_utils.c \
	$(PARSER)/print.c \
	$(PARSER)/utils.c \
	$(BUILTIN)/builtins.c \
	$(BUILTIN)/export_builtin.c \
	$(BUILTIN)/unset_builtin.c \
	$(BUILTIN)/env_builtin.c \
	$(BUILTIN)/pwd_builtin.c \
	$(BUILTIN)/cd_builtin.c 


OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

LIBS = -lreadline

$(NAME): $(OBJS) $(LIBFT_DIR)/libft.a
	@echo Linking $(NAME)
	@$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBFT_DIR)/libft.a $(LIBS) -o $(NAME)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@echo Compiling $<
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(LIBFT_DIR)/libft.a:
	@$(MAKE) -C $(LIBFT_DIR)

all: $(NAME)

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	@rm -rf $(OBJ_DIR)

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
