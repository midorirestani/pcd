/*
**  Nome: Gabriela Midori Restani R.A:104105
**  Nome: Júlia Scucuglia R.A:102030
**  Nome: Klara Sati Kague R.A:
**  UC:Programação Concorrente e Distribuída
**  Laboratório 01: Jogo da vida
**  Versão Sequêncial
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#define N 2048
#define SRAND_VALUE 1985
#define GENERATIONS 2000

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

void printMatrix(int **grid){
  for(int i=0; i<N; i++){
      printf("| " );
      for(int j=0; j<N; j++){
        printf("%d ", grid[i][j]);
      }
      printf("|\n");
  }
}

int main(int argc, char const *argv[]) {
  int **grid, **newgrid;
  int i, j;
  long int count=0;

  struct timeval inicio, final;
  int tmili;


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
      if(grid[i][j])count++;
    }
  }

  gettimeofday(&inicio, NULL);
  int neighbors;
  for(int gen=0; gen<GENERATIONS; gen++){
    count = 0;
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
    for(i = 0; i<N; i++)
      for(j = 0; j<N; j++){
        grid[i][j] = newgrid[i][j];
        if(newgrid[i][j]) count++;
      }
  }
  gettimeofday(&final, NULL);
  tmili = (int) (1000*(final.tv_sec - inicio.tv_sec) + (final.tv_usec - inicio.tv_usec)/1000);


  free(newgrid);
  free(grid);



  printf("tempo de corrido: %d ms\n",tmili );
  printf("Resultado: %ld células vivas na %d geração\n", count, GENERATIONS);
  return 0;
}
