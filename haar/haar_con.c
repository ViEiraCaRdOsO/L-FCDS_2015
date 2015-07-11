#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <omp.h>

#define pixel(x,y) pixels[((y)*size)+(x)]
#define CHUNK_SIZE 1<<9

FILE *fin, *fout, *ftime;
int *pixels;

void openfiles(char *in, char *out, char *time);
void closefiles(void);
void read_data(long long int size);
void transform(long long int size);
void write_data(long long int size);
void print(long long int size);

int main(int argc, char *argv[]) {
  assert(("Usage: ./haar_con [input_file] [output_file] [time_measure_file]\n") && argc == 4);
  openfiles(argv[1], argv[2], argv[3]);
  
  long long int size;
  fread(&size, sizeof(size), 1, fin);
  fwrite(&size, sizeof(size), 1, fout);
  
  pixels = (int *) malloc(size * size * sizeof(int));
  read_data(size);
  transform(size);
  write_data(size);
  
  free(pixels);
  closefiles();
  
  return 0;
}

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

void read_data(long long int size) {
  double st = omp_get_wtime();
  if (!fread(pixels, size * size * sizeof(int), 1, fin)) {
    perror("read error");
    exit(EXIT_FAILURE);
  }
  double ed = omp_get_wtime();
  printf("time to read: %.4lfs\n", ed - st);
}

void transform(long long int size) {
  double st = omp_get_wtime();
  
  double SQRT_2 = sqrt(2);
  for (long long int s = size; s > 1; s /= 2) {
    long long int mid = s/2;
    // row-transformation
    for (long long int y = 0; y < mid; y++) {
#pragma omp parallel for schedule(dynamic, CHUNK_SIZE)
      for (long long int x = 0; x < mid; x++) {
	long long int a = pixel(x,y);
	a = (a+pixel(mid+x,y)) / SQRT_2;
	long long int d = pixel(x, y);
	d = (d-pixel(mid+x,y)) / SQRT_2;
	pixel(x,y) = a;
	pixel(mid+x,y) = d;
      }
    }
    // column-transformation
    for (long long int y = 0; y < mid; y++) {
#pragma omp parallel for schedule(dynamic, CHUNK_SIZE)
      for (long long int x = 0; x < mid; x++) {
	long long int a = pixel(x,y);
	a = (a+pixel(x,mid+y)) / SQRT_2;
	long long int d = pixel(x, y);
	d = (d-pixel(x,mid+y)) / SQRT_2;
	pixel(x,y) = a;
	pixel(x,mid+y) = d;
      }
    }
  }

  double ed = omp_get_wtime();
  printf("time to haar %.4lfs\n", ed - st);
  fprintf(ftime, "%lf\n", ed - st);
}

void write_data(long long int size) {
  double st = omp_get_wtime();
  fwrite(pixels, size * size * sizeof(int), 1, fout);
  double ed = omp_get_wtime();
  printf("time to write: %.4lfs\n", ed - st);
}

void print(long long int size) {
  for (long long int y = 0; y < size; y++) {
    for (long long int x = 0; x < size; x++)
      printf("%10d ", pixel(x,y));
    printf("\n");
    fflush(stdout);
  }
}
