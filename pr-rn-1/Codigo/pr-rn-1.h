#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <sstream>
#include "../../RK4/runge_kuta_4.h"

using namespace std;

const double V_na=  50;
const double V_K = -77;
const double V_l = -54.4;

const double g_na= 120;
const double g_K = 36 ;
const double g_l = 0.3;

const double C = 1.0;


//////////////////////////////////
// 			EJERCICIO 1			//			
//		 t; 	t 				//	
//		 var1;	V				//	
//		 var2;	m				//	
//		 var3;	h				//	
//		 var4;  n				//		
//////////////////////////////////


///////////////////////////////////////////////////
double a_m(double V) {		return 0.1*(V + 40.0)/(1.0-exp(-0.1*V-4.0)); }

double b_m(double V) {		return 4.0*exp((-V -65.0)/18.);}

double a_h(double V) { 		return 0.07*exp((-V -65.)/20.); }

double b_h(double V) {		return 1.0/(1.0 + exp(-0.1*(V + 35.0))); }

double a_n(double V) {		return 0.01*(V + 55.)/(1.0-exp(-0.1*V-5.5)); }

double b_n(double V) {		return 0.125*(exp(-(V + 65.)/80.)); }

double x_inf(double V, parameter a_x, parameter b_x)
{ 	return a_x(V)/(a_x(V) + b_x(V)); }

double tau_x(double V, parameter a_x, parameter b_x)
{	return 1.0/(a_x(V) + b_x(V)); }

double x_t (double V , double x, parameter ax, parameter bx)
{	
	return (x_inf(V, ax, bx) - x)/tau_x(V, ax, bx); }

double V_t( double V, double m, double h, double n , double I)
{	return (I -g_na*m*m*m*h*(V-V_na) - g_K*n*n*n*n*(V - V_K) - g_l*(V-V_l)); }
////////////////////////////////////////////////////////////////////


void function_update_HH( rk_vector* x_0, rk_vector* aux , rk_vector* v, float I, float factor)
{		rk_vector f_i;

		f_i.var1 = V_t(aux->var1, aux->var2, aux->var3, aux->var4, I);
		f_i.var2 = x_t(aux->var1, aux->var2, a_m, b_m);
		f_i.var3 = x_t(aux->var1, aux->var3, a_h, b_h);
		f_i.var4 = x_t(aux->var1, aux->var4, a_n, b_n);

		x_increment(x_0, &f_i);

		aux->var1=v->var1 + factor*x_0->var1;
		aux->var2=v->var2 + factor*x_0->var2;
		aux->var3=v->var3 + factor*x_0->var3;
		aux->var4=v->var4 + factor*x_0->var4;
}


float HogdkinHuxleyNeuron(rk_vector* v, double h, double I, int n, int N, ofstream& myfile, bool print_file)
{	
	rk_vector x0; x0.t=h;
	rk_vector x1; x1.t=h;
	rk_vector x2; x2.t=h;
	rk_vector x3; x3.t=h;

	rk_vector aux ={0,0,0,0,0};

	int flag=0;

	double t_i=0.0, t_f=0.0;

		for (int i = n; i < N; ++i)
			{	copy_rk_vector(v, &aux);
	
				function_update_HH(&x0, &aux, v, I, 0.5); //x0
				function_update_HH(&x1, &aux, v, I, 0.5); //x1
				function_update_HH(&x2, &aux, v, I, 1.);  //x2
				function_update_HH(&x3, &aux, v, I, 0.);  //x3	
	
				aux.var1 = update_x(v->var1, x0.var1, x1.var1, x2.var1, x3.var1);
				aux.var2 = update_x(v->var2, x0.var2, x1.var2, x2.var2, x3.var2);
				aux.var3 = update_x(v->var3, x0.var3, x1.var3, x2.var3, x3.var3);
				aux.var4 = update_x(v->var4, x0.var4, x1.var4, x2.var4, x3.var4);
				aux.t+=h;
	
				if (v->var1>0.0 && i >0.5*(float)N && aux.var1< 0.0 && flag==0) {t_i=v->t; flag++;} 
				if (v->var1>0.0 && i >0.5*(float)N && aux.var1< 0.0 && flag==1  && abs(aux.t-v->t)>h)  {t_f=v->t -h*v->var1/(v->var1 - aux.var1); flag++;} 

				v->t+=h;
				copy_rk_vector(&aux, v);
		
				if(print_file) myfile << v->t << "\t" << v->var1 << "\t" << v->var2 << "\t" << v->var3 << "\t" << v->var4<< "\t" << I << endl; 	

		}
		
	if (flag==2) return t_f - t_i;
	else {return 0;}
}



/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

const double A_0=500.0;

double V_t_Int_and_Fire(rk_vector* v, double  I ,rk_function A) 
{	
	return 1.*(-v->var1 + I - A(*v)); }

double A_t_Int_and_Fire(rk_vector v)
{
	if (v.var1>=1) return 1.0*(-v.var4+A_0);
	else return -1.0*v.var4;
}

void function_update_I_and_F( rk_vector* x_0, rk_vector* aux , rk_vector* v, double I, float factor)
{
		rk_vector f_i = {0.,0.,0.,0.,0.};

		f_i.var1 = V_t_Int_and_Fire(aux, I, A_t_Int_and_Fire);
		f_i.var4 = A_t_Int_and_Fire(*aux);

		x_increment(x_0, &f_i);

		aux->var1=v->var1 + factor*x_0->var1;
		aux->var4=v->var4 + factor*x_0->var4;
}