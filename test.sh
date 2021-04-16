#!/bin/sh

echo -n "Starting tests"
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

echo " "
echo "-------------------------------------------------------------------"
echo " "
sleep 1

echo "Test 1: Test for cthread_create and cthread_join"
echo " "
sleep 1
./test1
echo " "
echo "-------------------------------------------------------------------"
echo " "
sleep 2

echo "Test 2: Test for cthread_create and cthread_join with parameters and return value"
echo " "
sleep 1
./test2
echo " "
echo "-------------------------------------------------------------------"
echo " "
sleep 2

echo "Test 3: Test for sychronization with mutex locks"
echo " "
sleep 1
./test3
echo " "
echo "-------------------------------------------------------------------"
echo " "
sleep 2

echo "Test 4: Test for sychronization with spin lock"
echo " "
sleep 1
./test4
echo " "
echo "-------------------------------------------------------------------"
echo " "
sleep 2

echo "Test 5: Test for fetching the current thread"
echo " "
sleep 1
./test5
echo " "
echo "-------------------------------------------------------------------"
echo " "
sleep 2

echo "Test 6: Test for exiting the thread safely with cthread_exit"
echo " "
sleep 1
./test6
echo " "
echo "-------------------------------------------------------------------"
echo " "
sleep 2

echo "Test 7: Test for sending signal to a thread"
echo " "
sleep 1
./test7
echo " "
echo "-------------------------------------------------------------------"
echo " "
sleep 2


echo "Performance Test"
echo "Matrix multiplication 1000 * 1000"
sleep 1
echo " "

echo "Without threads"
sleep 1
./test9 < data.txt
echo " "
sleep 2

echo "With threads"
sleep 1
./test8 < data.txt
echo " "

echo "-------------------------------------------------------------------"
echo " "

sleep 1
echo "All tests are completed"
