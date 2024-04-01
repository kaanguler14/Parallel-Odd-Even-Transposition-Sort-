#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define ARRAY_SIZE 10000
#define NUM_THREADS 2

int arr[ARRAY_SIZE];
pthread_barrier_t barrier;

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void* oddEvenSortThread(void* arg) {
    int thread_part = *(int*)arg;
    for (int phase = 0; phase < ARRAY_SIZE; phase++) {
        if (phase % 2 == 0) {
            for (int i = thread_part * 2; i < ARRAY_SIZE - 1; i += NUM_THREADS * 2) {
                if (arr[i] > arr[i + 1]) {
                    swap(&arr[i], &arr[i + 1]);
                }
            }
        }
        else {
            for (int i = thread_part * 2 + 1; i < ARRAY_SIZE - 1; i += NUM_THREADS * 2) {
                if (i + 1 < ARRAY_SIZE && arr[i] > arr[i + 1]) {
                    swap(&arr[i], &arr[i + 1]);
                }
            }
        }
        pthread_barrier_wait(&barrier); // Wait for all threads
    }
    return NULL;
}

// Print Array
void printArr() {
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        printf("%d ", arr[i]);
        if ((i + 1) % 10 == 0) printf("\n");
    }
}

int main() {
    srand(time(NULL));
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        arr[i] = rand() % 10000;
    }

    int thread_ids[NUM_THREADS];
    pthread_t threads[NUM_THREADS];
    // Barrier initialization with the number of threads plus one for the main thread
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, oddEvenSortThread, &thread_ids[i]);
    }

    // Join threads instead of waiting at every phase
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }
    //printArr();
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_taken = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Sorting took %f seconds.\n", time_taken);

    pthread_barrier_destroy(&barrier);
    return 0;
}
