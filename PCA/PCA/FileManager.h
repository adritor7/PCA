#pragma once
#include <fstream>;
#include <cstdio>;
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <mkl.h>


class FileManager {
	char* fileName;
	int registers, columns,skip;

public:
	FileManager(char* s);
	int dameNumRegistros();
	int dameNumCampos();
	int leeFicheroDeDatos(double* dst);
	int escribeSalida(double* data,char* outFileName);
};
