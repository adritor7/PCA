#include "FileManager.h"
using namespace std;

FileManager::FileManager(char* s) {
	fileName = s;

	ifstream in(fileName);
	if (!in.good()) {
		printf("No se ha abierto correctamente el fichero");
		exit(0);
	}

	char linea[100];
	char *numeros;
	char* context = NULL;

	in.getline(linea, 100);
	registers = atoi(linea);
	in.getline(linea, 100);
	columns = atoi(linea);

	skip = 2;
	in.close();
}

int FileManager::dameNumRegistros() {
	return registers;
}

int FileManager::dameNumCampos() {
	return columns;
}

int FileManager::leeFicheroDeDatos(double* dst) {
	
	ifstream in(fileName);
	if (!in.good()) {
		printf("No se ha abierto correctamente el fichero");
		exit(0);
	}

	char linea[100];
	char *numeros;
	char* context = NULL;

	for (int i = 0; i < skip; i++)
	{
		in.getline(linea, 100);
	}

	for (int i = 0; i < dameNumRegistros(); i++) {
		in.getline(linea, 100);
		numeros = strtok_s(linea, " ", &context);
		for (int j = 0; j < dameNumCampos(); j++) {
			dst[i*dameNumCampos() + j] = atof(numeros);
			numeros = strtok_s(NULL, " ", &context);
		}
	}
	in.close();
}

int FileManager::escribeSalida(double* data, char* outFileName) {
	return 0;
}