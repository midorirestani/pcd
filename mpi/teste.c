#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

#define SRAND_VALUE 1985
#define GENERATIONS 2000
#define N 3
#define SIZE N *N

int getRowLocation(int pcell, int k)
{
    int row = pcell + N * k;
    if (row < 0)
        return N - 1;
    else if (row > N)
        return 0;
    else
        return row;
}

int getColumnLocation(int pcell, int l)
{

    if (pcell % N == 0 && l < 0)
        return N - 1;
    else if (pcell % N == N - 1 && l > 0)
        return 1 - N;
    else
        return l;
}

int getNeighbor(int pcell, int k, int l)
{
    int neighbor = getRowLocation(pcell, k) + getColumnLocation(pcell, l);
    printf("neighbor = %d\n", neighbor);
    return neighbor;
}

int countNeighbors(int *grid, int pcell)
{
    int count = 0;
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

void run_game(int *grid, int generations)
{
    int neighbors, i, j, k;
    int new_grid[SIZE];
    for (i = 0; i < generations; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            neighbors = countNeighbors(grid, j);

            if (neighbors < 2 || neighbors > 3) //a célula morre por abandono ou por superpopulação
                new_grid[j] = 0;
            else if (neighbors == 2) //mantém o estado da célula
                new_grid[j] = grid[j];
            else if (neighbors == 3) //a célula vive
                new_grid[j] = 1;
        }
        for (k = 0; k < N; k++)
            grid[k] = new_grid[k];
    }
}
int main(int argc, char const *argv[])
{
    int grid[SIZE];     //= malloc((SIZE) * sizeof(int));
    int new_grid[SIZE]; //= malloc((SIZE) * sizeof(int));
    int i;

    printf("N*N = %d\nSIZE = %d\n", N * N, SIZE);

    srand(SRAND_VALUE);
    for (i = 0; i < SIZE; i++)
    {
        grid[i] = rand() % 2;
        if (i % N == 0 && i != 0)
        {
            printf("\n");
        }
        printf("%d ", grid[i]);
    }

    run_game(grid, 1);

    for (i = 0; i < SIZE; i++)
    {
        if (i % N == 0)
        {
            printf("\n");
        }
        printf("%d ", grid[i]);
    }
    return 0;
}
