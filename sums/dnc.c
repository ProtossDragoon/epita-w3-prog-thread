#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <pthread.h>
#include <stdatomic.h>

// #define INITIALIZE_ARRAY

void* worker(void *arg);

struct thread_data
{
    pthread_t sys_id;
    unsigned char *start;
    long size;
    long threshold;
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

unsigned long dnc_sum(unsigned char* start, long size, long threshold)
{
    if (size <= threshold) {
       return linear_sum(start, size);
    } 
    long size1 = size / 2;
    long size2 = size - size1;
    unsigned char* mid = start + size1;

    struct thread_data data;
    int err;
    data.start = start;
    data.size = size1;
    data.threshold = threshold;
    data.sum = 0;
    err = pthread_create(&(data.sys_id), NULL, worker, &data);
    if (err != 0) {
        errx(err, "pthread_create()");
    }
    unsigned long s2 = dnc_sum(mid, size2, threshold);
    err = pthread_join(data.sys_id, NULL);
    if (err != 0) {
        errx(err, "pthread_join()");
    }
    unsigned long s1 = data.sum;
    return s1 + s2;
}

// Just used to count and display the number of threads.
atomic_int thread_count = 1;

void* worker(void *arg)
{
    atomic_fetch_add(&thread_count, 1);

    struct thread_data* data = arg;
    data->sum = dnc_sum(data->start, data->size, data->threshold); 
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

    unsigned char *bytes = malloc(array_size);
    if (bytes == NULL) {
        errx(EXIT_FAILURE, "Not enough memory!\nTry to reduce the size of the array.");
    }

#ifdef INITIALIZE_ARRAY
    printf("Initializing array.. ");
    fflush(stdout);
    for (long i = 0; i < array_size; i++) {
        bytes[i] = 1;
    }
    printf("OK\n");
#endif
    printf("Sum................. %lu\n", dnc_sum(bytes, array_size, 1 + array_size / thread_number)); 
    printf("Number of threads... %i\n", atomic_load(&thread_count));

    free(bytes);
    return 0;
}

