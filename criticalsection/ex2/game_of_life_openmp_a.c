/*
**  Nome: Gabriela Midori Restani R.A:104105
**  Nome: Júlia Scucuglia R.A:102030
**  Nome: Klara Sati Kague R.A:
**  UC:Programação Concorrente e Distribuída
**  Docente: Profº Dr. Álvaro Fazenda
**  Laboratório 01: Jogo da vida
**  Versão OpenMP
*/

/*
** A partir do programa Jogo da Vida já desenvolvido em C/C++ e OpenMP na atividade de programação anterior , 
** modifique o  procedimento/função (ou trecho de código) que realiza a somatória de todas as posições da 
** última geração do tabuleiro (soma a quantidade total de células vivas no tabuleiro),
** utilizando a diretiva #pragma omp critical para realizar a operação entre as threads, 
** totalizando os resultados em uma variável global ao final;
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#ifdef _OPENMP
  #include <omp.h>
#endif

#define N 2048
#define SRAND_VALUE 1985
#define GENERATIONS 2000
#define MAX_THREADS 1

int getLine(int i, int neighbor_p){
  i+=neighbor_p;
  if(i<0)
    return i+N;
  if(i>=N)
    return i-N;
  return i;
}

int getCol(int j, int neighbor_p){
  j+=neighbor_p;
  if(j<0) 
    return j+N;
  if(j>=N)
    return j-N;
  return j;
}


int getNeighbors(int** grid, int i, int j){
  int neighbors = 0;

  for(int k = -1; k<=1; k++)
    for(int l=-1; l<=1; l++)
        if(k||l)
          if(grid[getLine(i,k)][getCol(j,l)])
            neighbors++;

  return neighbors;
}

int **grid, **newgrid, r;
long int result =0;

int main(){

  int i, j;

  grid = (int**)malloc(N*sizeof(int*));
  newgrid = (int**)malloc(N*sizeof(int*));

  for(int i = 0; i<N; i++){
    grid[i] = (int*)malloc(N*sizeof(int));
    newgrid[i] = (int*)malloc(N*sizeof(int));
  }

  srand(SRAND_VALUE);
  for(i = 0; i<N; i++) { //laço sobre as células do tabuleiro sem contar com um eventual halo
    for(j = 0; j<N; j++) {
      grid[i][j] = rand() % 2;
    }
  }
  //tempo inicial
  double start = omp_get_wtime();
  int neighbors;
  for(i=0; i<GENERATIONS; i++){
    result = 0;
    #pragma omp parallel num_threads(MAX_THREADS)
    {
      #pragma omp for private(i,j, neighbors)
      for(i = 0; i<N; i++){
        for(j = 0; j<N; j++){
          neighbors = getNeighbors(grid, i, j);

          if(neighbors < 2 || neighbors > 3)//a célula morre por abandono ou por superpopulação
            newgrid[i][j] = 0;
          else if(neighbors == 2) //mantém o estado da célula
            newgrid[i][j] = grid[i][j];
          else if(neighbors == 3) //a célula vive
            newgrid[i][j] = 1;
        }
      }

      #pragma omp for private (i,j)
      for(i = 0; i<N; i++)
      for(j = 0; j<N; j++){
        grid[i][j] = newgrid[i][j];
      }
    }
  }
  //tempo final
  double end = omp_get_wtime();

  #pragma omp parallel num_threads(MAX_THREADS)
  #pragma omp for private (i,j)
  for(i = 0; i<N; i++){
    for(j = 0; j<N; j++){
      if(newgrid[i][j]) 
        #pragma omp critical
        result++;
    }
  }

  printf("A quantidade de celulas vivas na %d geração é: %ld\n",GENERATIONS, result);
  printf("tempo de execução: %f segundos\n", end-start);
  free(grid);
  free(newgrid);

  return 0;
}
