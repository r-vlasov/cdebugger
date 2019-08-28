OBJECTS = debug.o interface.o breakpoint.o registers.o signals.o dwarf.o
CC = gcc
CFLAGS = -lreadline -g -Wall -c

CCPP = g++
CPPFLAGS = -Wall -g -c

BINPATH = ../obj/$@
DIR1 = ds
%.o: %.c
	$(CC) $(CFLAGS) $< -o $(BINPATH)


%.o: %.cpp
	$(CCPP)  $(CPPFLAGS) $< -o $(BINPATH)


run:	$(OBJECTS)
	$(MAKE) run -C $(DIR1)
