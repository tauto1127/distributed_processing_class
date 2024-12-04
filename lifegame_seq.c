/*
 * The Life Game for 2024 FUN class
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define NLOOP	(200)
// グリッドN行
#define N	(16384)
// グリッドM列
#define M	(16384)
#define ALIVE	(1)
#define DEAD	(0)

typedef int Grid[N + 2][M + 2];

struct thread_arg {
	int i;
	const Grid * pCur;
	Grid * pNext;
};

void computeAliveOrDead(int i, int j, const Grid * pCur, Grid * pNext)
{
	int count = 0;

	/* NW neighbor */
	if ((*pCur)[i - 1][j - 1] == ALIVE)
		count++;
	/* N neighbor */
	if ((*pCur)[i - 1][j] == ALIVE)
		count++;
	/* NE neighbor */
	if ((*pCur)[i - 1][j + 1] == ALIVE)
		count++;
	/* W neighbor */
	if ((*pCur)[i][j - 1] == ALIVE)
		count++;
	/* E neighbor */
	if ((*pCur)[i][j + 1] == ALIVE)
		count++;
	/* SW neighbor */
	if ((*pCur)[i + 1][j - 1] == ALIVE)
		count++;
	/* S neighbor */
	if ((*pCur)[i + 1][j] == ALIVE)
		count++;
	/* SE neighbor */
	if ((*pCur)[i + 1][j + 1] == ALIVE)
		count++;

	if (count <= 1 || count >= 4)
		(*pNext)[i][j] = DEAD;
	else if ((*pCur)[i][j] == ALIVE &&
		 (count == 2 || count == 3))
		(*pNext)[i][j] = ALIVE;
	else if ((*pCur)[i][j] == DEAD && count == 3)
		(*pNext)[i][j] = ALIVE;
	else
		(*pNext)[i][j] = DEAD;
}

void *thread_func(void *arg){
	int j;
	struct thread_arg *targ = (struct thread_arg *)arg;

	// 列ループ
	for (j = 1; j <= M; ++j) {
		computeAliveOrDead(targ->i, j, targ->pCur, targ->pNext);
	}
	pthread_exit(NULL);
}

void computeNextGen(const Grid * pCur, Grid * pNext)
{
	int i, j;
	pthread_t th[N];
	pthread_t th2[N];

	// 行ループ
	for (i = 1; i <= N; ++i) {
		struct thread_arg *targ = (struct thread_arg *)malloc(sizeof(struct thread_arg));
		targ->i = i;
		targ->pCur = pCur;
		targ->pNext = pNext;
		if(i % 2 == 0){
			pthread_create(&th[i], NULL, thread_func, (void *)targ);
		}
		if(i % 2 == 1){
			pthread_create(&th2[i], NULL, thread_func, (void *)targ);
		}
	}
	
	for (i = 1; i <= N; ++i) {
		if(i % 2 == 0){
			pthread_join(th[i], NULL);
		}
	}
	
	for (i = 1; i <= N; ++i) {
		if(i % 2 == 1){
			pthread_join(th2[i], NULL);
		}
	}
}

// 次の画面までの平均描画時間が5秒から2秒になった！
// 6core
// CPU使用率も50くらいから90くらいになった　
// ベストは100%?
// 
// 列ごとに分散して計算する戦略．
// データの整合性を保つために，一列間隔をあけて計算してる．
// 間隔開ける意味はなかった気がするけど
int main(int argc, char *argv[])
{
	static Grid g[2];
	int i, j, n, cur = 0;
	

	time_t start_time, end_time;

	printf("\033[2J");	/* clear screen */
	start_time = time(NULL);
	for (i = 0; i <= N + 1; ++i) {
		for (j = 0; j <= M + 1; ++j) {
			g[0][i][j] = random() & 1;
		}
		printf("Initializing g[%06d]...\r", i);
	}
	end_time = time(NULL);

	for (n = 0; n < NLOOP; n++) {
		printf("\033[2J");	/* clear screen */
		printf("n = %d, time:%ld\n", n, end_time - start_time);

		//表示するグリッドを制限？
		for (i = 1; i <= N >> 9; ++i) {
			//表示するグリッドを制限？
			for (j = 1; j <= M >> 6; ++j) {
				printf("%c", g[cur][i][j] == ALIVE ? '@' : '.');
			}
			printf("\n");
		}

		start_time = time(NULL);
		computeNextGen(&g[cur], &g[(cur + 1) & 1]);
		end_time = time(NULL);
		cur = (cur + 1) & 1;
	}

	return 0;
}
