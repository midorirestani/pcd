#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

long count = 0, sum = 0, respond = 0, request =0;

void *client_process(void* i){
    long thread_id = (long) i;
    long local = 0;
    while(respond!=thread_id){
        request = thread_id;
    }
    local = sum;
    sleep(rand()%2);
    sum = local + 1;

}

void *server_process(void* i){
    while(request==0);
    
    respond = request;
    
    while(respond!=0);

    request = 0;
}

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}
