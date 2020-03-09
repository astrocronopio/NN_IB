#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <sstream>


using namespace std;


typedef double parameter (double);

/////////////////////////////////////////////
typedef struct      //Genero una estructura para poder
{   double t; 
	double var1;		// llamar a cada coordenada por su nombre
	double var2;
	double var3;
	double var4; 
	double var5; 
} rk_vector;

typedef double rk_function (rk_vector);

void x_increment( rk_vector * x_0, rk_vector *f_i)
{
	x_0->var1 = x_0->t * f_i->var1;
	x_0->var2 = x_0->t * f_i->var2;
	x_0->var3 = x_0->t * f_i->var3;
	x_0->var4 = x_0->t * f_i->var4;
	x_0->var5 = x_0->t * f_i->var5;	
}

//////////////////////////////////////////////
double update_x(double x, double x_0, double x_1, double x_2, double x_3)
{	return x + (x_0 + 2.*x_1 + 2.*x_2 +x_3)/6.; }

/////////////////
void copy_rk_vector(rk_vector* in, rk_vector* out)
{		out->var1 = in->var1;
		out->var2 = in->var2;
		out->var3 = in->var3;
		out->var4 = in->var4;
		out->var5 = in->var5;
}
