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

CORE_NUM := 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31

all: make_dirs $(EXE_SEQ) $(EXE_CON) $(EXE_TEST) $(EXE_GEN)

clean:
	rm -f -r bin
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
	./$(EXE_GEN) $(shell if [ -z $(ITEMS) ] ; then read -p "generate test case of 94 * how many? " ITEMS ; echo $$ITEMS ; fi) > $(IN)
	# run seq
	./$(EXE_SEQ) $(IN) $(OUT_SEQ)
	# run con
	for i in $(shell seq 1 $(shell if [ -z $(PROC_NUM) ] ; then read -p "scale from 0, to how many processors?" PROC_NUM ; echo $$PROC_NUM ; fi )); do \
		echo "[now running on #proc = $$i]"; \
		export OMP_NUM_THREADS=$$i; \
		taskset -c $(CORE_NUM) ./$(EXE_CON) $(IN) $(OUT_CON); \
		./$(EXE_TEST) $(OUT_SEQ) $(OUT_CON); \
	done
final_scale:
	for i in $(shell seq 1 $(shell if [ -z $(PROC_NUM) ] ; then read -p "scale from 0, to how many processors?" PROC_NUM ; echo $$PROC_NUM ; fi )); do \
		echo "[now running on #proc = $$i]"; \
		export OMP_NUM_THREADS=$$i; \
		taskset -c $(CORE_NUM) ./$(EXE_CON) $(shell if [ -z $(IN_PATH) ] ; then read -p "input file path: " IN_PATH ; echo $$IN_PATH ; fi ) $(OUT_CON); \
		# check answer
		./$(EXE_TEST) $(shell if [ -z $(CHECK_PATH) ] ; then read -p "compare file path: " CHECK_PATH ; echo $$CHECK_PATH ; fi) $(OUT_CON);\
	done
