CC := gcc

#Debug
CFLAGS := -g -Wall -Iinclude -DDEBUG
#CFLAGS := -g -Wall -Iinclude
LDFLAGS := -lm
SRC_FOLDER := src
TEST_FOLDER := test

TEST_GATES_OBJ := $(SRC_FOLDER)/gates.o $(TEST_FOLDER)/test_gates.o
TEST_COMM_OBJ := $(SRC_FOLDER)/communication.o $(SRC_FOLDER)/util_comm.o $(TEST_FOLDER)/test_comm.o
TEST_HUNT_OBJ := $(SRC_FOLDER)/hunt.o $(TEST_FOLDER)/test_hunt.o

all: test_gates test_comm test_hunt

test_gates: $(TEST_GATES_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_comm: $(TEST_COMM_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_hunt: $(TEST_HUNT_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf $(TEST_GATES_OBJ) $(TEST_COMM_OBJ) $(TEST_HUNT_OBJ)
