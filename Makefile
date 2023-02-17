NAME = webserv

$(NAME): main.cpp
	c++ -Wall -Wextra -Werror -Wfatal-errors -std=c++98 -pedantic -pedantic-errors -o $(NAME) main.cpp

config:
	ln -s $(PWD)/.git-hooks/pre-commit $(PWD)/.git/hooks/pre-commit
