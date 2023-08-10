NAME := Matt_daemon

CC := clang++
FLAGS := -std=c++17 -Wall -Wextra -Werror
OBJDIR := obj/
SRCDIR := src/
SRC := main.cpp Lock_file.cpp Tintin_reporter.cpp Matt_daemon.cpp server.cpp
OBJ := $(addprefix $(OBJDIR), $(SRC:.cpp=.o))
INCLUDE := include/${wildcard *.hpp}

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) -o $(NAME) $(OBJ)

$(OBJ): $(OBJDIR)%.o : $(SRCDIR)%.cpp $(INCLUDE)
	@mkdir -p $(OBJDIR)
	@$(CC) $(FLAGS) -I$(INCLUDE) -o $@ -c $<

clean:
	@rm -f $(OBJ)
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all
