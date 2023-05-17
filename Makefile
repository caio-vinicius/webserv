NAME = webserv

MAIN := main.cpp

METHOD_DIR := src/method

METHOD_SRC := $(METHOD_DIR)/Method.cpp $(METHOD_DIR)/Get.cpp
        $(METHOD_DIR)/Delete.cpp $(METHOD_DIR)/MethodNotAllowed.cpp \
        $(METHOD_DIR)/Post.cpp

SRCS := src/Utils.cpp src/Config.cpp src/Server.cpp src/Response.cpp \
        src/Cgi.cpp $(METHOD_SRC)

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
