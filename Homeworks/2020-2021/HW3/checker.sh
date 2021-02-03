#!/bin/bash

INPUT_DIR=./input

OUT_DIR=./out
TASK1_OUT_DIR=$OUT_DIR/task1
TASK2_OUT_DIR=$OUT_DIR/task2
TASK3_OUT_DIR=$OUT_DIR/task3
TASK4_OUT_DIR=$OUT_DIR/task4
BONUS_OUT_DIR=$OUT_DIR/bonus

EXE=./sudoku

POINTS=0.0

SCORES=(
    0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.25 0.25 0.25 0.25 0.25 0.25 0.25 0.25 0.3 0.5 0.75 0.75 0.75 0.75
    0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.5 0.5 0.5 0.5 0.5 0.5 0.5 0.5 0.6 1 1.5 1.5 1.5 1.5
    0.6 0.6 0.6 0.6 0.6 0.6 0.6 0.6 0.6 0.6 0.6 0.6 0.6 0.6 0.6 0.6 0.6 0.6 0.6 0.6 0.6 0.75 0.75 0.75 0.75 0.75 0.75 0.75 0.75 0.9 1.5 2.25 2.25 2.25 2.25
    1 1 2 2 3 2 2 2 5 3 5 3 3 3 3
    2 2 2 2 2 2 3 3 3 4 5 5
)
MAX_POINTS=(10 20 30 40 35)

VALGRIND_ERR=0

# Make
make

# Clear and create output dirs
rm -rf $OUT_DIR
mkdir $OUT_DIR $TASK1_OUT_DIR $TASK2_OUT_DIR $TASK3_OUT_DIR $TASK4_OUT_DIR $BONUS_OUT_DIR

# Run task123
for input_file in $INPUT_DIR/task1/*; do
    if [ $input_file == "./input/task1/board33.json" ]; then
        valgrind --leak-check=full --error-exitcode=1 $EXE $input_file 123 &>/dev/null
        if [ $? == 1 ]; then
            VALGRIND_ERR=1
        fi
    else
        $EXE $input_file 123 &>/dev/null
    fi
done

# Run task4
for input_file in $INPUT_DIR/task4/*; do
    if [ $input_file == "./input/task4/board10.bmp" ]; then
        valgrind --leak-check=full --error-exitcode=1 $EXE $input_file 4 &>/dev/null
        if [ $? == 1 ]; then
            VALGRIND_ERR=1
        fi
    else
        $EXE $input_file 4 &>/dev/null
    fi
done

# Run bonus
for input_file in $INPUT_DIR/bonus/*; do
    if [ $input_file == "./input/bonus/board12.bmp" ]; then
        valgrind --leak-check=full --error-exitcode=1 $EXE $input_file bonus &>/dev/null
        if [ $? == 1 ]; then
            VALGRIND_ERR=1
        fi
    else
        $EXE $input_file bonus &>/dev/null
    fi
done

# Move files to out dir and rename to the same format as ref files
for output_file in output_task?_board* output_bonus_board*; do
    FILE_NAME="${output_file##*/}"

    TASK_NO="${FILE_NAME%_board*}"
    TASK_NO="${TASK_NO##*task}"

    FILE_NAME="out_${FILE_NAME##*_}"

    if [ "$TASK_NO" == "output_bonus" ]; then
        mv $output_file $OUT_DIR/bonus/$FILE_NAME
    else
        mv $output_file $OUT_DIR/task$TASK_NO/$FILE_NAME
    fi
done

# Compare ref file with out file
CURRENT_TASK="0"
TASK_POINTS=0
TASK_IDX=0
idx=-1
for out_file in $TASK1_OUT_DIR/* $TASK2_OUT_DIR/* $TASK3_OUT_DIR/* $TASK4_OUT_DIR/* $BONUS_OUT_DIR/*; do
    idx=$((idx + 1))

    demanded_task=$1
    demanded_task=${demanded_task^^}

    # Extract reference file according to current output file
    ref_file=`echo "${out_file//out/ref}"`

    # Run and keep the output of cmp and reformat it
    cmp_result=`cmp $out_file $ref_file 2>&1`
    cmp_result=`echo "${cmp_result/cmp/Cause}"`

    # Extract board number
    board="${out_file##*_}"
    board="${board%.*}"

    # Extract current running task
    running_task="${out_file##*task}"
    running_task="${running_task%/*}"

    if [ "$running_task" == "./out/bonus" ]; then
        running_task="${running_task##*/}"
        running_task=${running_task^^}
    fi

    # Skip if a specific task number was demanded
    if [[ "$demanded_task" != "$running_task" ]] && [ ! -z $1 ]; then
        continue
    fi

    # Update task number
    if [[ "$running_task" != "$CURRENT_TASK" ]]; then

        if [[ "$CURRENT_TASK" != "0" ]]; then
            echo -e "\tPoints for task $CURRENT_TASK: $TASK_POINTS/${MAX_POINTS[$TASK_IDX - 1]}.00\n"
            TASK_POINTS=0
        fi

        CURRENT_TASK=`echo $running_task`
        CURRENT_TASK=${CURRENT_TASK^^}
        TASK_IDX=$((TASK_IDX + 1))

        echo "========================================================================================================"
        echo "============================================ RUNNING TASK $CURRENT_TASK ============================================"
        echo "========================================================================================================"
    fi

    if [ -z "$cmp_result" ]; then
        echo "Test $board .................................... PASSED: ${SCORES[$idx]}p"
        POINTS=$( bc <<<"$POINTS + ${SCORES[$idx]}" )
        TASK_POINTS=$( bc <<<"${TASK_POINTS} + ${SCORES[$idx]}" )
    else
        echo "Test $board .................................... FAILED"
        echo -e "\t$cmp_result"
    fi
done

SCORE_IDX="0"
if [ ! -z $1 ]; then
    SCORE_IDX=$(($1 - 1))

    if [ $1 == "bonus" ]; then
        SCORE_IDX=4
    fi
    echo -e "\t\t Total points for task $1: $POINTS/${MAX_POINTS[$SCORE_IDX]}.00"
else
    if [ $VALGRIND_ERR == 1 ]; then
        echo -e "\t\t Total points without memory leak test: $POINTS"
        echo -e "\t\t Valgrind errors: yes"
        POINTS=`echo "$POINTS - 50.0" | bc`
        echo -e "\t\t Total points: $POINTS"
    else
        echo -e "\t\t Total points without memory leak test: $POINTS"
        echo -e "\t\t Valgrind errors: no"
        echo -e "\t\t TOTAL SCORE: $POINTS/135.00"
    fi
fi