CC = gcc
default: cthread.o

cthread.o: cthread.c
		$(CC) $(CFLAGS) -c cthread2.c

clean:
		$(RM) fibtest *.o *~