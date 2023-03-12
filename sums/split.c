#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <pthread.h>

// #define INITIALIZE_ARRAY

struct thread_data
{
    long id;
    pthread_t sys_id;
    unsigned char *start;
    long size;
    unsigned long sum;
};

unsigned long linear_sum(unsigned char* start, long size)
{
    unsigned long sum = 0;
    unsigned char* p = start;
    for (long i = 0; i < size; i++) {
        sum += (*p);
        p += 1;
    } 
    return sum;
}

void* worker(void *arg)
{
    struct thread_data* data = arg;
    data->sum = linear_sum(data->start, data->size); 
    printf("Thread %02ld: %ld\n", data->id, data->sum);
    return NULL;
}

int main(int argc, char** argv)
{
    if (argc != 3) {
        errx(EXIT_FAILURE, "Usage: array_size thread_number");
    }
    
    long array_size = atol(argv[1]);
    if (array_size <= 63) {
        errx(EXIT_FAILURE, "The size of the array is not valid (must be greater than 63).");
    }

    long thread_number = atol(argv[2]);
    if (thread_number < 1 || thread_number > 16) {
        errx(EXIT_FAILURE, "The number of therads is not valid (between 1 and 16).");
    }

    struct thread_data data[thread_number];
    unsigned char *bytes = malloc(array_size);
    if (bytes == NULL) {
        errx(EXIT_FAILURE, "Not enough memory!\nTry to reduce the size of the array.");
    }

#ifdef INITIALIZE_ARRAY
    printf("Initializing array........");
    fflush(stdout);
    for (long i = 0; i < array_size; i++) {
        bytes[i] = 1;
    }
    printf("OK\n");
#endif
    long n_left = array_size;
    long n_allocate_per_thread = array_size / thread_number;
    unsigned char* p_start = bytes;
    for (long i = 0; i < thread_number; i++) { 
        data[i].id = i;
        data[i].start = p_start;
        data[i].sum = 0;
        data[i].size = i == (thread_number - 1)? n_left : n_allocate_per_thread;
        int err = pthread_create(&(data[i].sys_id), NULL, worker, &data[i]);
        if (err != 0) {
            errx(err, "pthread_create()");
        }
        n_left -= n_allocate_per_thread;
        p_start += n_allocate_per_thread;
    }
    
    unsigned long sum = 0;
    for (long i = 0; i < thread_number; i++) {
        int err = pthread_join(data[i].sys_id, NULL);
        if (err != 0) {
            errx(err, "pthread_join()");
        }
        sum += data[i].sum;
    }
    printf("Sum......: %ld\n", sum);
    
    free(bytes);
    return 0;
}

