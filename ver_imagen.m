% Abre los archivos de datos grideados con matrices compartidas
f1 = fopen("datosgrideadosr.raw", "rb");
f2 = fopen("datosgrideadosi.raw", "rb");

% Abre los archivos de datos grideados con matrices locales
f3 = fopen("datosgrideadosr_local.raw", "rb");
f4 = fopen("datosgrideadosi_local.raw", "rb");

% Lee los archivos con matrices globales que poseen valores tipo float de doble precisión
s1 = fread(f1, "double");
s2 = fread(f2, "double");

% Lee los archivos con matrices locales que poseen valores tipo float de doble precisión
s3 = fread(f3, "double");
s4 = fread(f4, "double");

% Cierra los archivos
fclose(f1);
fclose(f2);
fclose(f3);
fclose(f4);

% Toma los vectores creados anteriormente y los transforma en una matriz
re1 = reshape(s1, 2048, 2048);
im1 = reshape(s2, 2048, 2048);
re2 = reshape(s3, 2048, 2048);
im2 = reshape(s4, 2048, 2048);

% Crea vectores complejos
v1 = complex(re1, im1);
v2 = complex(re2, im2);

% Realiza la transformada inversa de Fourier
I =  fftshift(ifft2(v1));
J =  fftshift(ifft2(v2));

% Muestra las imágenes en una sola figura usando subplot
figure;

subplot(1, 2, 1);
imagesc(abs(I));
colormap('hot');
title('Imagen resultante con matrices compartidas');

subplot(1, 2, 2);
imagesc(abs(J));
colormap('hot');
title('Imagen resultante con matrices locales');
