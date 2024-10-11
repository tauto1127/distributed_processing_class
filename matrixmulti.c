#include <pthread.h>
#include <stdio.h>
#include <sys/_pthread/_pthread_t.h>
#include <unistd.h>

#define N 6
static int n[N][2];
int left_matrix[N][N];
int right_matrix[N][N];
int result_matrix[N][N];

void *func(void *arg);
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
  for (int j = 0; j < 1; j++) {
    for (i = 0; i < N; i++) {
      int r;
      int *columnPointer = left_matrix[j];
      int raw[N] = {};
      for (int k = 0; k < N; k++) {
        raw[k] = right_matrix[k][i];
        printf("%d\n", raw[k]);
      }
      printf("\n");

      struct calculateLineArgs args = {columnPointer, raw,
                                       &result_matrix[j][i]};

      // スレッドの管理変数, スレッドの属性, スレッドの関数,
      // スレッドの引数(汎用ポインタ(どんなポインタでも受け取れる))
      r = pthread_create(&th[i], NULL, calculateLine, (void *)&args);
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
  for (int i = 0; i < N; i++) {
    printf("%d,\n", rawPointer[i]);
  }
  printf("columnPointer\n");
  for (int i = 0; i < N; i++) {
    printf("%d", columnPointer[i]);
  }
  printf("\n");

  *result = 0;
  for (int i = 0; i < N; i++) {
    *result += *(columnPointer + 1) * (rawPointer[i]);
  }
  pthread_exit(NULL);
}

void *func(void *arg) {
  int n = *(int *)arg;

  sleep(1);
  printf("Hello, I am thread-%02d.\n", n);
  pthread_exit(NULL);
}
