#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

#define SRAND_VALUE 1985
#define GENERATIONS 2000
#define N 2028
#define SIZE N *N

long int getRowLocation(long int i, long int neighbor_p)
{
    i += neighbor_p;
    printf("i=%d\n", i);
    if (i < 0)
        return i + N;
    if (i >= N)
        return i - N;
    return i;
}

long int getColumnLocation(long int j, long int neighbor_p)
{
    j += neighbor_p;
    //printf("j=%d\n", j);
    if (j < 0)
        return j + N;
    if (j >= N)
        return j - N;
    return j;
}

long int getNeighbor(long int pcell, long int k, long int l)
{
    int neighbor = N * getRowLocation(pcell % N - 1, k) + getColumnLocation(pcell % N, l);
    //printf("neighbor = %d\n", neighbor);
    return neighbor;
}

long int countNeighbors(int *grid, int pcell)
{
    long int count = 0;
    for (int k = -1; k < 2; k++)
    {
        for (int l = -1; l < 2; l++)
        {
            if (k || l)
                count += grid[getNeighbor(pcell, k, l)];
        }
    }
    return count;
}

long int *run_game(long int *grid)
{
    int neighbors, i, j, k;
    int new_grid[SIZE];
    for (i = 0; i < GENERATIONS; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            neighbors = countNeighbors(grid, j);
            //printf("neighbors = %d\n", neighbors);

            if (neighbors < 2 || neighbors > 3) //a célula morre por abandono ou por superpopulação
                new_grid[j] = 0;
            else if (neighbors == 2) //mantém o estado da célula
                new_grid[j] = 3;     //grid[j];
            else if (neighbors == 3) //a célula vive
                new_grid[j] = 1;
        }
        for (k = 0; k < SIZE; k++)
        {
            //printf("%d ", new_grid[i]);
            grid[k] = new_grid[k];
        }
    }
    long int count = 0;
    for (i = 0; i < SIZE; i++)
    {
        if (grid[i])
            count++;
    }
    return grid;
}
int main(int argc, char const *argv[])
{
    long int *grid = malloc((SIZE) * sizeof(int));
    long int *new_grid = malloc((SIZE) * sizeof(int));
    long int i;

    printf("N*N = %d\nSIZE = %d\n", N * N, SIZE);

    srand(SRAND_VALUE);
    for (i = 0; i < SIZE; i++)
    {
        grid[i] = rand() % 2;
    }

    new_grid = run_game(grid, 2);

    return 0;
}
