NAME = webserv

all: $(NAME)

$(NAME): main.cpp
	c++ -Wall -Wextra -Wfatal-errors -std=c++98 -pedantic -pedantic-errors -o $(NAME) main.cpp Config.cpp

clean:
	rm -f $(NAME)

fclean: clean

re: fclean all

config:
	ln -s $(PWD)/.git-hooks/pre-commit $(PWD)/.git/hooks/pre-commit
