CC = gcc
CFLAGS = -g -c -std=c99 -Wall
LDFLAGS = -g
SRC = $(wildcard src/*.c)
HEADER = $(wildcard src/include/*.h)
OBJ = $(SRC:.c=.o)
EXEC = skvm

all: $(EXEC) $(SRC) $(HEADER)

$(EXEC): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf src/*.o $(EXEC) *.skc