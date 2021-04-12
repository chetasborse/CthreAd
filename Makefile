CC = gcc
default: test test1 test2 test3 test4 test5 test6 test7 test8 test9

test: test.o cthread.o
		$(CC) $(CFLAGS) -o test test.o cthread.o

test.o: test.c
		$(CC) $(CFLAGS) -c test.c

test1: test1.o cthread.o
		$(CC) $(CFLAGS) -o test1 test1.o cthread.o

test1.o: test1.c
		$(CC) $(CFLAGS) -c test1.c

test2: test2.o cthread.o
		$(CC) $(CFLAGS) -o test2 test2.o cthread.o

test2.o: test2.c
		$(CC) $(CFLAGS) -c test2.c

test3: test3.o cthread.o
		$(CC) $(CFLAGS) -o test3 test3.o cthread.o

test3.o: test3.c
		$(CC) $(CFLAGS) -c test3.c

test4: test4.o cthread.o
		$(CC) $(CFLAGS) -o test4 test4.o cthread.o

test4.o: test4.c
		$(CC) $(CFLAGS) -c test4.c

test5: test5.o cthread.o
		$(CC) $(CFLAGS) -o test5 test5.o cthread.o

test5.o: test5.c
		$(CC) $(CFLAGS) -c test5.c

test6: test6.o cthread.o
		$(CC) $(CFLAGS) -o test6 test6.o cthread.o

test6.o: test6.c
		$(CC) $(CFLAGS) -c test6.c

test7: test7.o cthread.o
		$(CC) $(CFLAGS) -o test7 test7.o cthread.o

test7.o: test7.c
		$(CC) $(CFLAGS) -c test7.c

test8: test8.o cthread.o
		$(CC) $(CFLAGS) -o test8 test8.o cthread.o

test8.o: test8.c
		$(CC) $(CFLAGS) -c test8.c

test9: test9.c
		$(CC) $(CFLAGS) test9.c -o test9

cthread.o: cthread.c cthread.h
		$(CC) $(CFLAGS) -c cthread.c

clean:
		$(RM) test test1 test2 test3 test4 test5 test6 test7 test8 test9 *.o *~