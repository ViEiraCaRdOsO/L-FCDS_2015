CC    = gcc
FLAGS = -O3 -Wall -Wno-unused-result -fopenmp

SRC_SEQ	= bucketsort_seq.c main.c
SRC_CON	= bucketsort_con.c main.c

EXE_SEQ = bin/bucketsort_seq
EXE_CON	= bin/bucketsort_con

OUT_SEQ = output/seq.out
OUT_CON	= output/con.out

IN	= input/gen.in
OUT_TIME = output/time.out

EXE_GEN	= bin/gen
SRC_GEN	= test/gen.c

SRC_TEST = test/test.c
EXE_TEST = bin/test

all: make_dirs $(EXE_SEQ) $(EXE_CON) $(EXE_TEST) $(EXE_GEN)

clean:
	rm -f -r bin
	rm -f -r output
	rm -f -r input

make_dirs:
	mkdir -p bin
	mkdir -p input
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
	time ./$(EXE_SEQ) $(shell if [ -z $(IN_PATH) ] ; then read -p "input file path: " IN_PATH ; echo $$IN_PATH ; fi) $(OUT_SEQ) $(OUT_TIME)

gen_input:
	./$(EXE_GEN) $(shell if [ -z $(ITEMS) ] ; then read -p "generate test case of 94 * how many? " ITEMS ; echo $$ITEMS ; fi) > $(IN)

final_scale:
	if [ -z $(IN_PATH) ] ; then read -p "input file path: " IN_PATH ; \
	for i in $(shell seq 1 $(shell if [ -z $(PROC_NUM) ] ; then read -p "scale from 0, to how many processors?" PROC_NUM ; echo $$PROC_NUM ; fi )); do \
		echo "[now running on #proc = $$i]"; \
		export OMP_NUM_THREADS=$$i; \
		time taskset -c "0$$(printf ,%s $$(seq 1 $$i))" ./$(EXE_CON) $$IN_PATH $(OUT_CON) $(OUT_TIME); \
		./$(EXE_TEST) $(shell if [ -z $(CHECK_PATH) ] ; then read -p "compare file path: " CHECK_PATH ; echo $$CHECK_PATH ; fi) $(OUT_CON) $(OUT_TIME);\
	done; fi;
