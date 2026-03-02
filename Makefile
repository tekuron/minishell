PROJECT = minishell
NAME = minishell

SRC_DIR = src
INC_DIR = inc
INCLUDE = -I $(INC_DIR) -I libft/inc
OBJ_DIR = obj
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3
MAKE_F = make -s --no-print-directory
RM = rm -rf
AR = ar rcs

LIB_DIR = libft
LIB = $(LIB_DIR)/libft.a

GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
CYAN = \033[0;36m

SRCS = $(shell find $(SRC_DIR) -wholename "*/*.c")

OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(NAME)

$(LIB):
	@$(MAKE_F) -C $(LIB_DIR)

$(NAME): $(LIB) $(OBJS)
	@$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) $(LIB) -lreadline -o $(NAME)
	@echo "$(GREEN)$(PROJECT) built: $(NAME)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "$(CYAN)Compiling: $<"
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	@$(RM) $(OBJ_DIR)
	@$(MAKE_F) clean -C $(LIB_DIR)
	@echo "$(BLUE)$(PROJECT) object files removed"

fclean: clean
	@$(RM) $(NAME)
	@$(RM) libft/libft.a
	@echo "$(BLUE)libft clean (library removed)"
	@echo "$(BLUE)$(PROJECT) clean (binary removed)"

re: fclean all
	@echo "$(GREEN)$(PROJECT) rebuilt"

pclean:
	@$(RM) $(OBJ_DIR)
	@echo "$(BLUE)$(PROJECT) non-library object files removed"

pfclean: pclean
	@$(RM) $(NAME)
	@echo "$(BLUE)$(PROJECT) pclean (library removed)"

pre: pfclean all

.PHONY: all clean fclean re