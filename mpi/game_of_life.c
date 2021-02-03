/*
**  Nome: Gabriela Midori Restani R.A:104105
**  Nome: Júlia Scucuglia R.A:102030
**  Nome: Klara Sati Kague R.A:
**  UC:Programação Concorrente e Distribuída
**  Docente: Profº Dr. Álvaro Fazenda
**  Laboratório 03: Jogo da vida
**  Versão MPI
*/
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <assert.h>

#define SRAND_VALUE 1985
#define GENERATIONS 2000
#define N_DIM 2048
#define N N_DIM *N_DIM

int count_vizinhos(int *grid, int x)
{
    int count = 0;

    int primeira_linha = x < N_DIM ? 1 : 0;
    int ultima_linha = x >= N - N_DIM ? 1 : 0;
    int primeira_coluna = x % N_DIM == 0 ? 1 : 0;
    int ultima_coluna = (x + 1) % N_DIM == 0 ? 1 : 0;

    // somando os vizinhos A,B,C,D,E,F,G,H em formato
    // A  B  C
    // H  x  D
    // G  F  E

    if (primeira_linha)
    {
        if (primeira_coluna)
        {
            // x = 0
            count = grid[N - 1] + grid[N - N_DIM] + grid[N - N_DIM + 1] + grid[1] + grid[N_DIM + 1] + grid[N_DIM] + grid[N_DIM + N_DIM - 1] + grid[N_DIM - 1];
        }
        else if (ultima_coluna)
        {
            // x = N_DIM - 1
            count = grid[N - 2] + grid[N - 1] + grid[N - N_DIM] + grid[0] + grid[x + 1] + grid[x + N_DIM] + grid[x + N_DIM - 1] + grid[x - 1];
        }
        else
        {
            count = grid[x + N - N_DIM - 1] + grid[x + N - N_DIM] + grid[x + N - N_DIM + 1] + grid[x + 1] + grid[x + N_DIM + 1] + grid[x + N_DIM] + grid[x + N_DIM - 1] + grid[x - 1];
        }
    }
    else if (ultima_linha)
    {
        if (primeira_coluna)
        {
            count = grid[x - 1] + grid[x - N_DIM] + grid[x - N_DIM + 1] + grid[x + 1] + grid[1] + grid[0] + grid[N_DIM - 1] + grid[N - 1];
        }
        else if (ultima_coluna)
        {
            count = grid[x - 1 - N_DIM] + grid[x - N_DIM] + grid[x - N_DIM - N_DIM + 1] + grid[x - N_DIM + 1] + grid[0] + grid[N_DIM - 1] + grid[N_DIM - 2] + grid[x - 1];
        }
        else
        {
            count = grid[x - N_DIM - 1] + grid[x - N_DIM] + grid[x - N_DIM + 1] + grid[x + 1] + grid[x + N_DIM - N + 1] + grid[x + N_DIM - N] + grid[x + N_DIM - N - 1] + grid[x - 1];
        }
    }
    else
    {
        if (primeira_coluna)
        {
            count = grid[x - 1] + grid[x - N_DIM] + grid[x - N_DIM + 1] + grid[x + 1] + grid[x + N_DIM + 1] + grid[x + N_DIM] + grid[x + N_DIM + N_DIM - 1] + grid[x + N_DIM - 1];
        }
        else if (ultima_coluna)
        {
            count = grid[x - N_DIM - 1] + grid[x - N_DIM] + grid[x + 1 - N_DIM - N_DIM] + grid[x + 1 - N_DIM] + grid[x + 1] + grid[x + N_DIM] + grid[x + N_DIM - 1] + grid[x - 1];
        }
        else
        {
            count = grid[x - N_DIM - 1] + grid[x - N_DIM] + grid[x - N_DIM + 1] + grid[x + 1] + grid[x + N_DIM + 1] + grid[x + N_DIM] + grid[x + N_DIM - 1] + grid[x - 1];
        }
    }

    return count;
}

int soma_grid(int *grid)
{
    int i, count;
    for (i = 0; i < N; i++)
    {
        count += grid[i];
    }
    return count;
}

int main(int argc, char **argv)
{
    int process_rank, cluster_size;
    int i, j;
    float tempo_execucao = 0.0;
    float tempo_inicializacao = 0.0;
    float aux;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &cluster_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    MPI_Barrier(MPI_COMM_WORLD);
    tempo_inicializacao -= MPI_Wtime();

    // Inicializa grid, processo 0 será o root (quem vai guardar o grid durante a inicializacao)
    int *grid = (int *)malloc(N * sizeof(int));

    int chunk = N / cluster_size;
    int *chunk_grid = (int *)malloc(chunk * sizeof(int));

    // Inicializa o chunk aleatoriamente
    for (i = 0; i < chunk; i++)
    {
        chunk_grid[i] = rand() % 2;
    }

    // Envia os chunks de todos os processos para o processo 0 juntar
    MPI_Gather(chunk_grid, chunk, MPI_INT, grid, chunk, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (process_rank == 0)
    {
        printf("Condicao inicial: %d\n", soma_grid(grid));
    }

    // Envia o grid inicializado para os outros processos
    MPI_Bcast(grid, N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    aux = MPI_Wtime();
    tempo_inicializacao += aux;
    tempo_execucao -= aux;

    // Loop de geracoes
    int step = process_rank * chunk;

    for (i = 0; i < GENERATIONS; i++)
    {
        for (j = 0; j < chunk; j++)
        {
            int grid_ref = j + step;
            int num_vizinhos = count_vizinhos(grid, grid_ref);

            if (grid[grid_ref] == 1)
            { // celula viva
                if (num_vizinhos < 2 || num_vizinhos > 3)
                {
                    chunk_grid[j] = 0;
                }
                else
                {
                    chunk_grid[j] = 1;
                }
            }
            else
            { // celula morta
                if (num_vizinhos == 3)
                {
                    chunk_grid[j] = 1;
                }
                else
                {
                    chunk_grid[j] = 0;
                }
            }
        }

        // Envia os chunk_grid calculados por cada processo para o 0 juntar
        MPI_Gather(chunk_grid, chunk, MPI_INT, grid, chunk, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);

        // Se for o processo 0, printa as metricas
        if (process_rank == 0)
        {
            printf("Geracao %d: %d\n", i + 1, soma_grid(grid));
        }

        // Envia o grid atualizado para os outros processos
        MPI_Bcast(grid, N, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    tempo_execucao += MPI_Wtime();

    if (process_rank == 0)
    {
        printf("Tempo de inicialização: %.2f segundos\n", tempo_inicializacao);
        printf("Tempo de execução: %.2f segundos\n", tempo_execucao);
    }
    // limpeza de variáveis
    MPI_Barrier(MPI_COMM_WORLD);
    free(grid);
    free(chunk_grid);

    MPI_Finalize();
    return 0;
}
