#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <stdint.h>

#define pixel(x,y) pixels[((y)*size)+(x)]

FILE *fin, *fout, *ftime;

void openfiles(char *in, char *out, char *time) {
  fin = fopen(in, "rb");
  if (in == NULL) {
    perror("fopen: cannot open input file");
    exit(EXIT_FAILURE);
  }
  fout = fopen(out, "wb");
  if (out == NULL) {
    perror("fopen: cannot create output file");
    exit(EXIT_FAILURE);
  }
  ftime = fopen(time, "w+");
  if (ftime == NULL) {
    perror("fopen: cannot create time-measure file");
    exit(EXIT_FAILURE);
  }
}

void closefiles(void) {
  fclose(fin);
  fclose(fout);
  fclose(ftime);
}

void print(int *pixels, int size) {
  int x, y;
  for (y = 0; y < size; y++) {
    for (x = 0; x < size; x++) {
      printf("%10d ", pixel(x,y));
    }
    printf("\n");
    fflush(stdout);
  }
}

int main(int argc, char *argv[]) {
  assert(("Usage: ./haar_seq [input_file] [output_file] [time_measure_file]\n") && argc == 4);
  openfiles(argv[1], argv[2], argv[3]);

  clock_t start;
  clock_t end;

  long long int s, size, mid;
  int x, y;
  long long int a, d;
  double SQRT_2;

  fread(&size, sizeof(size), 1, fin);
  fwrite(&size, sizeof(size), 1, fout);

  int *pixels = (int *) malloc(size * size * sizeof(int));

  start = clock();
  if (!fread(pixels, size * size * sizeof(int), 1, fin)) {
    perror("read error");
    exit(EXIT_FAILURE);
  }
  end = clock();
  printf("time to read: %f s\n", (float)(end - start) / CLOCKS_PER_SEC);

  // haar
  start = clock();

  SQRT_2 = sqrt(2);
  for (s = size; s > 1; s /= 2) {
    mid = s / 2;
    // row-transformation
    for (y = 0; y < mid; y++) {
      for (x = 0; x < mid; x++) {
	a = pixel(x,y);
	a = (a+pixel(mid+x,y))/SQRT_2;
	d = pixel(x,y);
	d = (d-pixel(mid+x,y))/SQRT_2;
	pixel(x,y) = a;
	pixel(mid+x,y) = d;
      }
    }
    // column-transformation
    for (y = 0; y < mid; y++) {
      for (x = 0; x < mid; x++) {
	a = pixel(x,y);
	a = (a+pixel(x,mid+y))/SQRT_2;
	d = pixel(x,y);
	d = (d-pixel(x,mid+y))/SQRT_2;
	pixel(x,y) = a;
	pixel(x,mid+y) = d;
      }
    }
  }

  end = clock();
  printf("time to haar: %f s\n", (float)(end - start) / CLOCKS_PER_SEC);
  
  start = clock();
  fprintf(ftime, "%lf\n", (float)(end - start) / CLOCKS_PER_SEC);
  fwrite(pixels, size * size * sizeof(int), 1, fout);
  end = clock();
  printf("time to write: %f s\n", (float)(end - start) / CLOCKS_PER_SEC);

  free(pixels);

  closefiles();

  return EXIT_SUCCESS;
}
