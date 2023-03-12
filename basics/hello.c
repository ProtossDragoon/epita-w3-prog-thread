#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <pthread.h>

void* fn_thread(void* arg __attribute__((unused)));

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

    pthread_t thread[n_thread];
    for (long i = 0; i < n_thread; i++) {
        int err = pthread_create(&(thread[i]), NULL, fn_thread, NULL);
        if (err != 0) {
            errx(err, "pthread_create()");
        }
    }
    
    for (long i = 0; i < n_thread; i++) {
        pthread_join(thread[i], NULL);
    }
    
    return 0;
}

void* fn_thread(void* arg __attribute__((unused)))
{
    printf("Hello from thread!\n");
    return NULL;
}
