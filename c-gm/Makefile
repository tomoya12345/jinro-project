CC = gcc
CFLAGS = -Wall -I/opt/homebrew/opt/libpq/include -I/usr/include/postgresql
LDFLAGS = -L/opt/homebrew/opt/libpq/lib -lpq
TARGET = gm_server

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c $(LDFLAGS)

clean:
	rm -f $(TARGET)