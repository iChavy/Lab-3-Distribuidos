#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define MAX 256
#define VEL_LUZ 299792458
#define PI 3.14159265358979323846

double **matriz_fr;
double **matriz_fi;
double **matriz_wr;

/*
Descripción: Asigna memoria a las matrices globales matriz_fr, matriz_fi y matriz_wr.
Entrada: tamanyo_imagen: int que posee el valor del tamaño de la imagen.
Salida: No posee retorno.
*/
void asignarMemoria(int tamanyo_imagen){
    matriz_fr = (double **)malloc(tamanyo_imagen * sizeof(double *));
    matriz_fi = (double **)malloc(tamanyo_imagen * sizeof(double *));
    matriz_wr = (double **)malloc(tamanyo_imagen * sizeof(double *));

    for (int i = 0; i < tamanyo_imagen; i++)
    {
        matriz_fr[i] = (double *)malloc(tamanyo_imagen * sizeof(double));
        matriz_fi[i] = (double *)malloc(tamanyo_imagen * sizeof(double));
        matriz_wr[i] = (double *)malloc(tamanyo_imagen * sizeof(double));
    }
}

/*
Descripción: Asigna el valor 0.0 a las matrices matriz_fr, matriz_fi y matriz_wr.
Entrada: tamanyo_imagen: int que posee el valor del tamaño de la imagen.
Salida: No posee retorno
*/
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

/*
Descripción: Asigna el valor 0.0 a las matrices matriz_fr_local, matriz_fi_local y matriz_wr_local.
Entrada: matriz_fr: (double**) matriz_fr.
         matriz_fi: (double**) matriz_fi.
         matriz_wr: (double**) matriz_wr.
         tamanyo_imagen: int que posee el valor del tamaño de la imagen.
*/
void inicializarMatricesLocales(double **matriz_fr, double **matriz_fi, double **matriz_wr, int tamanyo_imagen)
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

/*
Descripción: Normaliza las matrices globales matriz_fr y matriz_fi.
Entrada: tamanyo_imagen: int que posee el valor del tamaño de la imagen.
Salida: No posee retorno.
*/
void normalizarMatrices(int tamanyo_imagen)
{
    for (int i = 0; i < tamanyo_imagen; i++)
    {
        for (int j = 0; j < tamanyo_imagen; j++)
        {
            // Si el valor de la matriz matriz_wr es 0, entonces el valor de la matriz matriz_fr y matriz_fi será 0.0
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

/*
Descripción: Escribe en un archivo de salida los valores de las matrices matriz_fr y matriz_fi.
Entrada: nombre_datos_grideados: char*. Puntero que apunta al primer caracter del nombre del archivo de salida.
         tamanyo_imagen: int que posee el valor del tamaño de la imagen.
Salida: No posee retorno.
*/
void escribirArchivoSalidaGlobal(char *nombre_datos_grideados, int tamanyo_imagen)
{
    char nombre_datos_grideados_r[MAX];
    char nombre_datos_grideados_i[MAX];
    strcpy(nombre_datos_grideados_r, nombre_datos_grideados);
    strcpy(nombre_datos_grideados_i, nombre_datos_grideados);
    // Concatena el nombre del archivo de salida con r.raw y i.raw
    strcat(nombre_datos_grideados_r, "r.raw");
    strcat(nombre_datos_grideados_i, "i.raw");
    FILE *archivo_salida_r = fopen(nombre_datos_grideados_r, "wb");
    FILE *archivo_salida_i = fopen(nombre_datos_grideados_i, "wb");
    
    // Escribe en los archivos de salida los valores de las matrices globales fr y fi
    for (int i = 0; i < tamanyo_imagen; i++)
    {
        for (int j = 0; j < tamanyo_imagen; j++)
        {
            fwrite(&matriz_fr[i][j], sizeof(double), 1, archivo_salida_r);
            fwrite(&matriz_fi[i][j], sizeof(double), 1, archivo_salida_i);
        }
    }
}

/*
Descripción: Escribe en un archivo de salida los valores obtenidos al acumular las matrices locales.
Entrada: nombre_datos_grideados: char*. Puntero que apunta al primer caracter del nombre del archivo de salida.
         tamanyo_imagen: int que posee el valor del tamaño de la imagen.
Salida: No posee retorno.
*/
void escribirArchivoSalidaLocal(char *nombre_datos_grideados, int tamanyo_imagen)
{
    char nombre_datos_grideados_i[MAX];
    strcpy(nombre_datos_grideados_i, nombre_datos_grideados);
    // Concatena el nombre del archivo de salida con r_local.raw y i_local.raw
    strcat(nombre_datos_grideados, "r_local.raw");
    strcat(nombre_datos_grideados_i, "i_local.raw");
    FILE *archivo_salida_r = fopen(nombre_datos_grideados, "wb");
    FILE *archivo_salida_i = fopen(nombre_datos_grideados_i, "wb");
    
    for (int i = 0; i < tamanyo_imagen; i++)
    {
        for (int j = 0; j < tamanyo_imagen; j++)
        {
            fwrite(&matriz_fr[i][j], sizeof(double), 1, archivo_salida_r);
            fwrite(&matriz_fi[i][j], sizeof(double), 1, archivo_salida_i);
        }
    }
}

/*
Descripción: Libera la memoria de las matrices definidas globalmente.
Entrada: tamanyo_imagen: int que posee el valor del tamaño de la imagen.
Salida: No posee retorno
*/
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

/*
Descripción: Abre el archivo de entrada, crea n tareas y mientras no se haya leído todo el archivo, cada tarea lee un chunk de líneas del archivo de entrada, luego se calcula la posición de la matriz que corresponde a la visibilidad y se acumula los resultados en las matrices globales.
Entrada: nombre_archivo_entrada: char*. Puntero que apunta al primer caracter del nombre del archivo de entrada.
         chunk_lectura: int que posee el valor del tamaño del chunk de líneas que se leerá del archivo de entrada.
         numero_tareas: int que posee el valor del número de tareas que se crearán.
         tamanyo_imagen: int que posee el valor del tamaño de la imagen.
         delta_u: double que posee el valor de la distancia entre los puntos de la transformada de Fourier V(u, v).
         delta_v: double que posee el valor de la distancia entre los puntos de la transformada de Fourier V(u, v).
Salida: No posee retorno.
*/
void matrizCompartida(char *nombre_archivo_entrada, int chunk_lectura, int numero_tareas, int tamanyo_imagen, double delta_u, double delta_v) //<<<<< revisar comentarios dentro de la funcion
{
    // Abro archivo para lectura binaria
    FILE *archivo_entrada = fopen(nombre_archivo_entrada, "rb");

    bool flag = true;
    int cont_final = 0; // BORRAR <<<<<<<<<<<<<<<<
    
    // Verifica que el archivo se abrió correctamente
    if (archivo_entrada == NULL)
    {
        fprintf(stderr, "No se ha podido abrir el archivo de entrada\n");
        exit(EXIT_FAILURE);//// <<<<<<<<<<<<<<<<< buscar manejo de errores
    }

    // Sección paralela
    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int i = 0; i < numero_tareas; i++) 
            {
                #pragma omp task
                {
                    // Mientras el archivo no esté vacío, flag = true
                    while (flag)
                    {
                        char *linea_aux;
                        char linea[MAX];
                        char matriz[chunk_lectura][MAX];
                        int contador = 0, i_k, j_k;
                        double u, v, w, visibilidad_real, visibilidad_im, peso_w, frec_obs, u_k, v_k, canal_espectral;

                        // Lee n chunk de líneas del archivo
                        for (int i = 0; i < chunk_lectura; i++){
                            // Si no hay líneas por leer, flag = false
                            if (fgets(linea, sizeof(linea), archivo_entrada) == NULL)
                            {
                                flag = false;
                            }
                            else
                            {
                                contador++;
                                // Almacena la línea leída en una matriz
                                strcpy(matriz[i], linea);
                            }
                        }

                        // Recorre la matriz que posee las líneas leídas, extrae los valores, realiza cálculos y acumula en las matrices globales
                        for (int i = 0; i < contador; i ++)
                        {
                            sscanf(matriz[i], "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf", &u, &v, &w, &visibilidad_real, &visibilidad_im, &peso_w, &frec_obs, &canal_espectral);

                            // Transformación de las coordenada u, v a longitud de onda
                            u_k = u * (frec_obs / VEL_LUZ);
                            v_k = v * (frec_obs / VEL_LUZ);

                            //  Determina la posición de la matriz que correspondela visibilidad
                            i_k = round((u_k / delta_u) + tamanyo_imagen / 2);
                            j_k = round((v_k / delta_v) + tamanyo_imagen / 2);

                            // Acumula en las matrices globales 
                            #pragma omp critical
                            {
                                cont_final++;// BORRAR <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
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
    printf("Contador final: %d\n", cont_final); // BORRAR <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    // Cierro archivo
    fclose(archivo_entrada);
}

/*
Descripción: Abre el archivo de entrada, crea n tareas y mientras no se haya leído todo el archivo, cada tarea lee un chunk de líneas del archivo de entrada, luego se calcula la posición de la matriz que corresponde a la visibilidad, acumula los resultados en la matriz local de cada tarea. Al finalizar, cada tarea acumula los resultados de su matriz local en las matrices globales.
*/
void matrizLocal(char *nombre_archivo_entrada, int chunk_lectura, int numero_tareas, int tamanyo_imagen, double delta_u, double delta_v)// REVISAR COMENTARIOS<<<<<<<<<<<
{
    // Abro archivo para lectura binaria
    FILE *archivo_entrada = fopen(nombre_archivo_entrada, "rb");

    bool flag = true;
    int cont_final = 0; // BORRAR <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    
    if (archivo_entrada == NULL)
    {
        fprintf(stderr, "No se ha podido abrir el archivo de entrada\n");
        exit(EXIT_FAILURE); // BUSCAR MANEJO DE ERRORES <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    }

    // Sección paralela
    #pragma omp parallel //private(matriz_fr_local, matriz_fi_local, matriz_wr_local)
    {
        #pragma omp single
        {
            for (int i = 0; i < numero_tareas; ++i) 
            {                
                #pragma omp task
                {
                    // mover al comienzo y probar <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
                    double **matriz_fr_local = (double **)malloc(tamanyo_imagen * sizeof(double *));
                    double **matriz_fi_local = (double **)malloc(tamanyo_imagen * sizeof(double *));
                    double **matriz_wr_local = (double **)malloc(tamanyo_imagen * sizeof(double *));

                    for (int i = 0; i < tamanyo_imagen; i++)
                    {
                        matriz_fr_local[i] = (double *)malloc(tamanyo_imagen * sizeof(double));
                        matriz_fi_local[i] = (double *)malloc(tamanyo_imagen * sizeof(double));
                        matriz_wr_local[i] = (double *)malloc(tamanyo_imagen * sizeof(double));
                    }
                    
                    inicializarMatricesLocales(matriz_fr_local, matriz_fi_local, matriz_wr_local, tamanyo_imagen);

                    // Mientras el archivo no esté vacío, flag = true
                    while (flag)
                    {
                        char *linea_aux;
                        char linea[MAX];
                        char matriz[chunk_lectura][MAX];
                        int contador = 0, i_k, j_k;
                        double u, v, w, visibilidad_real, visibilidad_im, peso_w, frec_obs, u_k, v_k, canal_espectral;                        
                        
                        for (int i = 0; i < chunk_lectura; i++)
                        {
                            // Si no hay líneas por leer, flag = false
                            if (fgets(linea, sizeof(linea), archivo_entrada) == NULL)
                            {
                                flag = false;
                            }
                            else
                            {
                                contador++;
                                // Almacena la línea leída en una matriz
                                strcpy(matriz[i], linea);                            
                                cont_final++;   // BORRAR <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
                            }
                                               
                        }
                        // Recorre la matriz que posee las líneas leídas, extrae los valores, realiza cálculos y acumula en las matrices locales que posee la tarea
                        for (int i = 0; i < contador; i ++)
                        {
                            sscanf(matriz[i], "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf", &u, &v, &w, &visibilidad_real, &visibilidad_im, &peso_w, &frec_obs, &canal_espectral);

                            // Transformación de las coordenada u, v a longitud de onda
                            u_k = u * (frec_obs / VEL_LUZ);
                            v_k = v * (frec_obs / VEL_LUZ);
                            //  Determina la posición de la matriz que correspondela visibilidad
                            i_k = round((u_k / delta_u) + tamanyo_imagen / 2);
                            j_k = round((v_k / delta_v) + tamanyo_imagen / 2);

                            // Acumula en matrices locales de la tarea
                            matriz_fr_local[i_k][j_k] += peso_w * visibilidad_real;
                            matriz_fi_local[i_k][j_k] += peso_w * visibilidad_im;
                            matriz_wr_local[i_k][j_k] += peso_w;
                        }
                    }
                    #pragma opm critical
                    // Acumula las matrices locales en las matrices globales
                    for (int i = 0; i < tamanyo_imagen; i++)
                    {
                        for (int j = 0; j < tamanyo_imagen; j++)
                        {
                            //matriz_fr_local_global[i][j] += matriz_fr_local[i][j];
                            //matriz_fi_local_global[i][j] += matriz_fi_local[i][j];
                            //matriz_wr_local_global[i][j] += matriz_wr_local[i][j];  
                            matriz_fr[i][j] += matriz_fr_local[i][j];
                            matriz_fi[i][j] += matriz_fi_local[i][j];
                            matriz_wr[i][j] += matriz_wr_local[i][j];                      
                        }
                    }
                } 
            }
        }
    }

    /*for (int i = 0; i < tamanyo_imagen; i++)
                {
                    free(matriz_fr_local[i]);
                    free(matriz_fi_local[i]);
                    free(matriz_wr_local[i]);
                }
                free(matriz_fr_local);
                free(matriz_fi_local);
                free(matriz_wr_local);*/
    printf("Contador final: %d\n", cont_final);// BORRAR <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    // Cierro archivo
    fclose(archivo_entrada);
}

/*
Decripción: Realiza el gridding de una imagen I(x, y) a su transformada V(u, v) con matriz compartida y con matriz local mediante task, luego los resultados son escritos en archivos.
Entrada: i: char*. Puntero que apunta al primer caracter del nombre del archivo de entrada.
         o: char*. Puntero que apunta al primer caracter del nombre del archivo de salida.
         d: double que posee el valor de delta_x.
         N: int que posee el valor del tamaño de la imagen.
         c: int que posee el valor del tamaño del chunk de líneas que se leerá del archivo de entrada.
         t: int que posee el valor del número de tareas que se crearán.
Salida: Retorna 0 si el programa finaliza correctamente.
*/
int main(int argc, char *argv[])
{
    // Nombre del archivo de entrada y de salida
    char *nombre_archivo_entrada, *nombre_datos_grideados;
    double delta_x, delta_u, delta_v;
    int tamanyo_imagen, chunk_lectura, numero_tareas, opcion;

    clock_t inicio, fin;
    double tiempo;

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
    
    asignarMemoria(tamanyo_imagen);
    inicializarMatrices(tamanyo_imagen);

    inicio = clock();
    matrizCompartida(nombre_archivo_entrada, chunk_lectura, numero_tareas, tamanyo_imagen, delta_u, delta_v);
    // Normalizar matrices
    normalizarMatrices(tamanyo_imagen);
    fin = clock();
    tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecución con matrices compartidas: %f\n", tiempo);

    // Escribir en archivo de salida
    escribirArchivoSalidaGlobal(nombre_datos_grideados, tamanyo_imagen);

    //////////////////////////////////// Programa con matrices locales ///////////////////////////////////////////
    inicializarMatrices(tamanyo_imagen);

    inicio = clock();
    matrizLocal(nombre_archivo_entrada, chunk_lectura, numero_tareas, tamanyo_imagen, delta_u, delta_v);
    normalizarMatrices(tamanyo_imagen);
    fin = clock();
    tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecución con matrices locales: %f\n", tiempo);

    // Escribir en archivo de salida
    escribirArchivoSalidaLocal(nombre_datos_grideados, tamanyo_imagen);

    //Libero memoria
    liberarMatrices(tamanyo_imagen);
    printf("Fin del programa local\n");

    return 0;
}
