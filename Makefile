CC = gcc
default: fibtest

fibtest: fibtest.o cthread.o
		$(CC) $(CFLAGS) -o fibtest fibtest.o cthread.o

fibtest.o: test.c
		$(CC) $(CFLAGS) -c fibtest.c

cthread.o: cthread.c
		$(CC) $(CFLAGS) -c cthread.c

clean:
		$(RM) fibtest *.o *~