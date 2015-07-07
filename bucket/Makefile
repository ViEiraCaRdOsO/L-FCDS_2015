CC    = gcc
FLAGS = -O3 -Wall -Wno-unused-result -fopenmp

SRC_SEQ	= bucketsort_seq.c main.c
SRC_CON	= bucketsort_con.c main.c

EXE_SEQ = bin/bucketsort_seq
EXE_CON	= bin/bucketsort_con

OUT_SEQ = output/seq.out
OUT_CON	= output/con.out

IN	= input/gen.in

EXE_GEN	= bin/gen
SRC_GEN	= test/gen.c

SRC_TEST = test/test.c
EXE_TEST = bin/test

all: make_dirs $(EXE_SEQ) $(EXE_CON) $(EXE_TEST) $(EXE_GEN)

clean:
	rm -f -r bin

make_dirs:
	mkdir -p bin

$(EXE_SEQ): $(SRC_SEQ)
	$(CC) $(FLAGS) $^ -o $@
$(EXE_CON): $(SRC_CON)
	$(CC) $(FLAGS) $^ -o $@
$(EXE_TEST): $(SRC_TEST)
	$(CC) $(FLAGS) $^ -o $(EXE_TEST)
$(EXE_GEN): $(SRC_GEN)
	$(CC) $(FLAGS) $^ -o $(EXE_GEN)

run_seq:
	./$(EXE_SEQ) $(IN) $(OUT_SEQ)
run_con: $(SRC_CON)
	$(CC) $(FLAGS) $^ -o $(EXE_CON)
	./$(EXE_CON) $(IN) $(OUT_CON)
run_test:
	./$(EXE_TEST) $(OUT_SEQ) $(OUT_CON)
run_scale:
	./$(EXE_GEN) 10000 > $(IN)
	export OMP_NUM_THREADS=8
	time ./$(EXE_CON) $(IN) $(OUT_CON)
#	./$(EXE_SEQ) $(IN) $(OUT_SEQ)
#	for i in $(shell seq 1 8); do \
		export OMP_NUM_THREADS=$$i; \
		./$(EXE_CON) $(IN) $(OUT_CON); \
		./$(EXE_TEST) $(OUT_SEQ) $(OUT_CON); \
	done
