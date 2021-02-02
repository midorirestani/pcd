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
#define N 8

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

int getNeighboor(int grid[N], int pcell, int k, int l)
{
    int neighboor = getRowLocation(pcell, k) + getColumnLocation(pcell, l);
}

int main(int argc, char** argv)
{
    int process_rank, cluster_size;
    int new_grid[N];
    int i;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &cluster_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    // inicializa grid, processo 0 será o root (quem vai guardar o grid)
    int *grid = NULL;
    if (process_rank == 0) {
        grid = (int*)malloc(N * sizeof(int));
    }
    int chunk = N / cluster_size;
    int *chunk_grid = (int*)malloc(chunk * sizeof(int));

    for (i = 0; i < chunk; i++) {
        // Obs: essa linha é só para testes 
        chunk_grid[i] = process_rank;

        // Inicializa o chunk aleatoriamente
        // chunk_grid[i] = rand() % 2;
    }

    // Envia os chunks de todos os processos para o processo 0 juntar
    MPI_Gather(chunk_grid, chunk, MPI_INT, grid, chunk, MPI_INT, 0, MPI_COMM_WORLD);
    
    // TODO: Atualizar grid a cada Geração
    MPI_Barrier(MPI_COMM_WORLD);
    if (process_rank == 0) {
        for (i = 0; i < N; i++) {
            printf("process %d> grid[%d] = %d\n", process_rank, i, grid[i]);
        }
    }

    // limpeza de variáveis
    MPI_Barrier(MPI_COMM_WORLD);
    if (process_rank == 0)
        free(grid);
    free(chunk_grid);
    
    MPI_Finalize();
    return 0;
}
