#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <sstream>
#include "pr-rn-1.h"

using namespace std;


///////////////////////////////////////////////////////////////////////////////
/*
Para este ejercicio:

*/


void Ejercicio4()
{	
	rk_vector v ={0.0,0.0,0.4,0.4,0.2};

	int n = 2500;
	int n_i=8000;

	double h=0.1;

	double current=-10.000;

	float period=0.0;

	ofstream myfile ("ejercicio_4.txt");

	ofstream freq_Current("freq_current_ej_4.txt");

	for (int i = 0; i < n_i; ++i)
	{	
		current = (i> 0.5*(float)n_i ? current-0.01 : current+0.01);

		if (i==0.5*(float)n_i ) freq_Current <<"\n\n\n\n\n\n\n"<< endl;
		period=HogdkinHuxleyNeuron(&v, h, current, 0, n, myfile, false);
		freq_Current << current << "\t" << period <<endl;

	}


	myfile.close();
	freq_Current.close();
}

void Ejercicio5()
{
	rk_vector v ={0.0,0.0,0.1,0.1,0.1};

	int n = 10000;

	double h=0.1;
	float period=0.0;

	ofstream myfile ("ejercicio_5.txt");

	period=HogdkinHuxleyNeuron(&v, h, 0.0 , 0	  , n 			, myfile, true);
	period=HogdkinHuxleyNeuron(&v, h, -4, n 	  , n+1000		, myfile, true);
	period=HogdkinHuxleyNeuron(&v, h, 0.0 , n+1000, n*1.5 		, myfile, true);

	myfile.close();
}


///////////////////////////////////////////////////////////////////////////////
/*
Para este ejercicio:
V ----- V: igual
m ----- I: representa la corriente
n ----- A: representa la adaptación A

*/
float Integrate_and_FireNeuron(rk_vector* v, double h, double I, int n, int N, ofstream& myfile, bool print_file)
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
	
				function_update_I_and_F(&x0, &aux, v, I, 0.5); //x0
				function_update_I_and_F(&x1, &aux, v, I, 0.5); //x1
				function_update_I_and_F(&x2, &aux, v, I, 1.); //x2
				function_update_I_and_F(&x3, &aux, v, I, 0.); //x3	
	
				aux.var1=update_x(v->var1, x0.var1, x1.var1, x2.var1, x3.var1);
				aux.var4=update_x(v->var4, x0.var4, x1.var4, x2.var4, x3.var4);
				aux.t+=h;
	
				v->t+=h;
				copy_rk_vector(&aux, v);
		
				if(print_file) myfile << v->t << "\t" << v->var1 << "\t"  << v->var4<< "\t" << I << endl; 	
		}
	if (flag==2) return t_f - t_i;
	else {return 0;}
}

float FitzHugh_Nagumo(rk_vector* v, double h, double I, int n, int N, ofstream& myfile, bool print_file)
{
	return 0;
}



void Ejercicio6()
{
	int n = 5000;
	int n_i=1;

	double h=0.1;
	double I=1.000;

	float period=0.0;

	ofstream myfile ("ejercicio_6.txt");

	ofstream freq_Current("freq_current_ej_6.txt");

	rk_vector v ={0.0,10.0,0.0,10.0,0.0};

	for (int i = 0; i < n_i; ++i)
	{	
		period=Integrate_and_FireNeuron(&v, h, I+i*0.01, 0, n, myfile, true);
		freq_Current << I+i*0.01 << "\t" << period <<endl;
	}

	myfile.close();
	freq_Current.close();
}


int main(int argc, char const *argv[])
{	if(argc==1) cout<<"¿Ejercicio?"<<endl;

	else if(argc>1 && *argv[1]=='4') Ejercicio4();
	else if(argc>1 && *argv[1]=='5') Ejercicio5();
	else if(argc>1 && *argv[1]=='6') Ejercicio6();
	
	else{
		cout<<"Unexpected"<<endl;
	}


	return 0;
}