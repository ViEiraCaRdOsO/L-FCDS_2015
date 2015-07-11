CC	= g++
FLAGS	= -fopenmp -O3 -Wall -Wno-unused-result

SRC_CON	= knapsack_con.c
SRC_SEQ	= knapsack_seq.c
EXE_CON	= bin/knapsack_con
EXE_SEQ	= bin/knapsack_seq
OUT_SEQ	= output/seq.out
OUT_CON	= output/con.out
OUT_TIME = output/time.out

GEN_IN	= input/gen.in

SRC_TEST = test/test_main.cpp test/test_function.cpp
EXE_TEST = bin/test

CORE_NUM = 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31

all: make_dirs $(EXE_CON) $(EXE_TEST) $(EXE_SEQ)

clean:
	rm -f -r bin
	rm -f -r output
	rm -f $(GEN_IN)

make_dirs:
	mkdir -p bin
	mkdir -p output

$(EXE_CON): $(SRC_CON)
	$(CC) $(FLAGS) $^ -o $@
$(EXE_TEST): $(SRC_TEST)
	$(CC) $(FLAGS) $^ -o $@
$(EXE_SEQ): $(SRC_SEQ)
	$(CC) $(FLAGS) $^ -o $@

gen_input:
	./$(EXE_TEST) gen $(GEN_IN) $(shell if [ -z $(SIZE_M) ] ; then read -p "#item? (n<=1024) " SIZE_M ;echo $$SIZE_M; fi) $(shell if [ -z $(SIZE_C) ] ; then read -p "knapsack capacity? (M<=10**6) " SIZE_C ;echo $$SIZE_C; fi)

run_seq:
	./$(EXE_SEQ) $(shell if [ -z $(IN_PATH) ] ; then read -p "input file path: " IN_PATH ;echo $$IN_PATH; fi) $(OUT_SEQ) $(OUT_TIME)

final_scale:
	if [ -z $(IN_PATH) ] ; then read -p "input file path: " IN_PATH ; \
	for i in $(shell seq 1 $(shell if [ -z $(PROC_NUM) ] ; then read -p "scale from 0, to how many processors?" PROC_NUM ; echo $$PROC_NUM ; fi )); do \
		echo "[now running on #proc = $$i]"; \
		export OMP_NUM_THREADS=$$i; \
		taskset -c $(CORE_NUM) ./$(EXE_CON) $$IN_PATH $(OUT_CON) $(OUT_TIME); \
		./$(EXE_TEST) check $(shell if [ -z $(CHECK_PATH) ] ; then read -p "compare file path: " CHECK_PATH ; echo $$CHECK_PATH ; fi) $(OUT_CON) $(OUT_TIME); \
	done; fi
