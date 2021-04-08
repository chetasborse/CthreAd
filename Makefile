CC = gcc
default: test

test: test.o cthread.o
		$(CC) $(CFLAGS) -o test test.o cthread.o

test.o: test.c
		$(CC) $(CFLAGS) -c test.c

cthread.o: cthread.c cthread.h
		$(CC) $(CFLAGS) -c cthread.c

clean:
		$(RM) test *.o *~