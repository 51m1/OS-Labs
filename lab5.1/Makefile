
CC = gcc
NAME= ex1 ex2 ex3 ex4 ex5a ex5b ex6

all: $(NAME)

$(NAME): %: %.c
	$(CC) $^ -o $@ 
clean:
	rm -r *.o
