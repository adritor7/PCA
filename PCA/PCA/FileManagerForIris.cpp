#include "FileManagerForIris.h"
using namespace std;

int dameNumRegistros() {
	return 150;
}

int dameNumCampos() {
	return 4;
}

int leeFicheroDeDatos(double* dst) {
	
	ifstream in("data.csv");
	if (!in.good()) {
		printf("No se ha abierto correctamente el fichero");
		exit(0);
	}

	char linea[100];
	char *numeros;
	char* context = NULL;

	for (int i = 0; i < dameNumRegistros(); i++) {
		in.getline(linea, 100);
		numeros = strtok_s(linea, ",", &context);
		for (int j = 0; j < dameNumCampos(); j++) {
			dst[i*dameNumCampos() + j] = atof(numeros);
			numeros = strtok_s(NULL, ",", &context);
		}
	}
	in.close();
}

int escribeSalida(double* data) {
	return 0;
}