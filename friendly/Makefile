CC    = gcc
FLAGS = -O3 -Wall -Wno-unused-result -fopenmp

CON_V1	= friendly_con_v1.c

SRC_SEQ	= friendly_seq.c main.c
EXE_SEQ	= bin/friendly_seq

SRC_CON	= $(CON_V1) main.c
EXE_CON	= bin/friendly_con

OUT_SEQ = output/seq.out
OUT_CON = output/con.out

SRC_TEST = test/test.c
EXE_TEST = bin/test

INPUT = input/large.in

all: make_dirs $(EXE_SEQ) $(EXE_CON) $(EXE_TEST)

clean:
	rm -f -r bin

make_dirs:
	mkdir -p bin
	mkdir -p output

$(EXE_SEQ): $(SRC_SEQ)
	$(CC) $(FLAGS) $^ -o $@
$(EXE_CON): $(SRC_CON)
	$(CC) $(FLAGS) $^ -o $@
$(EXE_TEST): $(SRC_TEST)
	$(CC) $(FLAGS) $^ -o $@

run_seq:
	time ./$(EXE_SEQ) < $(INPUT) > $(OUT_SEQ)
run_con:
	time ./$(EXE_CON) < $(INPUT) > $(OUT_CON)
run_test:
	./$(EXE_TEST) $(OUT_SEQ) $(OUT_CON)
run_scale:
	for i in $(shell seq 1 8); do \
		export OMP_NUM_THREADS=$$i; \
		time ./$(EXE_CON) < $(INPUT) > $(OUT_CON); \
		./$(EXE_TEST) $(OUT_SEQ) $(OUT_CON); \
	done
