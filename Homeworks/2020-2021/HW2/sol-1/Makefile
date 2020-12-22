CC = gcc
CFLAG = -Wall -Wextra
EXE = scrabble
OBJ = scrabble.o

OS := $(shell uname)
ifeq ($(OS), Darwin)
	LDFLAGS = -L./util -lprint_board_mac -lm
else
	LDFLAGS = -L./util -lprint_board_lin -lm
endif

build: $(EXE)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAG)

$(EXE): $(OBJ)
	$(CC) $^ $(CFLAG) $(LDFLAGS) -o $(EXE)

run:
	./$(EXE)

.PHONY: clean

clean:
	rm -f *.o $(EXE)

