CC    = g++
FLAGS = -O3 -Wall -Wno-unused-result -fopenmp

SRC_SEQ	= haar_seq.c
SRC_CON = haar_con.c
SRC_TEST = test/check.cpp
EXE_SEQ = bin/haar_seq
EXE_CON	= bin/haar_con
EXE_TEST = bin/check
GEN	 = bin/input_generator
IN	= input/gen.in
OUT_SEQ	= output/seq.out
OUT_CON = output/con.out

CORE_NUM = 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16

all: make_dirs $(EXE_SEQ) $(EXE_CON) $(GEN) $(EXE_TEST)

clean:
	rm -f -r bin
	rm -f -r input
	rm -f -r output
	rm -f $(IN)

make_dirs:
	mkdir -p bin
	mkdir -p input
	mkdir -p output

$(EXE_SEQ): $(SRC_SEQ)
	$(CC) $(FLAGS) $^ -o $@

$(EXE_CON): $(SRC_CON)
	$(CC) $(FLAGS) $^ -o $@

$(EXE_TEST): $(SRC_TEST)
	$(CC) $(FLAGS) $^ -o $@

$(GEN): input_generator.c
	$(CC) $(FLAGS) $^ -o $@

run_scale:
	./$(GEN) $(IN) 16384
	echo "seq: "
	./$(EXE_SEQ) $(IN) $(OUT_SEQ)
	for i in $(shell seq 1 8); do \
		export OMP_NUM_THREADS=$$i; \
		echo "con".$$i.":"; \
		taskset -c $(CORE_NUM) ./$(EXE_CON) $(IN) $(OUT_CON); \
		./$(EXE_TEST) $(OUT_SEQ) $(OUT_CON); \
	done
