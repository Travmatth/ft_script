DEBUG =
ifdef TEST
        DEBUG = -g -fsanitize=address -fsanitize=undefined
else
        DEBUG =
endif
ifdef LEAKS
        DEBUG = -g
endif

NAME = ft_script
LIBFT = libftprintf/libftprintf.a
CFLAGS = -Wall -Wextra -Werror -Wpedantic
LDFLAGS = -Llibftprintf -lftprintf -I./includes
IS_DEBUG =
MAIN = src/main.c
TEST_FILE = test/test_utils.c
TEST_NAME = test/test_utils
CORE = parse_args pty manage find lifecycle

FILES = $(addprefix src/, $(CORE))
SRC = $(addsuffix .c, $(FILES))
OBJ = $(SRC:.c=.o)

.PHONY = all clean fclean re

all: $(SUBMODULES) $(NAME)

debug: set-debug all

test_utils: set-debug all
	@echo -n 'Compiling ft_script tests... '
	@$(CC) $(IS_DEBUG) $(DEBUG) $(CFLAGS) $(LDFLAGS) $(OBJ) $(TEST_FILE) -o $(TEST_NAME)
	@echo "\033[32mdone\033[0m"

set-debug:
	@echo 'Compiling ft_script with debugging logs enabled'
	$(eval IS_DEBUG='-D__DEBUG__') 

$(LIBFT):
	@$(MAKE) -C libftprintf

$(OBJ): %.o: %.c
	@$(CC) -c $(IS_DEBUG) $(DEBUG) -I. $(CFLAGS) $< -o $@

$(NAME): $(LIBFT) $(OBJ)
	@echo -n 'Compiling ft_script... '
	@$(CC) $(IS_DEBUG) $(DEBUG) $(CFLAGS) $(LDFLAGS) $(OBJ) $(MAIN) -o $@
	@echo "\033[32mdone\033[0m"

clean:
	@$(MAKE) clean -C libftprintf
	@echo -n 'Cleaning ft_script object files... '
	@rm -rf $(OBJ) *.dSYM test/*.dSYM *.DS_Store typescript
	@echo "\033[32mdone\033[0m"

fclean: clean
	@$(MAKE) fclean -C libftprintf
	@echo -n 'Cleaning ft_script executable... '
	@rm -rf $(NAME) $(TEST_NAME)
	@echo "\033[32mdone\033[0m"

re: fclean all
