make

./test_gates 2> /dev/null > output
./test_comm 2> /dev/null >> output
./test_hunt 2> /dev/null >> output

make clean

SCORE=$(awk -F. '{sum += $NF} END {print sum}' output)
TOTAL_SCORE=90

cat output
echo -e "\n"
echo "TOTAL: ${SCORE}/${TOTAL_SCORE}"

