CC = gcc
default: test2

test2: test1.o cthread.o
		$(CC) $(CFLAGS) -o test2 test1.o cthread.o

test1.o: test1.c
		$(CC) $(CFLAGS) -c test1.c

cthread.o: cthread.c
		$(CC) $(CFLAGS) -c cthread.c

clean:
		$(RM) fibtest *.o *~