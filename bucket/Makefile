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

CORE_NUM = 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16

all: make_dirs $(EXE_SEQ) $(EXE_CON) $(EXE_TEST) $(EXE_GEN)

clean:
	rm -f -r bin
	rm -f -r output
	rm -f $(IN)

make_dirs:
	mkdir -p bin
	mkdir -p output

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
	# gen input
	./$(EXE_GEN) 10000 > $(IN)
	# run seq
	./$(EXE_SEQ) $(IN) $(OUT_SEQ)
	# run con
	for i in $(shell seq 1 8); do \
		export OMP_NUM_THREADS=$$i; \
		taskset -c $(CORE_NUM) ./$(EXE_CON) $(IN) $(OUT_CON); \
		./$(EXE_TEST) $(OUT_SEQ) $(OUT_CON); \
	done
