#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // Afin d'économiser de l'éspace mémoire on utilisera une implémentation avec une bitarray (64bits par élément)
    // On pourra marquer les nombres premiers en utilisant des opérations bitwise
    // Exemple N = 10 on a les nombres 0 1 2 3 4 5 6 7 8 9
    // 0, 1, 4, 6, 8 et 9 seront marqués non premiers en mettant le bit à la position i à 0
    // 9  8  7  6  5  4  3  2  1  0
    // 0  0  1  0  1  0  1  1  0  0
    // (11..110010101100)base2 = 18446744073709549743 qui sera stocké dans bitarray

    if (argc < 2) {
        printf("Inputs manquants !\n");
        return 1;
    }

    int N = atoi(argv[1]);
    struct timespec start, end;
    double executionTime;

    clock_gettime(CLOCK_MONOTONIC, &start);

    // Taille de la bitarray
    size_t NW = N / 64 + 1;
    uint64_t *array = calloc(sizeof(*array), NW);

    // On marque tout les n comme étant composé
    for (size_t i = 0; i < NW; i++) {
      array[i] = 0xffffffffffffffff;
    }

    // On marque 1 et 2 comme étant non premiers
    array[0] &= ~(1 << 1);
    array[0] &= ~(1 << 0);

    // Eratosthene
    for (uint64_t i = 2; i * i <= N; i++) {
        // Si le bit à la pos i = 1 : i est premier
        if (array[i / 64] & ((uint_fast64_t)1 << i)) {
            for (uint64_t j = i * i; j <= N; j += i) {
                // On mets le bit à la position j à 0 : j est composé
                array[j / 64] &= ~(((uint_fast64_t)1) << (j % 64));
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    executionTime = (end.tv_sec - start.tv_sec) +
                    (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("%.6f\n", executionTime);

    // Affichage et vérif
    int total_primes = 0; 

    for (uint64_t i = 0; i <= N; i++) {
      if (array[i / 64] & ((uint64_t)1 << i)) {
        //printf("%lu ", i);
        total_primes++;
      }
    }
    //printf("\n");
    //printf("\nTotal %d \n", total_primes);
    return 0;
}
