NAME = webserv


MAIN := main.cpp
SRCS := src/Utils.cpp src/Config.cpp src/Server.cpp src/Response.cpp src/Cgi.cpp src/method/Method.cpp src/method/Get.cpp src/method/Delete.cpp src/method/MethodNotAllowed.cpp src/method/Post.cpp

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
