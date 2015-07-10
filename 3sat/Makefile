CC    = g++
FLAGS = -O3 -Wall -Wno-unused-result -fopenmp

CON_1 = 3sat_con_v1
CON_2 = 3sat_con_v2 # random -> but false positive too high
CON_3 = 3sat_con_v3 # not good
CON	= $(CON_1)

SRC_SEQ	= 3sat_seq.c
SRC_CON = $(CON).c
EXE_SEQ = bin/3sat_seq
EXE_CON	= bin/$(CON)

SRC_GEN	= test/gen.c
EXE_GEN	= bin/gen

IN	= input/gen.in

CORE_NUM = 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31

all: make_dirs $(EXE_SEQ) $(EXE_CON) $(EXE_GEN)

set_proc:
	taskset -c $(shell pn=0; for n in $(shell seq 1 14); do pn:=$$pn","$$n;done;echo $$pn) ./myprogram

clean:
	rm -f -r bin
	rm -f $(IN)

make_dirs:
	mkdir -p bin

$(EXE_SEQ): $(SRC_SEQ)
	$(CC) $(FLAGS) $^ -o $@ -lm
$(EXE_CON): $(SRC_CON)
	$(CC) $(FLAGS) $^ -o $@ -lm
$(EXE_GEN): $(SRC_GEN)
	$(CC) $(FLAGS) $^ -o $@ -lm

run_scale:
	# gen input
	./$(EXE_GEN) $(shell if [ -z $(CLAUSES) ] ; then read -p "generate test case: clauses N = ? (N < 256) " CLAUSES ; echo $$CLAUSES ; fi) $(shell if [ -z $(LITERALS) ] ; then read -p "generate test case: literals L = ? (L < 2**64) " LITERALS ; echo $$LITERALS ; fi) > $(IN)
	# run seq
	./$(EXE_SEQ) < $(IN)
	# run con
	for i in $(shell seq 1 $(shell if [ -z $(PROC_NUM) ] ; then read -p "scale from 0, to how many processors?" PROC_NUM ; echo $$PROC_NUM ; fi )); do \
		echo "[now running on #proc = $$i]"; \
		export OMP_NUM_THREADS=$$i; \
		taskset -c $(CORE_NUM) ./$(EXE_CON) < $(IN); \
	done
final_scale:
	for i in $(shell seq 1 $(shell if [ -z $(PROC_NUM) ] ; then read -p "scale from 0, to how many processors?" PROC_NUM ; echo $$PROC_NUM ; fi )); do \
		echo "[now running on #proc = $$i]"; \
		export OMP_NUM_THREADS=$$i; \
		./$(EXE_CON) < $(shell if [ -z $(IN_PATH) ] ; then read -p "input file path: " IN_PATH ; echo $$IN_PATH ; fi ); \
	done
# taskset -c $(CORE_NUM) ./$(EXE_CON) < $(shell if [ -z $(IN_PATH) ] ; then read -p "input file path: " IN_PATH ; echo $$IN_PATH ; fi );
