PROJECT = libft
NAME = libft.a
SRC_DIR = src
INC_DIR = inc
INCLUDE = -I $(INC_DIR)
OBJ_DIR = obj

CC = gcc
CFLAGS = -Wall -Wextra -Werror
RM = rm -rf
AR = ar rcs

GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
CYAN = \033[0;36m

SRCS = $(shell find $(SRC_DIR) -name "*.c")

OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	@$(AR) $(NAME) $(OBJS)
	@echo "$(GREEN)$(PROJECT) built: $(NAME)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "$(CYAN)Compiling: $<"
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	@$(RM) $(OBJ_DIR)
	@echo "$(BLUE)$(PROJECT) object files removed"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(BLUE)$(PROJECT) clean (library removed)"

re: fclean all
	@echo "$(GREEN)$(PROJECT) rebuilt"

.PHONY: all clean fclean re