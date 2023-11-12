#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 24  // Longitud del arreglo (N = 2^(2k))
#define H 4   // Número de hilos
#define T 2   // Número de tareas

int main() {
    int arr[N];
    int max_value = 0;

    // Se llena el arreglo con valores aleatorios 
    for (int i = 0; i < N; i++) {
        arr[i] = rand() % 1000;
    }

    // Imprimir el arreglo
    printf("Arreglo de entrada:\n");
    for (int i = 0; i < N; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    #pragma omp parallel num_threads(H) // Inicio de la región paralela
    {
        #pragma omp single // Se ejecuta una sola vez para crear las tasks
        {
            #pragma omp taskloop num_tasks(T) // Creación de las tasks
            for (int i = 0; i < N; i++) {
                #pragma omp critical // Sección crítica para actualizar el valor máximo
                {
                    if (arr[i] > max_value) {
                        max_value = arr[i];
                    }
                }
            }
        }
    }
    // Imprimir el valor máximo encontrado
    printf("Valor máximo: %d\n", max_value);
    return 0;
}