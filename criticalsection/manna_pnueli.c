#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define THREADS 4
#define N 1000000000

long count = 0, sum = 0, respond = 0, request =0;
long iterations = N/THREADS;
void *client_process(void* id){
    long thread_id = (long) id;
    long local = 0;
    long i;

    for(i=0; i<iterations; i++){
        while(respond!=thread_id){ //wait server response
            request = thread_id;
        }
        //crital section start
        local = sum;
        sleep(rand()%2);
        sum = local + 1;
        printf("Thread %ld accessed %ld times\n", thread_id, sum);
        fflush(stdout);
        respond = 0;
    }
    pthread_exit(NULL);
}

void *server_process(void* i){
    while(1){
        while(request==0); //wait for any client request

        respond = request;

        while(respond!=0);//wait client finish critical section

        request = 0;
    }
    pthread_exit(NULL);
}

int main(void)
{
    pthread_t threads[THREADS];
    pthread_t server;
    int thid;
    int i;

    pthread_create(&server, NULL, server_process, NULL);

    for(i = 0; i<THREADS; i++){
        pthread_create(&threads[i], NULL, client_process, (void*)(i+1));
    }
    for(i = 0; i<THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    printf("sum = %ld\n", sum);
    return 0;
}
