
NAME = mini_serv
SRC = mini_serv.c
OBJ = $(SRC:.c=.o)

$(NAME): $(OBJ)
	cc $(OBJ) -o $@

%.o: %.c
	cc -Wall -Werror -Wextra -c $< -o $@

all: $(NAME)

clean:
	rm rf $(NAME)