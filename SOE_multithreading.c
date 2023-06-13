#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

struct thread_args {
    uint64_t *array; // bitarray pour marquer les n premiers
    uint64_t max_i, begin, end; // Utiles pour calculer la plage de chaque thread
};

void *thread_function(void *th_arg) {
    struct thread_args *args = th_arg;
    uint64_t *array = args->array;

    for (uint64_t i = 2; i <= args->max_i; i += 1) {
        if ((array[i / 64] & ((uint64_t)1 << (i % 64)))){
            // Calcul de la plage pour chauque thread [start - stop]
            uint64_t start = (args->begin + i - 1) / i * i;
            if (start < i * i)
                start = i * i;
            uint64_t stop = args->end;
            for (uint64_t j = start; j < stop; j += i ) {
                array[j / 64] &= ~((uint64_t)1 << (j % 64));
            }
        }
    }
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) {

    if (argc < 3) {
        printf("Inputs manquants !.\n");
        return 1;
    }

    uint64_t N = atoi(argv[1]);
    int K = atoi(argv[2]);
    struct timespec start, end;
    double executionTime;

    /* Début exe */
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Taille de la bitarray
    size_t NW = N / 64 + 1;
    uint64_t *array = calloc(sizeof(*array), NW);

    // Liste de k pthread
    pthread_t k_thread[K];

    // Liste des arguments pour chaque thread
    struct thread_args args[K];


    // On marque tout les n comme étant composé
    for (size_t i = 0; i < NW; i++) {
        array[i] = 0xFFFFFFFFFFFFFFFF;
    }

    // On marque 0 et 1 comme étant non premiers
    array[0] &= ~(1 << 1);
    array[0] &= ~(1 << 0);

    // Création de k threads et initialisation des args pour chaque thread
    for (int k = 0; k < K; k++) {
        args[k].array = array;
        args[k].max_i = (uint64_t)+sqrt(N);
        args[k].begin = 64 * ((uint64_t)NW * k / K);
        args[k].end = 64 * ((uint64_t)NW * (k + 1) / K);
        pthread_create(&k_thread[k], NULL, thread_function, &args[k]);
    }

    // Synchronisation
    for (int k = 0; k < K; k++) {
        pthread_join(k_thread[k], NULL);
    }
    
    /* Fin exe */
    clock_gettime(CLOCK_MONOTONIC, &end);
    executionTime = (end.tv_sec - start.tv_sec) +
                    (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("%.6f\n", executionTime);
   

    uint64_t total_primes = 0; // Utile pour vérification
    
    for (uint64_t i = 0; i <= N; i++) {
        if (array[i / 64] & ((uint64_t)1 << (i % 64))) {
            //printf("%llu ", i);
            total_primes++;
        }
    }
    //printf("\n");
    //printf("N = %lu, %d thread(s), Total primes %d \n\n",N,K, total_primes);
    free(array);
    return 0;
}