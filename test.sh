#!/bin/bash

gcc cthread2.c -c -Wall

gcc cthread2.o tests/yield.c -o test
echo "---------------------------------"

echo "Running yield test..."
t1=$(./test)
if [ $t1 == 2046 ]
then
    echo "Yield Test passed"
else
    echo "Yield test failed"
fi

gcc cthread2.o tests/args.c -o test
echo "---------------------------------"
echo "Running arguments test..."
t2=$(./test)
if [ $t2 == 36 ]
then
    echo "Arguments Test passed"
else
    echo "Arguments test failed"
fi


gcc cthread2.o tests/stress.c -o test
echo "---------------------------------"
echo "Running Stress test with maximum threads..."
t2=$(./test)
if [ $t2 == 512 ]
then
    echo "Stress Test passed"
else
    echo "Stress test failed"
fi


gcc cthread2.o tests/sync1.c -o test
t2=$(./test)
echo "---------------------------------"
echo "Running Synchronisation test #1..."
echo "    Expected result: 300000"
echo "    Actual result $t2"
if [ $t2 == 300000 ]
then
    echo "    Stress Test passed"
else
    echo "    Stress test failed"
fi


gcc cthread2.o tests/sync2.c -o test
echo "---------------------------------"
echo "Running Synchronisation test #2..."
./test

gcc cthread2.o tests/producer-consumer.c -o test
echo "---------------------------------"
echo "Running Producer Consumer test with 1 producer, 2 consumers..."
./test

if [ $? == 0 ]
echo "---------------------------------"
then
    echo "ALL TESTS PASSED"
else
    echo "TESTS FAILED"
fi
echo "---------------------------------"
