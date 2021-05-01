#!/bin/bash

echo " "
echo " "

echo -n "Starting Many-one tests"
sleep 0.4
echo -n "."
sleep 0.4
echo -n "."
sleep 0.4
echo -n "."
sleep 0.4
echo -n "."
sleep 0.4
echo "."


gcc cthread2.c -c -Wall

gcc cthread2.o tests/yield.c -o test

echo " "
echo "-------------------------------------------------------------------"
echo " "
sleep 1

echo "Test 1: Running yield test..."
echo " "
sleep 1
t1=$(./test)
if [ $t1 == 2046 ]
then
    echo "Yield Test passed"
else
    echo "Yield test failed"
fi

echo " "
echo "-------------------------------------------------------------------"
echo " "
sleep 2


gcc cthread2.o tests/args.c -o test

echo "Test 2: Running arguments test..."
echo " "
sleep 1
t2=$(./test)
if [ $t2 == 36 ]
then
    echo "Arguments Test passed"
else
    echo "Arguments test failed"
fi

echo " "
echo "-------------------------------------------------------------------"
echo " "
sleep 2


gcc cthread2.o tests/stress.c -o test

echo "Test 3: Running Stress test with maximum threads..."
echo " "
sleep 1
t2=$(./test)
if [ $t2 == 512 ]
then
    echo "Stress Test passed"
else
    echo "Stress test failed"
fi

echo " "
echo "-------------------------------------------------------------------"
echo " "
sleep 2


gcc cthread2.o tests/sync1.c -o test
echo "Test 4: Running Synchronisation test #1..."
echo "    Expected result: 300000"
echo "    Actual result $t2"
echo " "
sleep 1
t2=$(./test)
if [ $t2 == 300000 ]
then
    echo "    Stress Test passed"
else
    echo "    Stress test failed"
fi

echo " "
echo "-------------------------------------------------------------------"
echo " "
sleep 2


gcc cthread2.o tests/sync2.c -o test
echo "Test 5: Running Synchronisation test #2..."
echo " "
sleep 1
./test

echo " "
echo "-------------------------------------------------------------------"
echo " "
sleep 2

gcc cthread2.o tests/producer-consumer.c -o test
echo "Test 6: Running Producer Consumer test with 1 producer, 2 consumers..."
echo " "
sleep 1
./test

echo " "
echo "-------------------------------------------------------------------"

if [ $? == 0 ]
then
    echo "ALL TESTS PASSED"
else
    echo "TESTS FAILED"
fi

rm test

