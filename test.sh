#!/bin/sh

echo -n "Starting one-one tests"
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
./test9 < ./one-one/tests/data.txt
echo " "
sleep 2

echo "With threads"
sleep 1
./test8 < ./one-one/tests/data.txt
echo " "

echo "-------------------------------------------------------------------"
echo " "

sleep 1

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


echo " "
echo "-------------------------------------------------------------------"
echo " "
sleep 1

echo "Test 1: Running yield test..."
echo " "
sleep 1
t1=$(./test10)
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


echo "Test 2: Running arguments test..."
echo " "
sleep 1
t2=$(./test11)
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


echo "Test 3: Running Stress test with maximum threads..."
echo " "
sleep 1
t2=$(./test12)
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


echo "Test 4: Running Synchronisation test #1..."
sleep 1
t2=$(./test13)
echo "    Expected result: 300000"
echo "    Actual result $t2"
echo " "
if [ $t2 == 300000 ]
then
    echo "    Synchronisation Test passed"
else
    echo "    Synchronisation test failed"
fi

echo " "
echo "-------------------------------------------------------------------"
echo " "
sleep 2


echo "Test 5: Running Synchronisation test #2..."
echo " "
sleep 1
./test14

echo " "
echo "-------------------------------------------------------------------"
echo " "
sleep 2

echo "Test 6: Running Producer Consumer test with 1 producer, 2 consumers..."
echo " "
sleep 1
./test15

echo " "
echo "-------------------------------------------------------------------"

if [ $? == 0 ]
then
    echo "ALL TESTS PASSED"
else
    echo "TESTS FAILED"
fi

make clean > /dev/null