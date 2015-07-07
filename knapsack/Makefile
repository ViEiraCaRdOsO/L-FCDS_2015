CC	= g++
FLAGS	= -fopenmp -O3 -Wall -Wno-unused-result

SRC	= knapsack.c
EXEC	= bin/knapsack
SEQ	= sequential/knapsack.c
SEQ_EXE	= bin/seq_knapsack
GEN_IN	= input/gen.in
RESULT	= output/result.txt
OUT_SEQ	= output/seq.out
OUT_CON	= output/con.out
TEST_SRC= test/test_main.cpp test/test_function.cpp
TEST	= test/test

all: make_dirs $(EXEC) $(TEST) $(SEQ_EXE)

clean:
	rm -f -r bin
	rm -f -r output

make_dirs:
	mkdir -p bin
	mkdir -p output

$(EXEC): $(SRC)
	$(CC) $(FLAGS) $^ -o $@
$(TEST): $(TEST_SRC)
	$(CC) $(FLAGS) $^ -o $@
$(SEQ_EXE): $(SEQ)
	$(CC) $(FLAGS) $^ -o $@

gen_run_check: $(TEST_M) $(TEST_F)
	rm -f $(OUT_CON)
	./$(TEST) gen $(GEN_IN)  1024 24000

	for i in $(shell seq 1 8); do \
		./$(EXEC) $$i $(GEN_IN) $(OUT_CON); \
	done
	#time ./$(SEQ_EXE) $(GEN_IN) $(OUT_SEQ)
	#./$(TEST) check $(OUT_SEQ) $(OUT_CON) $(RESULT)
