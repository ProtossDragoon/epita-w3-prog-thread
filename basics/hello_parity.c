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

    pthread_t thread[n_thread];
    char* string_even = "Hello from an even thread.";
    char* string_odd = "Hello from an odd thread.";
    char* string;
    for (long i = 0; i < n_thread; i++) {
        if (i % 2 == 0) {
            string = string_even;
        } else {
            string = string_odd;
        }
        int err = pthread_create(&(thread[i]), NULL, fn_thread, string);
        if (err != 0) {
            errx(err, "pthread_create()");
        }
    }
    
    for (long i = 0; i < n_thread; i++) {
        pthread_join(thread[i], NULL);
    }
    
    return 0;
}

void* fn_thread(void* arg)
{
    sleep(rand() % 3);
    printf("%s\n", (char*) arg);
    return NULL;
}
