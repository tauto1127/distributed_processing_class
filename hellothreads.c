#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define N 16
static int n[N];

void *func(void *arg);

int main(int argc, char *argv[]) {
  // スレッドの管理変数
  pthread_t th[N];
  int i;

  for (i = 0; i < N; i++) {
    int r;
    n[i] = i + 1;
    // スレッドの管理変数, スレッドの属性, スレッドの関数, スレッドの引数
    r = pthread_create(&th[i], NULL, func, (void *)&n[i]);
    if (r != 0)
      perror("new thread");
  }

  for (i = 0; i < N; i++) {
    pthread_join(th[i], NULL);
  }
  printf("done.\n");

  return 0;
}

void *func(void *arg) {
  int n = *(int *)arg;

  // スリープなどでの誤差具合で出力順が変わることがある
  sleep(1);
  printf("Hello, I am thread-%02d.\n", n);
  pthread_exit(NULL);
}
