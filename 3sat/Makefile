CC    = g++
FLAGS = -std=c++11 -O3 -Wall -Wno-unused-result -fopenmp

CON_1 = 3sat_con_v1
CON_2 = 3sat_con_v2
CON_3 = 3sat_con_v3
CON	= $(CON_3)

SRC_SEQ	= 3sat_seq.c
SRC_CON = $(CON).c
EXE_SEQ = bin/3sat_seq
EXE_CON	= bin/$(CON)

SRC_GEN	= test/gen.c
EXE_GEN	= bin/gen

IN	= input/gen.in

all: make_dirs $(EXE_SEQ) $(EXE_CON) $(EXE_GEN)

clean:
	rm -f -r bin

make_dirs:
	mkdir -p bin

$(EXE_SEQ): $(SRC_SEQ)
	$(CC) $(FLAGS) $^ -o $@ -lm
$(EXE_CON): $(SRC_CON)
	$(CC) $(FLAGS) $^ -o $@ -lm
$(EXE_GEN): $(SRC_GEN)
	$(CC) $(FLAGS) $^ -o $@ -lm

run_scale:
	#./$(EXE_GEN) 50 10 > $(IN)
	#./$(EXE_SEQ) < $(IN)
	for i in $(shell seq 1 8); do \
		export OMP_NUM_THREADS=$$i; \
		./$(EXE_CON) < input/large.in; \
	done
