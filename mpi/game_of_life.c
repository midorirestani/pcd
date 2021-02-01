/*
**  Nome: Gabriela Midori Restani R.A:104105
**  Nome: Júlia Scucuglia R.A:102030
**  Nome: Klara Sati Kague R.A:
**  UC:Programação Concorrente e Distribuída
**  Docente: Profº Dr. Álvaro Fazenda
**  Laboratório 01: Jogo da vida
**  Versão MPI
*/
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <assert.h>

#define SRAND_VALUE 1985
#define GENERATIONS 2000
#define N 2048

int getRowLocation(int pcell, int k)
{
    int row = pcell + N * k;
    if (row < 0)
        return N - 1;
    if (row > N)
        return 0;
}

int getColumnLocation(int pcell, int l)
{
    return 0;
}
int getNeighboor(int grid[N], int pcell, int k, int l)
{
    int rlocation = pcell % (N - 1);
    int clocation = pcell % (N);
}

int main(int argc, char const *argv[])
{
    int process_rank, cluster_size;
    int grid[N];
    int new_grid[N];
    int i;

    srand(SRAND_VALUE);
    for (i = 0; i < N; i++)
    {
        grid[i] = rand() % 2;
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &cluster_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    return 0;
}
