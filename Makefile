NAME = webserv

MAIN := main.cpp

METHOD_DIR := src/method

CONFIG_DIR := src/config

METHOD_SRC := $(METHOD_DIR)/Method.cpp $(METHOD_DIR)/Get.cpp \
		$(METHOD_DIR)/Delete.cpp $(METHOD_DIR)/MethodNotAllowed.cpp \
		$(METHOD_DIR)/Post.cpp

CONFIG_SRC := $(CONFIG_DIR)/Config.cpp $(CONFIG_DIR)/Server.cpp \
		$(CONFIG_DIR)/Location.cpp

SRCS := src/Utils.cpp src/Server.cpp src/Response.cpp \
		src/Cgi.cpp $(METHOD_SRC) $(CONFIG_SRC)

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
