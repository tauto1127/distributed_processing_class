#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/_pthread/_pthread_t.h>
#include <unistd.h>

#define N 6
static int n[N][2];
int left_matrix[N][N];
int right_matrix[N][N];
int result_matrix[N][N];

void *calculateLine(void *arg);

struct calculateLineArgs {
  int *columnPointer;
  int *rawPointer; // 二次元配列のポインターとして正しく宣言
  int *result;
};

int main(int argc, char *argv[]) {
  int left_matrix[6][6] = {{1, 2, 3, 4, 5, 6}, {2, 3, 4, 5, 6, 7},
                           {3, 4, 5, 6, 7, 8}, {4, 5, 6, 7, 8, 9},
                           {5, 6, 7, 8, 9, 0}, {6, 7, 8, 9, 0, 1}};

  int right_matrix[6][6] = {{9, 8, 7, 6, 5, 4}, {8, 7, 6, 5, 4, 3},
                            {7, 6, 5, 4, 3, 2}, {6, 5, 4, 3, 2, 1},
                            {5, 4, 3, 2, 1, 0}, {4, 3, 2, 1, 0, 9}};

  // スレッドの管理変数
  pthread_t th[N];
  int i;
  for (int j = 0; j < N; j++) {
    for (i = 0; i < N; i++) {
      int r;
      // int *columnPointer = left_matrix[j];
      // malloc使わんとバグる
      int *columnPointer = (int *)malloc(sizeof(int) * N);
      for (int k = 0; k < N; k++) {
        columnPointer[k] = left_matrix[j][k];
      }
      int *raw = (int *)malloc(sizeof(int) * N);
      for (int k = 0; k < N; k++) {
        raw[k] = right_matrix[k][i];
      }

      // struct calculateLineArgs args = {columnPointer, raw,
      //                                  &result_matrix[j][i]};
      struct calculateLineArgs *args = malloc(sizeof(struct calculateLineArgs));
      args->columnPointer = columnPointer;
      args->rawPointer = raw;
      args->result = &result_matrix[j][i];

      // スレッドの管理変数, スレッドの属性, スレッドの関数,
      // スレッドの引数(汎用ポインタ(どんなポインタでも受け取れる))
      r = pthread_create(&th[i], NULL, calculateLine, (void *)args);
      if (r != 0)
        perror("new thread");
    }
  }

  for (i = 0; i < N; i++) {
    pthread_join(th[i], NULL);
  }
  printf("done.\n");

  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      printf("%d ", result_matrix[i][j]);
    }
    printf("\n");
  }

  return 0;
}

void *calculateLine(void *arg) {
  struct calculateLineArgs *args = (struct calculateLineArgs *)arg;
  int *columnPointer = args->columnPointer;
  int *rawPointer = args->rawPointer;
  int *result = args->result;

  // printf("(1,1) = %d\n", *(rawPointer[1] + 1));

  *result = 0;
  for (int i = 0; i < N; i++) {
    *result += *(columnPointer + i) * (rawPointer[i]);
  }
  pthread_exit(NULL);
}
