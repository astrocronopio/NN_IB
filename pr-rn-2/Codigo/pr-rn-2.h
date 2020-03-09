#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <sstream>
#include "../pr-rn-1/pr-rn-1.h"


using namespace std;

const float tau_s= 3.0;

float Heaviside(float V){
	float output = V>0.0 ? 1.0 : 0.0;
	return output;
}

float lineal_trigger_x(float  f_x, float f_y, float g_xx, float g_xy, float I_x){
	return -f_x + g_xx*f_x*Heaviside(f_x) - g_xy*f_y*Heaviside(f_y) + I_x;
}


/////////////////////////////////////////////

void function_update_ej1( rk_vector* x_0, rk_vector* aux , rk_vector* v, float factor)
{		rk_vector f_i;

		float g_ee=1.0, g_ii=1.0, g_ie=1.0, g_ei=1.0;

		f_i.var1 = lineal_trigger_x(aux->var1, aux->var2, g_ee, g_ei, aux->var3);/// e
		f_i.var2 = lineal_trigger_x(aux->var2, aux->var1, g_ii, g_ie, aux->var4);/// i

		f_i.var1 = f_i.var1 >= 0.0 ? f_i.var1: 0.0;
		f_i.var2 = f_i.var2 >= 0.0 ? f_i.var2: 0.0;

		x_increment(x_0, &f_i);

		aux->var1=v->var1 + factor*x_0->var1;
		aux->var2=v->var2 + factor*x_0->var2;
}

///////////////////////////////////////////////////

float s_inf(float V){
	return 0.5*(1+tanh(V*0.2));
}

float s_t(float V, float s){
	return (s_inf(V) - s)/tau_s;
}

const double V_syn=0.0;

double I_t( double V, double g_syn, double s)
{
	return -g_syn*s*(V-V_syn);
}

//////////////////////////////////////////////////

void function_update_HH_desfase( rk_vector* x_0, rk_vector* aux , rk_vector* v, float I, float factor)
{		rk_vector f_i;

		f_i.var1 = V_t(aux->var1, aux->var2, aux->var3, aux->var4, I);
		f_i.var2 = x_t(aux->var1, aux->var2, a_m, b_m);
		f_i.var3 = x_t(aux->var1, aux->var3, a_h, b_h);
		f_i.var4 = x_t(aux->var1, aux->var4, a_n, b_n);
		f_i.var5 = s_t(aux->var1, aux->var5);

		x_increment(x_0, &f_i);

		aux->var1=v->var1 + factor*x_0->var1;
		aux->var2=v->var2 + factor*x_0->var2;
		aux->var3=v->var3 + factor*x_0->var3;
		aux->var4=v->var4 + factor*x_0->var4;
		aux->var5=v->var5 + factor*x_0->var5;
}


void HH_neuron_t(rk_vector* v, rk_vector* aux ,double I, double h)
{	copy_rk_vector(v, aux);

	rk_vector x0, x1, x2, x3;
	x0.t=h, x1.t=h, x2.t=h, x3.t=h;
	
	function_update_HH_desfase(&x0, aux, v, I, 0.5); //x0
	function_update_HH_desfase(&x1, aux, v, I, 0.5); //x1
	function_update_HH_desfase(&x2, aux, v, I, 1.);  //x2
	function_update_HH_desfase(&x3, aux, v, I, 0.);  //x3	
	
	aux->var1 = update_x(v->var1, x0.var1, x1.var1, x2.var1, x3.var1);
	aux->var2 = update_x(v->var2, x0.var2, x1.var2, x2.var2, x3.var2);
	aux->var3 = update_x(v->var3, x0.var3, x1.var3, x2.var3, x3.var3);
	aux->var4 = update_x(v->var4, x0.var4, x1.var4, x2.var4, x3.var4);
	aux->var5 = update_x(v->var5, x0.var5, x1.var5, x2.var5, x3.var5);

	aux->t+=h;
}
