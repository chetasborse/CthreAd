CC = gcc
default: test1 test2 test3 test4 test5 test6 test7 test8 test9 test10 test11 test12 test13 test14 test15 

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

test10: test10.o cthread2.o
		@$(CC) $(CFLAGS) -o test10 test10.o cthread2.o

test10.o: ./many-one/tests/test10.c
		@$(CC) $(CFLAGS) -c ./many-one/tests/test10.c

test11: test11.o cthread2.o
		@$(CC) $(CFLAGS) -o test11 test11.o cthread2.o

test11.o: ./many-one/tests/test11.c
		@$(CC) $(CFLAGS) -c ./many-one/tests/test11.c

test12: test12.o cthread2.o
		@$(CC) $(CFLAGS) -o test12 test12.o cthread2.o

test12.o: ./many-one/tests/test12.c
		@$(CC) $(CFLAGS) -c ./many-one/tests/test12.c

test13: test13.o cthread2.o
		@$(CC) $(CFLAGS) -o test13 test13.o cthread2.o

test13.o: ./many-one/tests/test13.c
		@$(CC) $(CFLAGS) -c ./many-one/tests/test13.c

test14: test14.o cthread2.o
		@$(CC) $(CFLAGS) -o test14 test14.o cthread2.o

test14.o: ./many-one/tests/test14.c
		@$(CC) $(CFLAGS) -c ./many-one/tests/test14.c

test15: test15.o cthread2.o
		@$(CC) $(CFLAGS) -o test15 test15.o cthread2.o

test15.o: ./many-one/tests/test15.c
		@$(CC) $(CFLAGS) -c ./many-one/tests/test15.c


cthread.o: ./one-one/cthread.c ./one-one/cthread.h
		@$(CC) $(CFLAGS) -c ./one-one/cthread.c

cthread2.o: ./many-one/cthread2.c ./many-one/cthread.h
		@$(CC) $(CFLAGS) -c ./many-one/cthread2.c

run: default
		@bash ./test.sh

clean:
		@$(RM) test1 test2 test3 test4 test5 test6 test7 test8 test9 test10 test11 test12 test13 test14 test15 test *.o *~
