CC = gcc
BIN = sistem.exe
OBJ = HashMap.o main.o

%.o: %.c
	$(CC) -c $< -o $@

$(BIN): $(OBJ)
	$(CC) $^ -o $@
