CC    = g++
FLAGS = -O3 -Wall -Wno-unused-result -fopenmp -Wsign-compare

CON_1 = 3sat_con_v1
CON_2 = not_used/3sat_con_v2 # random -> but false positive too high
CON_3 = not_used/3sat_con_v3 # not good
CON	= $(CON_1)

SRC_SEQ	= 3sat_seq.c
SRC_CON = $(CON).c
OUT_SEQ = output/seq.out
EXE_SEQ = bin/3sat_seq
EXE_CON	= bin/$(CON)
OUT_CON = output/con.out

S_GEN	= test/inputer.py
NS_GEN	= test/inputer_unsolvable.py
SRC_CK	= test/check.cpp
EXE_CK	= bin/check

OUT_TIME = output/time.out

IN	= input/gen.in

all: make_dirs $(EXE_SEQ) $(EXE_CON) $(EXE_CK)

clean:
	rm -f -r bin
	rm -f -r output

make_dirs:
	mkdir -p bin
	mkdir -p input
	mkdir -p output

$(EXE_SEQ): $(SRC_SEQ)
	$(CC) $(FLAGS) $^ -o $@ -lm
$(EXE_CON): $(SRC_CON)
	$(CC) $(FLAGS) $^ -o $@ -lm
$(EXE_CK): $(SRC_CK)
	$(CC) $(FLAGS) $^ -o $@ -lm

gen_solvable:
	python $(S_GEN) -n $(shell if [ -z $(CLAUSES) ] ; then read -p "generate test case: clauses N = ? (N < 256) " CLAUSES ; echo $$CLAUSES ; fi) -l $(shell if [ -z $(LITERALS) ] ; then read -p "generate test case: literals L = ? (L < 2**64) " LITERALS ; echo $$LITERALS ; fi) > $(IN)
gen_unsolvable:
	python $(NS_GEN) -n $(shell if [ -z $(CLAUSES) ] ; then read -p "generate test case: clauses N = ? (N < 256) " CLAUSES ; echo $$CLAUSES ; fi) -l $(shell if [ -z $(LITERALS) ] ; then read -p "generate test case: literals L = ? (L < 2**64) " LITERALS ; echo $$LITERALS ; fi) > $(IN)

run_seq:
	./$(EXE_SEQ) < $(shell if [ -z $(IN_PATH) ] ; then read -p "input file path: " IN_PATH ; echo $$IN_PATH ; fi ) $(OUT_SEQ) $(OUT_TIME)

final_scale:
	if [ -z $(IN_PATH) ] ; then read -p "input file path: " IN_PATH; \
	for i in $(shell seq 1 $(shell if [ -z $(PROC_NUM) ] ; then read -p "scale from 0, to how many processors?" PROC_NUM ; echo $$PROC_NUM ; fi )); do \
		echo "[now running on #proc = $$i]"; \
		export OMP_NUM_THREADS=$$i; \
		taskset -c "0$$(printf ,%s $$(seq 1 $$i))" ./$(EXE_CON) < $$IN_PATH $(OUT_CON) $(OUT_TIME); \
		./$(EXE_CK) $$IN_PATH $(shell if [ -z $(CMP_PATH) ] ; then read -p "compare file path: " CMP_PATH ; echo $$CMP_PATH ; fi ) $(OUT_CON) $(OUT_TIME); \
	done; fi;
