/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

#define MIN(X,Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X,Y) (((X) < (Y)) ? (Y) : (X))

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N]) {
    int b = 8, starti, startj;
    // upper right part of A, loop by A
    int i, j, k;
    for (starti = 0; starti < M; starti += b) {
        for (startj = starti; startj < N; startj += b) {
            // fill cache first
            for (k = 0; k < b; k++) {
                B[startj + k][starti + k] = A[starti + k][startj + k];
            }

            // cache friendly, skewed rectangle
            for (j = startj + 1; j < MIN(startj+2*b, N); j++) {
                for (i = starti + MAX(0, j-startj-b+1); i < starti + MIN(j-startj, b); i++) {
                    B[j][i] = A[i][j];
                }
            }
        }
    }

    // lower left part of A, loop by B
    for (starti = 0; starti < M; starti += b) {
        for (startj = starti; startj < N; startj += b) {
            // fill cache first
            for (k = 0; k < b; k++) {
                B[starti + k][startj + k] = A[startj + k][starti + k];
            }

            // cache friendly, skewed rectangle
            for (j = startj + 1; j < MIN(startj+2*b, N); j++) {
                for (i = starti + MAX(0, j-startj-b+1); i < starti + MIN(j-startj, b); i++) {
                    B[i][j] = A[j][i];
                }
            }
        }
    }
}

char transpose_combine_desc[] = "Transpose skewed rectangle as a whole";
void transpose_combine(int M, int N, int A[N][M], int B[M][N]) {
    // printf("address of A: %p, address of B: %p, difference: %ld\n", A, B, B - A);
    int b = 8, a = 8, starti, startj;
    // upper right part of A, loop by A
    int i, j, k;
    for (starti = 0; starti < M; starti += a) {
        for (startj = starti; startj < N; startj += b) {
            // printf("starti: %d, startj: %d\n", starti, startj);
            // fill cache first
            for (k = 0; k < a; k++) {
                if (startj+k < N && starti+k < M) {
                    B[startj + k][starti + k] = A[starti + k][startj + k];
                }
            }

            // cache friendly, skewed rectangle
            for (j = startj + 1; j < MIN(startj+b+a, N); j++) {
                for (i = starti + MAX(0, j-startj-b+1); i < starti + MIN(j-startj, a); i++) {
                    B[j][i] = A[i][j];
                }
            }
        }
    }

    // lower left part of A, loop by B
    for (starti = 0; starti < M; starti += a) {
        for (startj = starti; startj < N; startj += b) {
            // fill cache first
            for (k = 0; k < a; k++) {
                if (startj+k < N && starti+k < M) {
                    B[starti + k][startj + k] = A[startj + k][starti + k];
                }
            }

            // cache friendly, skewed rectangle
            for (j = startj + 1; j < MIN(startj+b+a, N); j++) {
                for (i = starti + MAX(0, j-startj-b+1); i < starti + MIN(j-startj, a); i++) {
                    B[i][j] = A[j][i];
                }
            }
        }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char transpose_verbose_desc[] = "Transpose split";
void transpose_verbose(int M, int N, int A[N][M], int B[M][N])
{
    int b = 8, starti, startj;
    // upper right part of A, loop by A
    int i, j, k;
    for (starti = 0; starti < M; starti += b) {
        for (startj = starti; startj < N; startj += b) {
            // fill cache first
            for (k = 0; k < b; k++) {
                B[startj + k][starti + k] = A[starti + k][startj + k];
            }

            // cache friendly, left triangle
            for (j = startj + 1; j < startj + b; j++) {
                for (i = starti; i < starti + (j-startj); i++) {
                    B[j][i] = A[i][j];
                }
            }
            // right triangle if exist
            if (j < N) {
                for ( ; j < startj + 2 * b; j++) {
                    for (i = starti + (j - startj - b) + 1; i < starti + b; i++) {
                        B[j][i] = A[i][j];
                    }
                }
            }
        }
    }

    // lower left part of A, loop by B, so code can be reused
    for (starti = 0; starti < M; starti += b) {
        for (startj = starti; startj < N; startj += b) {
            // fill cache first, diagonal repeated, no big deal
            for (k = 0; k < b; k++) {
                B[starti + k][startj + k] = A[startj + k][starti + k];
            }

            // cache friendly, left triangle
            for (j = startj + 1; j < startj + b; j++) {
                for (i = starti; i < starti + (j-startj); i++) {
                    B[i][j] = A[j][i];
                }
            }
            // right triangle if exist
            if (j < N) {
                for ( ; j < startj + 2 * b; j++) {
                    for (i = starti + (j - startj - b) + 1; i < starti + b; i++) {
                        B[i][j] = A[j][i];
                    }
                }
            }
        }
    }
}

char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register combine function */
    registerTransFunction(transpose_combine, transpose_combine_desc); 
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

