#pragma once
#include <fstream>;
#include <cstdio>;
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <mkl.h>

//registers == rows, fields == columns
int dameNumRegistros();
int dameNumCampos();
int leeFicheroDeDatos(double* dst);
int escribeSalida(double* data);