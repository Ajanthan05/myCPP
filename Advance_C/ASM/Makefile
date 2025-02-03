CC = gcc
CFLAGS = -z execstack # -m32

TARGET = test
SRC = 3_Shellcode3.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TARGET)
