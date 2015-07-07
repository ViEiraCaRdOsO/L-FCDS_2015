CC    = g++
FLAGS = -O3 -Wall -Wno-unused-result -fopenmp

CON_1 = 3sat_con_v1
CON_2 = 3sat_con_v2
CON_3 = 3sat_con_v3
CON	= $(CON_1)

SRC_SEQ	= 3sat_seq.c
SRC_CON = $(CON).c
EXE_SEQ = bin/3sat_seq
EXE_CON	= bin/$(CON)

SRC_GEN	= test/gen.c
EXE_GEN	= bin/gen

IN	= input/gen.in

CORE_NUM = 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16

all: make_dirs $(EXE_SEQ) $(EXE_CON) $(EXE_GEN)

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
	./$(EXE_GEN) 50 10 > $(IN)
	# run seq
	./$(EXE_SEQ) < $(IN)
	# run con
	for i in $(shell seq 1 8); do \
		export OMP_NUM_THREADS=$$i; \
		taskset -c $(CORE_NUM) ./$(EXE_CON) < input/large.in; \
	done
