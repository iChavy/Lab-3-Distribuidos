gridding: gridding.o 
	gcc -fopenmp -o gridding gridding.o -lm

gridding.o: gridding.c
	gcc -fopenmp -c gridding.c

clean:
	rm -f gridding *.o 

#./gridding -i datosuv.txt -o datosgrideados.raw -d deltax -N tamañoimagen -c chunklectura -t numerotareas

#./gridding -i hltau_completo_uv.csv -o datosgrideados.raw -d 0.003 -N 2048 -c 3 -t 3

#./gridding -i prueba100.csv -o datosgrideados.raw -d 0.003 -N 2048 -c 3 -t 3
