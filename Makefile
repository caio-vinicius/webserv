NAME = webserv

all: $(NAME)

SRCS = main.cpp Utils.cpp Config.cpp Server.cpp Method.cpp Response.cpp Cgi.cpp

CFLAGS := -Wall -Wextra -Wfatal-errors -std=c++98 -pedantic -g -pedantic-errors

CC := c++

$(NAME): main.cpp
	$(CC) $(CFLAGS) -o $(NAME) $(SRCS)

clean:
	rm -f $(NAME)

fclean: clean

re: fclean all

config:
	ln -s $(PWD)/.git-hooks/pre-commit $(PWD)/.git/hooks/pre-commit
