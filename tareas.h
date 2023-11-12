#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>

void leerArchivo(char *nombre_archivo_entrada, int chunk_lectura, int numero_tareas, int tamanyo_imagen);
void inicializarMatrices(double **matriz_fr, double **matriz_fi, double **matriz_wr, int tamanyo_imagen);
void liberarMatrices(double **matriz_fr, double **matriz_fi, double **matriz_wr, int tamanyo_imagen);

