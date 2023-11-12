#include "tareas.h"

// Inicializa matrices en 0
void inicializarMatrices(double **matriz_fr, double **matriz_fi, double **matriz_wr, int tamanyo_imagen)
{
    for (int i = 0; i < tamanyo_imagen; i++)
    {
        for (int j = 0; j < tamanyo_imagen; j++)
        {
            matriz_fr[i][j] = 0;
            matriz_fi[i][j] = 0;
            matriz_wr[i][j] = 0;
        }
    }
}

void liberarMatrices(double **matriz_fr, double **matriz_fi, double **matriz_wr, int tamanyo_imagen)
{
    for (int i = 0; i < tamanyo_imagen; i++)
    {
        free(matriz_fr[i]);
        free(matriz_fi[i]);
        free(matriz_wr[i]);
    }

    free(matriz_fr);
    free(matriz_fi);
    free(matriz_wr);
}

void leerArchivo(char *nombre_archivo_entrada, int chunk_lectura, int numero_tareas, int tamanyo_imagen)
{
    double **matriz_fr = (double **)malloc(tamanyo_imagen * sizeof(double *));
    double **matriz_fi = (double **)malloc(tamanyo_imagen * sizeof(double *));
    double **matriz_wr = (double **)malloc(tamanyo_imagen * sizeof(double *));

    inicializarMatrices(matriz_fr, matriz_fi, matriz_wr, tamanyo_imagen);

    // Abro archivo para lectura binaria
    FILE *archivo_entrada = fopen(nombre_archivo_entrada, "rb");

    bool flag = true;

    if (archivo_entrada == NULL)
    {
        fprintf(stderr, "Error al abrir el archivo de entrada\n");
        exit(EXIT_FAILURE);
    }
    // Sección paralela
    #pragma omp parallel num_threads(numero_tareas)
    {
        // id tarea <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< BOORRAR NO ME SIRVE DE MOMENTO
        //int id_tarea = omp_get_thread_num();
        
        #pragma omp single
        {
            // Hebra maestra crea tareas
            for (int i = 1; i <= numero_tareas; ++i) 
            {
                // Ejecución de la tarea
                #pragma omp task
                {
                    // Mientras haya lineas por leer, flag = true
                    while (flag)
                    {
                        char linea[chunk_lectura];
                        char vector[chunk_lectura];
                        // EMPEZARA ACA LA SC???????????????????????????????????????????????????????????????
                        for (int i = 0; i < chunk_lectura; i++)
                        {
                            #pragma omp critical{
                                //probar con fgets
                                int leido = fread(linea, sizeof(linea), 1, archivo_entrada);
                                // si no se pudo leer, termino
                                if (leido != 1)
                                {
                                    flag = false;
                                }
                                
                                else{
                                    // guardo la linea en vector
                                    vector[i] = linea[0];  
                                }
                            }
                        } 
                
                    }
                }
            }
        }
    }

    // Cierro archivo
    fclose(archivo_entrada);
    //Libero memoria
    liberarMatrices(matriz_fr, matriz_fi, matriz_wr, tamanyo_imagen);
}