#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* fn_thread(void* arg);

int main(int argc, char** argv)
{
    if (argc < 2) {
        errx(EXIT_FAILURE, "The number of threads is missing.");
    }
    else if (argc > 2) {
        errx(EXIT_FAILURE, "Specify only the number of threads.");
    }

    long n_thread = atol(argv[1]);
    if (n_thread <= 0) {
        errx(-1, "The number of threads is not valid.");
    }

    pthread_t* thread = malloc(n_thread * sizeof(pthread_t));
    if (thread == NULL) {
        errx(EXIT_FAILURE, "malloc()");
    }
    for (long i = 0; i < n_thread; i++) {
        long* thread_id = malloc(sizeof(long)); 
        if (thread_id == NULL) {
            errx(EXIT_FAILURE, "malloc()");
        }
        *thread_id = i;
        int err = pthread_create(&(thread[i]), NULL, fn_thread, thread_id);
        if (err != 0) {
            free(thread_id);
            free(thread);
            errx(err, "pthread_create()");
        }
    }
    
    for (long i = 0; i < n_thread; i++) {
        pthread_join(thread[i], NULL);
    }
    free(thread);
    
    return 0;
}

void* fn_thread(void* arg)
{
    sleep(rand() % 3);
    long thread_id = *((long*) arg); free(arg);
    printf("Hello from thread %ld!\n", thread_id);
    return NULL;
}
