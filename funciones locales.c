void inicializarMatricesLocales(double **matriz_fr, double **matriz_fi, double **matriz_wr, int tamanyo_imagen)
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

void matrizLocal(char *nombre_archivo_entrada, int chunk_lectura, int numero_tareas, int tamanyo_imagen, double delta_u, double delta_v)
{
    double **matriz_fr_local = (double **)malloc(tamanyo_imagen * sizeof(double *));
    double **matriz_fi_local = (double **)malloc(tamanyo_imagen * sizeof(double *));
    double **matriz_wr_local = (double **)malloc(tamanyo_imagen * sizeof(double *));

    double **matriz_fr_global = (double **)malloc(tamanyo_imagen * sizeof(double *));
    double **matriz_fi_global = (double **)malloc(tamanyo_imagen * sizeof(double *));
    double **matriz_wr_global = (double **)malloc(tamanyo_imagen * sizeof(double *));

    for (int i = 0; i < tamanyo_imagen; i++)
    {
        matriz_fr_local[i] = (double *)malloc(tamanyo_imagen * sizeof(double));
        matriz_fi_local[i] = (double *)malloc(tamanyo_imagen * sizeof(double));
        matriz_wr_local[i] = (double *)malloc(tamanyo_imagen * sizeof(double));

        matriz_fr_global[i] = (double *)malloc(tamanyo_imagen * sizeof(double));
        matriz_fi_global[i] = (double *)malloc(tamanyo_imagen * sizeof(double));
        matriz_wr_global[i] = (double *)malloc(tamanyo_imagen * sizeof(double));
    }
    
    inicializarMatricesLocales(matriz_fr_local, matriz_fi_local, matriz_wr_local, tamanyo_imagen);
    inicializarMatricesLocales(matriz_fr_global, matriz_fi_global, matriz_wr_global, tamanyo_imagen);

    // Abro archivo para lectura binaria
    FILE *archivo_entrada = fopen(nombre_archivo_entrada, "rb");

    bool flag = true;
    
    if (archivo_entrada == NULL)
    {
        fprintf(stderr, "Error al abrir el archivo de entrada\n");
        exit(EXIT_FAILURE);
    }
    // Sección paralela <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ACA SE PONE PRIVATE????
    #pragma omp parallel num_threads(numero_tareas) private(matriz_fr_local, matriz_fi_local, matriz_wr_local)
    {
        #pragma omp single
        {
            for (int i = 1; i <= numero_tareas; ++i) 
            {
                //printf("mi id: %d\n", omp_get_thread_num());
                
                #pragma omp task
                {
                    // Mientras haya lineas por leer, flag = true
                    while (flag)
                    {
                        char *linea_aux;
                        char linea[MAX];
                        char matriz[chunk_lectura][MAX];
                        int contador = 0, i_k, j_k;
                        double u, v, visibilidad_real, visibilidad_im, peso_w, frec_obs, u_k, v_k;
                        // EMPEZARA ACA LA SC???????????????????????????????????????????????????????????????
                        for (int i = 0; i < chunk_lectura; i++)
                        {
                            #pragma omp critical
                            {
                                //probar con fgets
                                if (fgets(linea, sizeof(linea), archivo_entrada) == NULL)
                                {
                                    flag = false;
                                    //break;
                                }
                                else{
                                    // guardo la linea en matriz
                                    strcpy(matriz[i], linea);
                                    contador++;
                                }
                            } 
                        }
                        for (int i = 0; i < contador; i ++)
                        {
                            linea_aux = strtok(matriz[i], ",");
                            u = strtod(linea_aux, NULL);
                            v = strtod(strtok(NULL, ","), NULL);
                            linea_aux = strtok(NULL, ",");
                            visibilidad_real = strtod(strtok(NULL, ","), NULL);
                            visibilidad_im = strtod(strtok(NULL, ","), NULL);
                            peso_w = strtod(strtok(NULL, ","), NULL);
                            frec_obs = strtod(strtok(NULL, ","), NULL);
                            
                            // Transformación de las coordenada u, v a longitud de onda
                            u_k = u * (frec_obs / VEL_LUZ);
                            v_k = v * (frec_obs / VEL_LUZ);
                            //  Determina la posición de la matriz que corresponde la visibilidad
                            i_k = round((u_k / delta_u) + tamanyo_imagen / 2);
                            j_k = round((v_k / delta_v) + tamanyo_imagen / 2);

                            // Acumula en matriz
                            matriz_fr_local[i_k][j_k] += peso_w * visibilidad_real;
                            matriz_fi_local[i_k][j_k] += peso_w * visibilidad_im;
                            matriz_wr_local[i_k][j_k] += peso_w;
                        }
                    }
                }
            }
        }
    }

    // PREGUNTAR COMO SUMAR MATRICES DE LAS TAREAS<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    // Libera las matrices locales


    // Cierro archivo
    fclose(archivo_entrada);
}

/*void calculoVisibilidad(int *i_k, int *j_k, double *peso_w, double *visibilidad_real, double *visibilidad_im, int contador, double delta_u, double delta_v, int tamanyo_imagen, char **matriz)
{
    double u, v, frec_obs, u_k, v_k;

    for (int i = 0; i < contador; i++)
    {
        u = matriz[i][0];
        v = matriz[i][1];
        *visibilidad_real = matriz[i][3];
        *visibilidad_im = matriz[i][4];
        *peso_w = matriz[i][5];
        frec_obs = matriz[i][6];

        // Transformación de las coordenada u, v a longitud de onda
        u_k = u * (frec_obs / VEL_LUZ);
        v_k = v * (frec_obs / VEL_LUZ);

        //  Determina la posición de la matriz que corresponde la visibilidad
        *i_k = round((u_k / delta_u) + tamanyo_imagen / 2);
        *j_k = round((v_k / delta_v) + tamanyo_imagen / 2);
    }

}*/
