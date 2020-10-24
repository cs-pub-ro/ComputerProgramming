#!/bin/bash

points=(5 5 5 5 5 10 15 20 10 20)

function check_task() {
	# @params: $TASK_NUMBER $OUTPUT $REFERENCE
	grep "^-$1" $2 > .tmp.out
	grep "^-$1" $3 > .tmp.ref
	diff -iwBu .tmp.out .tmp.ref &> /dev/null
	return $?
}

function check_test() {
	# @params: $INPUT $OUTPUT $REFERENCE
	INPUT=in/$1
	OUTPUT=out/$(basename $INPUT .in).out
	REFERENCE=ref/$(basename $INPUT .in).ref
	echo "=========================="
	echo "Test file "$INPUT
	echo "=========================="
	./$EXE < $INPUT > $OUTPUT
	test_points=0
	for iter in $(seq 0 9); do
		check_task $iter $OUTPUT $REFERENCE
		if [ $? -eq 0 ]; then
			echo -e "\ttask $iter: OK"
			test_points=$(($test_points + ${points[iter]}))
		else
			echo -e "\ttask $iter: FAILED"
		fi
	done
	return $test_points
}

EXE=ip
TESTING=checker

if [ $? -ne 0 ]; then
	exit 1
fi
make build
if [ $? -ne 0 ]; then
	exit 1
fi
if [ ! -f $EXE ]; then
	echo "The executable name must be $EXE."
	exit 1
fi

TOTAL=0
C=0
mkdir -p out

if [ "$#" -eq 1 ]; then
    if [ $1 == "easy" ]; then 
		echo "##########################"
		echo "RUNNING EASY TESTS"
		echo "##########################"

		for i in $(ls in); do
			if [ $C -lt 20 ]; then
				check_test $i
				TOTAL=$(($TOTAL+$?))
			fi

			((C++))
		done

		TOTAL=$(awk -v m=$TOTAL 'BEGIN { print m / 100 }')
		echo "=============="
		echo "TOTAL: "$TOTAL"/20"
		echo "=============="

	fi

	if [ $1 == "medium" ]; then 
		echo "##########################"
		echo "RUNNING MEDIUM TESTS"
		echo "##########################"

		for i in $(ls in); do
			if [ $C -ge 20 ] && [ $C -lt 50 ]; then
				check_test $i
				TOTAL=$(($TOTAL+$?))
			fi

			((C++))
		done

		TOTAL=$(awk -v m=$TOTAL 'BEGIN { print m / 100 }')
		echo "=============="
		echo "TOTAL: "$TOTAL"/30"
		echo "=============="
	fi

	if [ $1 == "hard" ]; then 
		echo "##########################"
		echo "RUNNING HARD TESTS"
		echo "##########################"

		for i in $(ls in); do
			if [ $C -ge 50 ]; then
				check_test $i
				TOTAL=$(($TOTAL+$?))
			fi

			((C++))
		done

		TOTAL=$(awk -v m=$TOTAL 'BEGIN { print m / 100 }')
		echo "=============="
		echo "TOTAL: "$TOTAL"/40"
		echo "=============="
	fi

else 
	for i in $(ls in); do
		if [ $C -eq 0 ]; then
			echo "##########################"
			echo "RUNNING EASY TESTS"
			echo "##########################"
		fi

		if [ $C -eq 20 ]; then
			echo "##########################"
			echo "RUNNING MEDIUM TESTS"
			echo "##########################"
		fi

		if [ $C -eq 50 ]; then
			echo "##########################"
			echo "RUNNING HARD TESTS"
			echo "##########################"
		fi

		check_test $i
		TOTAL=$(($TOTAL+$?))

		((C++))
	done

	TOTAL=$(awk -v m=$TOTAL 'BEGIN { print m / 100 }')
	echo "=============="
	echo "TOTAL: "$TOTAL"/90"
	echo "=============="
fi

rm -f .tmp.*
make clean
