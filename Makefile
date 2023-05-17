NAME = webserv


MAIN := main.cpp
SRCS := Utils.cpp Config.cpp Server.cpp Method.cpp Response.cpp Cgi.cpp Get.cpp Delete.cpp MethodNotAllowed.cpp Post.cpp

CFLAGS := -Wall -Wextra -Wfatal-errors -std=c++98 -pedantic -g -pedantic-errors

CC := c++

all: $(NAME)

$(NAME): $(SRCS)
	$(CC) $(CFLAGS) -o $(NAME) $(MAIN) $(SRCS)

clean:
	rm -f $(NAME)

fclean: clean

re: fclean all

config:
	ln -s $(PWD)/.git-hooks/pre-commit $(PWD)/.git/hooks/pre-commit
