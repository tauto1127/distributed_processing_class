#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
static long num_rects = 2 * 1000 * 1000 * 1000;
//static long num_rects = 2 * 1000 * 1000 ;
void *calculate(void *arg);
#define NUM_THREADS 21

struct calculateArgs {
  double width;
  long *for_length;
  long for_offset;
  double sum;
};

int main(int argc, char *argv[]) {
  // midとheightはスレッドごとに独立させる必要がある
  double mid, height, width, sum = 0.0;
  double area;
  width = 1.0 / (double)num_rects;
  printf("widthは%f\n", width);
  printf("num_rectsは%ld\n", num_rects);

  long compute = num_rects / NUM_THREADS;
  long compute_mod = num_rects % NUM_THREADS;
  long for_length[NUM_THREADS] = {};

  for (int i = 0; i < NUM_THREADS; i++) {
    for_length[i] = compute;
    if (compute_mod > 0) {
      for_length[i]++;
      compute_mod--;
    }
  }

  long thread_compute_offset = 0;
  pthread_t th[NUM_THREADS];
  int r;

  for (int i = 0; i < NUM_THREADS; i++) {
    struct calculateArgs *args = malloc(sizeof(struct calculateArgs));
    args->width = width;
    args->for_length = &for_length[i];
    args->sum = sum;
    args->for_offset = thread_compute_offset;
    r = pthread_create(&th[i], NULL, calculate, (void *) args);
    if (r != 0) {
      perror("Failed to create thread");
    }
    thread_compute_offset += for_length[i];
  }
  for (int i = 0; i < NUM_THREADS; i++) {
      struct calculateArgs *args;
      // sum変数を直接編集するようにすると，スレッド間で競合が発生したため，ここのfor文でsumに加算する
    pthread_join(th[i], (void**)&args);
    sum += args->sum;
    free(args);
    printf("%f", sum);
  }
  printf("done.\n");
  printf("Computed pi = %f\n", sum * width);

  return 0;
}
void *calculate(void *arg) {
  struct calculateArgs *args = (struct calculateArgs *)arg;
  long for_length = *args->for_length;
  double width = args->width;
  long for_offset = args->for_offset;
    double height = 0.0;
  double mid;

  printf("calculate : %ld, %ld, %f\n", for_length, for_offset, width);

  for (long i = for_offset; i < for_offset + for_length; i++) {
    mid = (i + 0.5) * width;
    height = 4.0 / (1.0 + mid * mid);
    args->sum += height;
  }
  pthread_exit(arg);
}
