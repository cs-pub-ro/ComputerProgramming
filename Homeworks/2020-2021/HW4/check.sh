#!/bin/bash

bc --version &> /dev/null
bcInstalled=$?
valgrind --version &> /dev/null
vgInstalled=$?
if [ $bcInstalled -eq 127 ] || [ $vgInstalled -eq 127 ]; then
    printf "In order to run the checker, please install bc and valgrind\n"
    printf "\tsudo apt install bc\n"
    printf "\tsudo apt install valgrind\n"
    exit
fi

DEBUG=0 # change this to 1 to preserve output files
TESTS_NUMBER=5
TASKS_NUMBER=9
TOTAL_POINTS=90
STUDENT_SCORE=0.00
POINTS_PER_TEST=$(bc <<< "scale=2; $TOTAL_POINTS/$TESTS_NUMBER")
POINTS_PER_TASK=$(bc <<< "scale=2; $POINTS_PER_TEST/$TASKS_NUMBER")
README_POINTS=5
EXE=tema4

printf "_________________________________________________________\n"
printf "|          Introduction to Computer Programming         |\n"
printf "|                       Homework 4                      |\n"
printf "|_______________________________________________________|\n"

# Build solution
printf "|                   Building solution                   |\n"
make &> out.make
cnt=$(cat out.make| grep Error | wc -l)
if [ $cnt -gt 0 ]; then
    printf "|!!!!!!!!!!!!!!!!!!!Compilation errors!!!!!!!!!!!!!!!!!!|\n"
    printf "|_______________________________________________________|\n"
    cat out.make
    rm -f out.make
    exit
else
    printf "|-----------------Compilation successful----------------|\n"
    printf "|_______________________________________________________|\n"
fi
rm -f out.make

# Prepare environment
rm -rf output
mkdir output

printf "|.....................Running tests.....................|\n"
for (( i=0; i < TESTS_NUMBER; i++ )); do
    printf "|_______________________ Test $i _______________________"
    if [ $i -lt 10 ]; then
        printf "_"
    fi
    printf "|\n"
    valgrind --leak-check=full --error-exitcode=1 ./$EXE input/test_$i.in output/test_$i > test_$i.err 2> /dev/null

    valgrindErrors=$?
    errorCode=$(head -n1 test_$i.err)
    rm test_$i.*


    printf "|                        Part 1                         |\n"
    printf "| Task 1 - "
    cat output/test_$i-task1.out > tsk1.out
    cat ref/test_$i-task1.ref > tsk1.ref
    diff -q -Z tsk1.out tsk1.ref > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        printf "FAILED                                       |\n"
        rm -f tsk1*
        continue
    else
        printf "PASSED                                       |\n"
        STUDENT_SCORE=$(echo "$STUDENT_SCORE+$POINTS_PER_TASK" | bc)
    fi
    rm -f tsk1*

    printf "| Task 2 - "
    cat output/test_$i-task2.out > tsk2.out
    cat ref/test_$i-task2.ref > tsk2.ref
    diff -q -Z tsk2.out tsk2.ref > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        printf "FAILED                                       |\n"
        rm -f tsk2*
        continue
    else
        printf "PASSED                                       |\n"
        STUDENT_SCORE=$(echo "$STUDENT_SCORE+$POINTS_PER_TASK" | bc)
    fi
    rm -f tsk2*

    printf "| Task 3 - "
    cat output/test_$i-task3.out > tsk3.out
    cat ref/test_$i-task3.ref > tsk3.ref
    diff -q -Z tsk3.out tsk3.ref > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        printf "FAILED                                       |\n"
        rm -f tsk3*
        continue
    else
        printf "PASSED                                       |\n"
        STUDENT_SCORE=$(echo "$STUDENT_SCORE+$POINTS_PER_TASK" | bc)
    fi
    rm -f tsk3*

    printf "| Task 4 - "
    cat output/test_$i-task4.out > tsk4.out
    cat ref/test_$i-task4.ref > tsk4.ref
    diff -q -Z tsk4.out tsk4.ref > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        printf "FAILED                                       |\n"
        rm -f tsk4*
        continue
    else
        printf "PASSED                                       |\n"
        STUDENT_SCORE=$(echo "$STUDENT_SCORE+$POINTS_PER_TASK" | bc)
    fi
    rm -f tsk4*

    printf "| Task 5 - "
    cat output/test_$i-task5.out > tsk5.out
    cat ref/test_$i-task5.ref > tsk5.ref
    diff -q -Z tsk5.out tsk5.ref > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        printf "FAILED                                       |\n"
        rm -f tsk5*
        continue
    else
        printf "PASSED                                       |\n"
        STUDENT_SCORE=$(echo "$STUDENT_SCORE+$POINTS_PER_TASK" | bc)
    fi
    rm -f tsk5*

    printf "| Task 6 - "
    cat output/test_$i-task6.out > tsk6.out
    cat ref/test_$i-task6.ref > tsk6.ref
    diff -q -Z tsk6.out tsk6.ref > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        printf "FAILED                                       |\n"
        rm -f tsk6*
        continue
    else
        printf "PASSED                                       |\n"
        STUDENT_SCORE=$(echo "$STUDENT_SCORE+$POINTS_PER_TASK" | bc)
    fi
    rm -f tsk6*

    printf "| Task 7 - "
    cat output/test_$i-task7.out > tsk7.out
    cat ref/test_$i-task7.ref > tsk7.ref
    diff -q -Z tsk7.out tsk7.ref > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        printf "FAILED                                       |\n"
        rm -f tsk7*
        continue
    else
        printf "PASSED                                       |\n"
        STUDENT_SCORE=$(echo "$STUDENT_SCORE+$POINTS_PER_TASK" | bc)
    fi
    rm -f tsk7*

    printf "| Task 8 - "
    cat output/test_$i-task8.out > tsk8.out
    cat ref/test_$i-task8.ref > tsk8.ref
    diff -q -Z tsk8.out tsk8.ref > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        printf "FAILED                                       |\n"
        rm -f tsk8*
        continue
    else
        printf "PASSED                                       |\n"
        STUDENT_SCORE=$(echo "$STUDENT_SCORE+$POINTS_PER_TASK" | bc)
    fi
    rm -f tsk8*

    printf "| Task 9 - "
    if [ $valgrindErrors -ne 0 ]; then
        printf "FAILED                                       |\n"
        continue
    else
        printf "PASSED                                       |\n"
        STUDENT_SCORE=$(echo "$STUDENT_SCORE+$POINTS_PER_TASK" | bc)
    fi
done

printf "|_______________________________________________________|\n"

README=README
if (( $(echo "$STUDENT_SCORE == 0" |bc -l)  )); then
    printf "| No $README points, tests points are $STUDENT_SCORE!              |"
elif [ ! -f $README ]; then
    printf "| Missing $README!                                       |"
elif [ -f $README ] && [ "`ls -l $README | awk '{print $5}'`" == "0" ]; then
    printf "| Empty $README!                                         |"
else
    printf "| $README - PASSED                                       |"
    STUDENT_SCORE=$(echo "$STUDENT_SCORE+$README_POINTS" | bc)
fi
printf "\n"

printf "| 5 points for coding style will be manually given      |\n"


if (( $(echo "$STUDENT_SCORE < 10.00" |bc -l)  )); then
        printf "|_____________________ TOTAL: $STUDENT_SCORE/100 ____________________|\n"
else
        printf "|____________________ TOTAL: $STUDENT_SCORE/100 __________________|\n"
fi
printf "|_______________________________________________________|\n"