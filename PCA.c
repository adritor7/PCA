#include <fstream>;
#include <cstdio>;
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <mkl.h>

using namespace std;

int main(int argc, char* argv[]){
	//Lectura del fichero
	int numeroDeFilas = 0;
	char linea[100];
	char *numeros;
	int numeroDeColumnas = 0;
	int posicion = 0;
	ifstream in("iris.dat");
	if (!in.good()){
		printf("No se ha abierto correctamente el fichero");
		exit(0);
	}
	in.getline(linea, 100);
	numeroDeFilas = atoi(linea);
	in.getline(linea, 100);
	numeroDeColumnas = atoi(linea);
	double *x = (double *)mkl_malloc(numeroDeColumnas*numeroDeFilas*sizeof(double), 64);
	int *tipo = (int *)mkl_malloc(numeroDeFilas*sizeof(int), 64);
	for (int i = 0; i < numeroDeFilas; i++){
		in.getline(linea, 100);
		numeros = strtok(linea, " ");
		for (int j = 0; j < numeroDeColumnas; j++){
			x[posicion] = atof(numeros);
			numeros = strtok(NULL, " ");
			posicion++;
		}
		tipo[i] = atoi(numeros);
	}
	in.close();
	//Lectura del fichero
	//Calcular media para cada columna;
	double *media = (double *)mkl_malloc(numeroDeFilas*sizeof(double), 64);
	for (int i = 0; i < numeroDeColumnas*numeroDeFilas; i = i + 4){
		for (int j = 0; j < numeroDeColumnas; j++){
			media[j] += x[i+j];
		}
	}
	for (int j = 0; j < numeroDeColumnas; j++){
		media[j] = media[j] / numeroDeFilas;
	}
	//Calcular media para cada columna;
	//Normalización de las coordenadas de los puntos
	for (int i = 0; i < numeroDeColumnas*numeroDeFilas; i = i + 4){
		for (int j = 0; j < numeroDeColumnas; j++){
			x[i+j] = x[i+j] - media[j];
			printf("%f", x[i + j]);
		}
		printf("\n");
	}
	//Normalización de las coordenadas de los puntos
	//Calculamos la covarianza Z
	double *z = (double*)mkl_malloc(numeroDeColumnas*numeroDeFilas*sizeof(double), 64);
	cblas_dgemm(CblasRowMajor, CblasTrans, CblasNoTrans, numeroDeColumnas, numeroDeColumnas, numeroDeColumnas, 1.0/numeroDeFilas, x, numeroDeFilas, x, numeroDeColumnas, 0.0, z, numeroDeColumnas);
	/*for (int i = 0; i < numeroDeColumnas*numeroDeFilas; i = i+4){
		for (int j = 0; j < numeroDeColumnas; j++){
			printf("%f ", z[i + j]);
		}
		printf("\n");
	}*/
	std::getchar();
	return 0;
}