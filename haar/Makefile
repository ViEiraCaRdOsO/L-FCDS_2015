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
OUT_TIME = output/time.out

CORE_NUM = 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31

all: make_dirs $(EXE_SEQ) $(EXE_CON) $(GEN) $(EXE_TEST)

clean:
	rm -f -r bin
	rm -f -r input
	rm -f -r output

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

gen_input:
	./$(GEN) $(IN) $(shell if [ -z $(SIZE) ] ; then read -p "matrix size? " SIZE ;echo $$SIZE; fi)

run_seq:
	./$(EXE_SEQ) $(shell if [ -z $(IN_PATH) ] ; then read -p "input file path: " IN_PATH ;echo $$IN_PATH; fi) $(OUT_SEQ) $(OUT_TIME)

final_scale:
	if [ -z $(IN_PATH) ] ; then read -p "input file path: " IN_PATH ; \
	for i in $(shell seq 1 $(shell if [ -z $(PROC_NUM) ] ; then read -p "scale from 0, to how many processors?" PROC_NUM ; echo $$PROC_NUM ; fi )); do \
		echo "[now running on #proc = $$i]"; \
		export OMP_NUM_THREADS=$$i; \
		echo "con".$$i.":"; \
		taskset -c $(CORE_NUM) ./$(EXE_CON) $$IN_PATH $(OUT_CON) $(OUT_TIME); \
		./$(EXE_TEST) $(shell if [ -z $(CHECK_PATH) ] ; then read -p "compare file path: " CHECK_PATH ; echo $$CHECK_PATH ; fi) $(OUT_CON) $(OUT_TIME); \
	done; fi
