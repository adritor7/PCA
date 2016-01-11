#include <fstream>;
#include <cstdio>;
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <mkl.h>
#include <errno.h>
#include "FileManager.h"

using namespace std;

void getEigenVector(double eigenvectors[],double eigenvector[],int column,int numberOfColumn){
	int countOfPositions = 0;
	for (int i = column; i < numberOfColumn*numberOfColumn; i = i + 4) {
		eigenvector[countOfPositions] = eigenvectors[i];
		countOfPositions++;
	}
}
void copyEigenVector(double eigenvectors[], double eigenvector[],int column,int numberOfColumn){
	int countOfPositions = 0;
	for (int i = column; i < numberOfColumn*numberOfColumn; i = i + 4) {
		eigenvectors[i] = eigenvector[countOfPositions];
		countOfPositions++;
	}
}

void quicksort(double eigenvalues[],double eigenvectors[] ,int izq, int der,int numberOfColumn){
	int i, j;
	i = izq;
	j = der;
	double pivote = (eigenvalues[i] + eigenvalues[j]) / 2;
	double auxEigenvalue = 0;
	double *firstEigenvector = (double *)mkl_malloc(numberOfColumn*sizeof(double), 64);
	double *secondEigenvector = (double *)mkl_malloc(numberOfColumn*sizeof(double), 64);
	while (i < j){
		while (eigenvalues[i] > pivote && i < der) i += 1;
		while (pivote > eigenvalues[j] && j > izq) j -= 1;
		if (i <= j){
			auxEigenvalue = eigenvalues[i];
			getEigenVector(eigenvectors, firstEigenvector, i, numberOfColumn);
			eigenvalues[i] = eigenvalues[j];
			getEigenVector(eigenvectors, secondEigenvector, j, numberOfColumn);
			copyEigenVector(eigenvectors, secondEigenvector, i, numberOfColumn);
			eigenvalues[j] = auxEigenvalue;
			copyEigenVector(eigenvectors, firstEigenvector, j,numberOfColumn);
			i += 1;
			j -= 1;
		}
	}
	if (izq < j){
		quicksort(eigenvalues,eigenvectors,izq, j,numberOfColumn);
	}
	if (der > i){
		quicksort(eigenvalues, eigenvectors,i, der,numberOfColumn);
	}
}
int main(int argc, char* argv[]){
	//Lectura del fichero
	FileManager fm("iris.dat");
	int numeroDeFilas = fm.dameNumRegistros();
	int numeroDeColumnas = fm.dameNumCampos();
	
	double *x = (double *)mkl_malloc(numeroDeColumnas*numeroDeFilas*sizeof(double), 64);
	fm.leeFicheroDeDatos(x);

	//Lectura del fichero
	//Calcular media para cada columna;
	double *media = (double *)mkl_malloc(numeroDeFilas*sizeof(double), 64);
	for (int i = 0; i < numeroDeFilas; i++){
		for (int j = 0; j < numeroDeColumnas; j++){
			media[j] += x[i*numeroDeColumnas + j];
		}
	}
	for (int j = 0; j < numeroDeColumnas; j++){
		media[j] = media[j] / numeroDeFilas;
		//printf("Centro de datos %f \n", media[j]);

	}
	//Calcular media para cada columna;
	//Normalización de las coordenadas de los puntos
	for (int i = 0; i < numeroDeFilas; i++){
		for (int j = 0; j < numeroDeColumnas; j++){
			x[i*numeroDeColumnas+j] = x[i*numeroDeColumnas+j] - media[j];
			//printf("%f ", x[i + j]);
		}
		//printf("\n");
	}
	//Normalización de las coordenadas de los puntos
	//Calculamos la covarianza Z
    double *z = (double*)mkl_malloc(numeroDeColumnas*numeroDeColumnas*sizeof(double), 64);
	cblas_dgemm(CblasRowMajor, CblasTrans, CblasNoTrans, numeroDeColumnas, numeroDeColumnas, numeroDeFilas, 1.0/numeroDeFilas, x, numeroDeColumnas, x, numeroDeColumnas, 0.0, z, numeroDeColumnas);
	/*for (int i = 0; i < numeroDeColumnas; i++){
		for (int j = 0; j < numeroDeColumnas; j++){
			printf("%f ", z[i*numeroDeColumnas + j]);
		}
		printf("\n");
	}*/
	//Calculo de los autovalores y de los autovectores
	double *eigenvalues = (double*)mkl_malloc(numeroDeColumnas*sizeof(double), 64);
	int result = LAPACKE_dsyev(LAPACK_ROW_MAJOR, 'V', 'U', numeroDeColumnas, z, numeroDeColumnas, eigenvalues);
	/*for (int i = 0; i < numeroDeColumnas; i++){
		printf("%lf ", eigenvalues[i]);
	}
	printf("\n");
	for (int i = 0; i < numeroDeColumnas; i++){
		for (int j = 0; j < numeroDeColumnas; j++){
			printf("%lf ", z[i * 4 + j]);
		}
		printf("\n");
	}*/
	quicksort(eigenvalues, z, 0, numeroDeColumnas-1,numeroDeColumnas);
	/*for (int i = 0; i < numeroDeColumnas; i++){
		printf("%lf ", eigenvalues[i]);
	}
	printf("\n");
	for (int i = 0; i < numeroDeColumnas; i++){
		for (int j = 0; j < numeroDeColumnas; j++){
			printf("%lf ", z[i * 4 + j]);
		}
		printf("\n");
	}*/
	// Calcular Y = X*V
	double *y = (double*)mkl_malloc(numeroDeColumnas*numeroDeFilas*sizeof(double), 64);
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, numeroDeFilas, numeroDeColumnas, numeroDeColumnas, 1.0, x, numeroDeColumnas, z, numeroDeColumnas, 0.0, y, numeroDeColumnas);
	for (int i = 0; i < numeroDeFilas; i++){
		for (int j = 0; j < numeroDeColumnas; j++){
			printf("%lf ", y[i * numeroDeColumnas + j]);
		}
		printf("\n");
	}
	// Escritura en el fichero
	FILE *out;
	errno = 0;
	errno = fopen_s(&out,"iris_pca.dat","w");
	//if (!out){
	if (errno != 0){
		printf("No es posible abrir el fichero de escritura");
		exit(0);
	}

	fprintf(out, "%d\n", numeroDeFilas);
	fprintf(out, "%d\n", numeroDeColumnas);
	for (int i = 0; i < numeroDeFilas; i++){
		for (int j = 0; j < numeroDeColumnas; j++){
			fprintf(out, "%lf ",y[i*numeroDeColumnas + j]);
		}
		//fprintf(out, "%d\n", tipo[i]);
	}
	fclose(out);
	getchar();
	return 0;
}
