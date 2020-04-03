#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <sstream>


using namespace std;



void initialize(double * matrix)
{

}

//Para el ejercicio 1, es una matriz de 5 neuronas y una de T offset
double create_weight_matrix(int N, bool bias)
{		double matrix=0;

		int neuronas = bias? N+1: N;
		initialize(&matrix);

		return matrix;
}



void destroy_matrix(double * matrix, int N)
{


}


void xor_nn() //Entrada
{
	
	
}

void ejercicio_1_xor_2D()
{	
	double matrix = create_weight_matrix(5, true);

	int epochs = 1000;

	for (int i = 0; i < epochs; ++i)
	{
		//xor_nn(entradas, matrix, salidas, myfile)
	}

}

void ejercicio_2_xor_nD(){

}


void ejercicio_3_logistica(){

}

int main(int argc, char const *argv[])
{
	ejercicio_1_xor_2D();
	ejercicio_2_xor_nD();
	ejercicio_3_logistica();
	return 0;
}