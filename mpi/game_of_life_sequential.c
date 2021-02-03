/*
**  Nome: Gabriela Midori Restani R.A:104105
**  Nome: Júlia Scucuglia R.A:102030
**  Nome: Klara Sati Kague R.A:
**  UC:Programação Concorrente e Distribuída
**  Docente: Profº Dr. Álvaro Fazenda
**  Laboratório 03: Jogo da vida MPI
**  Versão Sequencial Utilizando Array
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

#define SRAND_VALUE 1985
#define GENERATIONS 2000
#define N 2048
#define SIZE N *N

long int getRowLocation(long int i, long int neighbor_p)
{
    i += neighbor_p;
    //printf("i=%ld\n", i);
    if (i < 0)
        return i + N;
    if (i >= N)
        return i - N;
    return i;
}

long int getColumnLocation(long int j, long int neighbor_p)
{
    j += neighbor_p;
    //printf("j=%ld\n", j);
    if (j < 0)
        return j + N;
    if (j >= N)
        return j - N;
    return j;
}

long int getNeighbor(long int pcell, long int k, long int l)
{
    long int neighbor = N * getRowLocation(pcell / N, k) + getColumnLocation(pcell % N, l);
    //printf("neighbor = %d\n", neighbor);
    return neighbor;
}

long int countNeighbors(long int *grid, int pcell)
{
    long int count = 0;
    for (long int k = -1; k < 2; k++)
    {
        for (long int l = -1; l < 2; l++)
        {
            if (k || l)
                count += grid[getNeighbor(pcell, k, l)];
        }
    }
    return count;
}

long int run_game(long int *grid)
{
    long int neighbors, i, j, k;
    long int *new_grid = malloc(SIZE * sizeof(long int));
    for (i = 0; i < GENERATIONS; i++)
    {
        printf("generation = %ld\n", i);
        for (j = 0; j < SIZE; j++)
        {
            neighbors = countNeighbors(grid, j);
            //printf("neighbors = %d\n", neighbors);

            if (neighbors < 2 || neighbors > 3) //a célula morre por abandono ou por superpopulação
                new_grid[j] = 0;
            else if (neighbors == 2) //mantém o estado da célula
                new_grid[j] = grid[j];
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
    return count;
}
int main(int argc, char const *argv[])
{
    long int *grid = malloc((SIZE) * sizeof(long int));
    long int result = 0;
    long int i;

    printf("N*N = %d\nSIZE = %d\n", N * N, SIZE);

    srand(SRAND_VALUE);
    for (i = 0; i < SIZE; i++)
    {
        grid[i] = rand() % 2;
    }

    result = run_game(grid);

    printf("result = %li", result);
    free(grid);
    return 0;
}
