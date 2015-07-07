#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "friendly.h"

int main(int argc, char **argv) {
	long int start;
	long int end;

	while (1) {
		scanf("%ld %ld", &start, &end);
		if (start == 0 && end == 0)
			break;
		printf("Number %ld to %ld\n", start, end);
		friendly_numbers(start, end);
	}

	return EXIT_SUCCESS;
}
