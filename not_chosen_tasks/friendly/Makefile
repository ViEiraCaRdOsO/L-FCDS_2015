CC    = gcc
FLAGS = -O3 -Wall -Wno-unused-result -fopenmp

CON_V1	= friendly_con_v1.c

SRC_SEQ	= friendly_seq.c main.c
EXE_SEQ	= bin/friendly_seq

SRC_CON	= $(CON_V1) main.c
EXE_CON	= bin/friendly_con

OUT_SEQ = output/seq.out
OUT_CON = output/con.out
OUT_TIME = output/time.out

SRC_TEST = test/test.c
EXE_TEST = bin/test

INPUT = input/large.in

CORE_NUM = 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31

all: make_dirs $(EXE_SEQ) $(EXE_CON) $(EXE_TEST)

clean:
	rm -f -r bin
	rm -f -r output

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
	./$(EXE_SEQ) < $(shell if [ -z $(IN_PATH) ] ; then read -p "input file path: " IN_PATH ;echo $$IN_PATH; fi) $(OUT_TIME) > $(OUT_SEQ)

final_scale:
	if [ -z $(IN_PATH) ] ; then read -p "input file path: " IN_PATH ; \
	for i in $(shell seq 1 $(shell if [ -z $(PROC_NUM) ] ; then read -p "scale from 0, to how many processors?" PROC_NUM ; echo $$PROC_NUM ; fi )); do \
		echo "[now running on #proc = $$i]"; \
		export OMP_NUM_THREADS=$$i; \
		taskset -c $(CORE_NUM) ./$(EXE_CON) < $$IN_PATH > $(OUT_CON) $(OUT_TIME); \
		./$(EXE_TEST) $(shell if [ -z $(CHECK_PATH) ] ; then read -p "compare file path: " CHECK_PATH ; echo $$CHECK_PATH ; fi) $(OUT_CON) $(OUT_TIME); \
	done; fi;
