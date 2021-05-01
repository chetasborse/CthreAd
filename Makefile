CC = gcc
default: test1 test2 test3 test4 test5 test6 test7 test8 test9

test1: test1.o cthread.o
		@$(CC) $(CFLAGS) -o test1 test1.o cthread.o

test1.o: ./one-one/tests/test1.c
		@$(CC) $(CFLAGS) -c ./one-one/tests/test1.c

test2: test2.o cthread.o
		@$(CC) $(CFLAGS) -o test2 test2.o cthread.o

test2.o: ./one-one/tests/test2.c
		@$(CC) $(CFLAGS) -c ./one-one/tests/test2.c

test3: test3.o cthread.o
		@$(CC) $(CFLAGS) -o test3 test3.o cthread.o

test3.o: ./one-one/tests/test3.c
		@$(CC) $(CFLAGS) -c ./one-one/tests/test3.c

test4: test4.o cthread.o
		@$(CC) $(CFLAGS) -o test4 test4.o cthread.o

test4.o: ./one-one/tests/test4.c
		@$(CC) $(CFLAGS) -c ./one-one/tests/test4.c

test5: test5.o cthread.o
		@$(CC) $(CFLAGS) -o test5 test5.o cthread.o

test5.o: ./one-one/tests/test5.c
		@$(CC) $(CFLAGS) -c ./one-one/tests/test5.c

test6: test6.o cthread.o
		@$(CC) $(CFLAGS) -o test6 test6.o cthread.o

test6.o: ./one-one/tests/test6.c
		@$(CC) $(CFLAGS) -c ./one-one/tests/test6.c

test7: test7.o cthread.o
		@$(CC) $(CFLAGS) -o test7 test7.o cthread.o

test7.o: ./one-one/tests/test7.c
		@$(CC) $(CFLAGS) -c ./one-one/tests/test7.c

test8: test8.o cthread.o
		@$(CC) $(CFLAGS) -o test8 test8.o cthread.o

test8.o: ./one-one/tests/test8.c
		@$(CC) $(CFLAGS) -c ./one-one/tests/test8.c

test9: ./one-one/tests/test9.c
		@$(CC) $(CFLAGS) ./one-one/tests/test9.c -o test9

cthread.o: ./one-one/cthread.c ./one-one/cthread.h
		@$(CC) $(CFLAGS) -c ./one-one/cthread.c

run: default
		@bash ./test.sh

clean:
		@$(RM) test1 test2 test3 test4 test5 test6 test7 test8 test9 test *.o *~
