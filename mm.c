#include <stdio.h>
#define N 4
 
// This function multiplies A[][] and B[][], and stores
// the result in C[][]
void multiply(int A[][N], int B[][N], int C[][N])
{
    int i, j, k;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            for (k = 0; k < N; k++)
                C[i][j] += A[i][k]*B[k][j];
        }
    }
}

//__attribute__((target_clones("avx2","arch=atom","default"))) 
int main()
{
    int A[N][N] = { {1, 1, 1, 1},
                    {2, 2, 2, 2},
                    {3, 3, 3, 3},
                    {4, 4, 4, 4}};
 
    int B[N][N] = { {1, 1, 1, 1},
                    {2, 2, 2, 2},
                    {3, 3, 3, 3},
                    {4, 4, 4, 4}};
 
    int C[N][N]; // To store result
    int i, j;
    multiply(A, B, C);
    printf("Result matrix is \n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++)
           printf("%d ", C[i][j]);
        printf("\n");
    }
 
    return 0;
}
