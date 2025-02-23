CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = hala

SRC = hala.c miniz.c
OBJ = $(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

#build
build: $(TARGET)
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

#run
run: $(TARGET)
	./$(TARGET)

#clean
clean:
	rm -f $(TARGET) $(OBJ)
