OBJECTS = lists.o
CC = gcc
CFLAGS = -lreadline -g -nostartfiles -Wall -c
BINPATH = ../../obj/$@


%.o: %.c
	$(CC) $(CFLAGS) $< -o $(BINPATH)


run: 	$(OBJECTS)
