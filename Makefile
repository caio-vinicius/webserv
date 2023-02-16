NAME = webserv

MIN_CLANG_VERSION := 12

CLANG_VERSION := $(shell c++ --version | grep -o 'version [0-9]*' | grep -o '[0-9]*')

ifneq ($(shell echo "$(CLANG_VERSION) >= $(MIN_CLANG_VERSION)" | bc),1)
	$(error Clang version $(CLANG_VERSION) is not supported. Please install Clang $(MIN_CLANG_VERSION) or later.)
endif

$(NAME): main.cpp
	c++ -Wall -Wextra -Werror -Wfatal-errors -std=c++98 -pedantic -pedantic-errors -o $(NAME) main.cpp

config:
	ln -s $(PWD)/.git-hooks/pre-commit $(PWD)/.git/hooks/pre-commit
