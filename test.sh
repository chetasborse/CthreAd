#!/bin/bash
gcc cthread2.o tests/yield.c -o test
t1=$(./test)
if [ $t1 == 2046 ]
then
    echo "Yield Test passed"
else
    echo "Yield test failed"
fi

gcc cthread2.o tests/args.c -o test
t2=$(./test)
if [ $t2 == 36 ]
then
    echo "Arguments Test passed"
else
    echo "Arguments test failed"
fi

gcc cthread2.o tests/stress.c -o test
t2=$(./test)
if [ $t2 == 512 ]
then
    echo "Stress Test passed"
else
    echo "Stress test failed"
fi
