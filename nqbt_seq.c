/*
 * 並列分散処理 第5回課題
 * Dec. 17th, 2024
 */
#include <stdio.h>
#include <stdlib.h>

#define MAX_N (28)

int check_and_set(unsigned int mat[], int n, int row, int col)
{
	int i, j, c = 0;

	{
	/* horizontal */
	for (j = col - 1; j >= 0; j--) {
		if (mat[j] & (1U << row)) {
			return c;
		}
	}
	}

	/* obliquely upward */
	for (i = row - 1, j = col - 1; j >= 0 && i >= 0; j--, i--) {
		if (mat[j] & (1U << i)) {
			return c;
		}
	}

	/* diagonally downward */
	for (i = row + 1, j = col - 1; j >= 0 && i < n; j--, i++) {
		if (mat[j] & (1U << i)) {
			return c;
		}
	}

	/* set */
	mat[col] = 1 << row;

	if (col == n - 1) {
		/* completed */
		c++;
	} else {
		/* set remain columns */
		for (i = n - 1; i >= 0; i--)
			c += check_and_set(mat, n, i, col + 1);
	}
	mat[col] = 0U;

	return c;
}

int main(int argc, char *argv[])
{
	unsigned int mat[MAX_N], n;
	int i, count = 0;

	/* check argument */
	if (argc < 2) {
		fprintf(stderr, "Usage: %s number\n", argv[0]);
		return -1;
	}

	/* obtain n */
	n = atoi(argv[1]);
	if ((n < 2) || (n > MAX_N)) {
		fprintf(stderr, "You should specify a number "
			"between 2 and %d.\n", MAX_N);
		return -1;
	}

	/* initialize */
	#pragma omp parallel for private(i)
	for (i = 0; i < n; i++)
		mat[i] = 0U;

	/* query */
	#pragma omp parallel for private(i) reduction(+:count)
	for (i = n - 1; i >= 0; i--)
		count += check_and_set(mat, n, i, 0);

	printf("Total answer for %d x %d = %d\n", n, n, count);

	return 0;
}
