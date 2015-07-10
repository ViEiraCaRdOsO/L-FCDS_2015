CC	= g++
FLAGS	= -fopenmp -O3 -Wall -Wno-unused-result

SRC_CON	= knapsack_con.c
SRC_SEQ	= knapsack_seq.c
EXE_CON	= bin/knapsack_con
EXE_SEQ	= bin/knapsack_seq
OUT_SEQ	= output/seq.out
OUT_CON	= output/con.out

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

run_scale:
	rm -f $(OUT_CON)
	./$(EXE_TEST) gen $(GEN_IN)  1024 1000000
	for i in $(shell seq 1 $(shell if [ -z $(PROC_NUM) ] ; then read -p "scale from 0, to how many processors?" PROC_NUM ; echo $$PROC_NUM ; fi )); do \
		echo "[now running on #proc = $$i]"; \
		taskset -c $(CORE_NUM) ./$(EXE_CON) $$i $(GEN_IN) $(OUT_CON); \
	done
#	./$(EXE_SEQ) $(GEN_IN) $(OUT_SEQ)
#	./$(EXE_TEST) check $(OUT_SEQ) $(OUT_CON)
