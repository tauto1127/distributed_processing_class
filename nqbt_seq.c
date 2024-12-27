/*
 * 並列分散処理 第5回課題
 * Dec. 17th, 2024
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_N (28)

int check_and_set(unsigned int mat[], int n, int row, int col)
{
    int i, j, c = 0;

    /* horizontal */
    for (j = col - 1; j >= 0; j--) {
        if (mat[j] & (1U << row)) {
            return c;
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
        unsigned int mat_copy[MAX_N];
        #pragma omp parallel for reduction(+:c) private(i, mat_copy)schedule(static)
        for (i = n - 1; i >= 0; i--) {
            memcpy(mat_copy, mat, sizeof(unsigned int) * MAX_N);
            c += check_and_set(mat_copy, n, i, col + 1);
        }
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
        return 1;
    }

    n = atoi(argv[1]);
    if (n > MAX_N) {
        fprintf(stderr, "number must be less than or equal to %d\n", MAX_N);
        return 1;
    }

    #pragma omp parallel for reduction(+:count) private(i) schedule(static)
    for (i = 0; i < n; i++) {
        unsigned int mat_copy[MAX_N] = {0};
        count += check_and_set(mat_copy, n, i, 0);
    }

    printf("Total solutions: %d\n", count);

    return 0;
}
