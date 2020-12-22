#!/bin/bash

EXEC="scrabble"

function init {
	total_score=0
	pad=$(printf '%.1s' "."{1..70})
	padlength=71

	make
	if [ $? -ne 0 ]; then
		echo "Makefile failed!"
		exit 1
	fi
}

function print_result {
	printf "%s" "$1"
	printf "%*.*s" 0 $((padlength - ${#1} - ${#2} )) "$pad"
	printf "%s\n" "$2"
}

function check_task {
	task_id=$1
	start_test_id=$2
	end_test_id=$3
	test_score=$4
	echo "..............................TASK $task_id..................................."
	
	for test_id in $(seq $start_test_id $end_test_id); do
		test_file="./tests/task${task_id}/test${test_id}.txt"
		ref_file="./ref/task${task_id}/test${test_id}_ref.txt"
		output_file=$(mktemp)
	
		./$EXEC < "$test_file" > "$output_file"
	
		if [ -f "$output_file" ]; then
			if diff -w "$output_file" "$ref_file" &> /dev/null; then
				let "total_score += $test_score"
				print_result "Test ${test_id}" "${test_score}/${test_score}p passed"
			else 
				print_result "Test ${test_id}" "0/${test_score}p failed"
			fi
		fi
		rm -f "$output_file"
	done
}

function check_readme {
	echo "..............................TASK README.............................."
	ls . | grep -i "readme" &>/dev/null
	if [ $? -eq 0 ]; then
		print_result "Test README" "10/10p passed"
		let "total_score += 10"
	else
		print_result "Test README" "0/10p failed"
	fi	
}

init
check_task 0 0 0 2
check_task 1 1 9 2
check_task 2 10 19 2
check_task 3 20 29 3
check_task 4 30 39 4
check_task 5 40 49 3
check_task 6 50 59 1
check_readme

print_result "Final score" "${total_score}/150"
make clean &> /dev/null
