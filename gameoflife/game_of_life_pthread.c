/*
**  Nome: Gabriela Midori Restani R.A:104105
**  Nome: Júlia Scucuglia R.A:102030
**  Nome: Klara Sati Kague R.A:
**  UC:Programação Concorrente e Distribuída
**  Docente: Profº Dr. Álvaro Fazenda
**  Laboratório 01: Jogo da vida
**  Versão PThreads
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define MAX_THREADS 4
#define N 2048
#define GENERATIONS 2000
#define SRAND_VALUE 1985

int **grid, **newgrid, ratio = N/MAX_THREADS;

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

int getNeighbors(int **grid, int i, int j){
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

void *gameOfLife(void *thid){
  long int i, j, neighbors;
  long int thread_id = (long int) thid;

  for(i = thread_id*ratio; i<(thread_id+1)*ratio; i++){
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

}

void *refreshGrid(void *thid){
  long int i, j;
  long int thread_id = (long int) thid;
  
  for(i=thread_id*ratio; i<ratio*(thread_id+1); i++){
    for(j=0; j<N; j++){
      grid[i][j] = newgrid[i][j];
    }
  }
}

long int getResult(){
  long int result = 0;
  for(int i=0; i<N; i++)
    for(int j=0; j<N; j++)
      if(grid[i][j])result++;

  return result;
}

int main(int argc, char const *argv[]) {
  pthread_t t[MAX_THREADS], t2[MAX_THREADS];
  long int thid, i, j, count;

  struct timeval init, final;
  int tmili;

  grid = (int**)malloc(N*sizeof(int*));
  newgrid = (int**)malloc(N*sizeof(int*));

  for(int i = 0; i<N; i++){
    grid[i] = (int*)malloc(N*sizeof(int));
    newgrid[i] = (int*)malloc(N*sizeof(int));
  }

  srand(SRAND_VALUE);
  count =0;
  for(i = 0; i<N; i++) { //laço sobre as células do tabuleiro sem contar com um eventual halo
    for(j = 0; j<N; j++) {
      grid[i][j] = rand() % 2;
      if(grid[i][j])count++;
    }
  }

  //pegando o tempo inicial
  gettimeofday(&init, NULL);
  void *status;
  for(i=0; i<GENERATIONS; i++){

    for(j=0; j<MAX_THREADS; j++){
      pthread_create(&t[j], NULL, gameOfLife, (void *)j);
    }

    for(j=0; j<MAX_THREADS; j++){
      pthread_join(t[j],&status);
    }
    for(j=0; j<MAX_THREADS; j++){
      pthread_create(&t2[j], NULL, refreshGrid, (void *)j);
    }
    for(j=0; j<MAX_THREADS; j++){
      pthread_join(t2[j],&status);
    }
  }

  //pegando o tempo final
  gettimeofday(&final, NULL);

  //calculando o tempo de execução utilizando gettimeofday
  tmili = (int) (1000*(final.tv_sec - init.tv_sec) + (final.tv_usec - init.tv_usec)/1000);

  printf("tempo decorrido em milissegundos: %d ms\n",tmili );
  printf("Resultado: \nNúmero de células vivas na primeira geração: %ld\nNúmero de células vivas na última geração: %ld\n", count, getResult());

  free(newgrid);
  free(grid);
  return 0;
}
