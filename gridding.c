#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <math.h>

#define MAX 256
#define VEL_LUZ 299792458
#define PI 3.14159265358979323846

double **matriz_fr;
double **matriz_fi;
double **matriz_wr;

// Inicializa matrices en 0
void inicializarMatrices(int tamanyo_imagen)
{
    for (int i = 0; i < tamanyo_imagen; i++)
    {
        for (int j = 0; j < tamanyo_imagen; j++)
        {
            matriz_fr[i][j] = 0.0;
            matriz_fi[i][j] = 0.0;
            matriz_wr[i][j] = 0.0;
        }
    }
}

void liberarMatrices(int tamanyo_imagen)
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

void matrizCompartida(char *nombre_archivo_entrada, int chunk_lectura, int numero_tareas, int tamanyo_imagen, double delta_u, double delta_v)
{
    // Abro archivo para lectura binaria
    FILE *archivo_entrada = fopen(nombre_archivo_entrada, "rb");

    bool flag = true;
    int cont_final = 0;
    
    if (archivo_entrada == NULL)
    {
        fprintf(stderr, "Error al abrir el archivo de entrada\n");
        exit(EXIT_FAILURE);
    }
    // Sección paralela
    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int valor_t = 0; valor_t < numero_tareas; valor_t++) 
            {
                #pragma omp task
                {
                    //MIentras archivo no este vacio
                    while (flag)
                    {
                        char *linea_aux;
                        char linea[MAX];
                        char matriz[chunk_lectura][MAX];
                        int contador = 0, i_k, j_k;
                        double u, v, w, visibilidad_real, visibilidad_im, peso_w, frec_obs, u_k, v_k, canal_espectral;

                        // EMPEZARA ACA LA SC???????????????????????????????????????????????????????????????
                        for (int i = 0; i < chunk_lectura; i++){
                            // Si no hay lineas por leer, flag = false
                            if (fgets(linea, sizeof(linea), archivo_entrada) == NULL)
                            {
                                flag = false;
                            }
                            else
                            {
                                #pragma omp critical
                                contador++;
                                strcpy(matriz[i], linea);
                                //printf("Mi id tarea es: %d, mi id hilo es: %d, contador: %d\n", valor_t, omp_get_thread_num(), contador);
                            }
                        }
            
                        for (int i = 0; i < contador; i ++)
                        {
                            sscanf(matriz[i], "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf", &u, &v, &w, &visibilidad_real, &visibilidad_im, &peso_w, &frec_obs, &canal_espectral);

                            // Transformación de las coordenada u, v a longitud de onda
                            u_k = u * (frec_obs / VEL_LUZ);
                            v_k = v * (frec_obs / VEL_LUZ);
                            //  Determina la posición de la matriz que correspondela visibilidad
                            i_k = round((u_k / delta_u) + tamanyo_imagen / 2);
                            j_k = round((v_k / delta_v) + tamanyo_imagen / 2);

                            // Acumula en matriz
                            #pragma omp critical
                            {
                                cont_final++;
                                matriz_fr[i_k][j_k] += peso_w * visibilidad_real;
                                matriz_fi[i_k][j_k] += peso_w * visibilidad_im;
                                matriz_wr[i_k][j_k] += peso_w;
                            }                               
                        }
                    }
                }
            }
        }
    }
    printf("Contador final: %d\n", cont_final);
    // Cierro archivo
    fclose(archivo_entrada);
}

void escribirArchivoSalida(char *nombre_datos_grideados, int tamanyo_imagen){
    char nombre_datos_grideados_2[MAX];
    strcpy(nombre_datos_grideados_2, nombre_datos_grideados);
    strcat(nombre_datos_grideados, "r.raw");
    strcat(nombre_datos_grideados_2, "i.raw");
    FILE *archivo_salida_r = fopen(nombre_datos_grideados, "wb");
    FILE *archivo_salida_i = fopen(nombre_datos_grideados_2, "wb");
    
    for (int i = 0; i < tamanyo_imagen; i++)
    {
        for (int j = 0; j < tamanyo_imagen; j++)
        {
            fwrite(&matriz_fr[i][j], sizeof(double), 1, archivo_salida_r);
            fwrite(&matriz_fi[i][j], sizeof(double), 1, archivo_salida_i);
        }
    }
}

void normalizarMatrices(int tamanyo_imagen){
    for (int i = 0; i < tamanyo_imagen; i++)
    {
        for (int j = 0; j < tamanyo_imagen; j++)
        {
            if (matriz_wr[i][j] == 0)
            {
                matriz_fr[i][j] = 0.0;
                matriz_fi[i][j] = 0.0;
            }

            else
            {
                matriz_fr[i][j] = matriz_fr[i][j] / matriz_wr[i][j];
                matriz_fi[i][j] = matriz_fi[i][j] / matriz_wr[i][j];
            }
        }
    }
}

int main(int argc, char *argv[])
{
    // Nombre del archivo de entrada y de salida
    char *nombre_archivo_entrada, *nombre_datos_grideados;
    double delta_x, delta_u, delta_v;
    int tamanyo_imagen, chunk_lectura, numero_tareas, opcion;

    while ((opcion = getopt(argc, argv, "i:o:d:N:c:t:")) != -1)
    {
        switch (opcion)
        {
        case 'i':
            nombre_archivo_entrada = optarg;
            break;
        case 'o':
            nombre_datos_grideados = optarg;
            break;
        case 'd':
            delta_x = atof(optarg);
            break;
        case 'N':
            tamanyo_imagen = atoi(optarg);
            break;
        case 'c':
            chunk_lectura = atoi(optarg);
            break;
        case 't':
            numero_tareas = atoi(optarg);
            break;
        default:
            fprintf(stderr, "Error en la entrada de parametros\n");
            exit(EXIT_FAILURE);
        }
    }
    
    // Cálculo de delta_x.
    delta_x = (PI * delta_x) / (3600 * 180);

    // Imagen I(x, y) es delta_x y delta_y, luego la distancia en los puntos de su transformada V(u, v) es:
    delta_u = 1 / (tamanyo_imagen * delta_x);
    delta_v = 1 / (tamanyo_imagen * delta_x);
    
    matriz_fr = (double **)malloc(tamanyo_imagen * sizeof(double *));
    matriz_fi = (double **)malloc(tamanyo_imagen * sizeof(double *));
    matriz_wr = (double **)malloc(tamanyo_imagen * sizeof(double *));

    for (int i = 0; i < tamanyo_imagen; i++)
    {
        matriz_fr[i] = (double *)malloc(tamanyo_imagen * sizeof(double));
        matriz_fi[i] = (double *)malloc(tamanyo_imagen * sizeof(double));
        matriz_wr[i] = (double *)malloc(tamanyo_imagen * sizeof(double));
    }
    
    inicializarMatrices(tamanyo_imagen);
    
    //nombre arch entrada
    printf("Nombre archivo entrada: %s\n", nombre_archivo_entrada);
    matrizCompartida(nombre_archivo_entrada, chunk_lectura, numero_tareas, tamanyo_imagen, delta_u, delta_v);

    // Normalizar matrices
    normalizarMatrices(tamanyo_imagen);
    // Escribir en archivo de salida
    escribirArchivoSalida(nombre_datos_grideados, tamanyo_imagen);

    //Libero memoria
    liberarMatrices(tamanyo_imagen);
    printf("Fin del programa\n");

    //matrizLocal(nombre_archivo_entrada, chunk_lectura, numero_tareas, tamanyo_imagen, delta_u, delta_v);
    return 0;
}
