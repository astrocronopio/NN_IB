#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <sstream>
#include <stdlib.h>



const double V_syn=-80.0;

double I_t( double V, double g_syn, double s)
{
	return -g_syn*s*(V-V_syn);
}
#include "pr-rn-2.h"


using namespace std;
//////////////////////////////////
// 			EJERCICIO 1			//			
//		 t; 	t 				//	
//		 var1;	e				//	
//		 var2;	i				//	
//		 var3;	I_e				//	
//		 var4;  I_i				//		
//////////////////////////////////
float  linear_trigger_neuron(rk_vector* v, double h, 
	double I, int n, int N, ofstream& myfile, bool print_file)
{

	rk_vector x0; x0.t=h;
	rk_vector x1; x1.t=h;
	rk_vector x2; x2.t=h;
	rk_vector x3; x3.t=h;

	rk_vector aux ={0,0,0,0,0};

		for (int i = n; i < N; ++i)
			{	
				copy_rk_vector(v, &aux);
	
				function_update_ej1(&x0, &aux, v, 0.5); //x0
				function_update_ej1(&x1, &aux, v, 0.5); //x1
				function_update_ej1(&x2, &aux, v, 1.);  //x2
				function_update_ej1(&x3, &aux, v, 0.);  //x3	
	
				aux.var1=update_x(v->var1, x0.var1, x1.var1, x2.var1, x3.var1);
				aux.var2=update_x(v->var2, x0.var2, x1.var2, x2.var2, x3.var2);

				aux.t+=h;
	
				v->t+=h;
				copy_rk_vector(&aux, v);
		
				if(print_file) myfile << v->t << "\t" << v->var1 << "\t" << v->var2 << "\t" << v->var3 << "\t" << v->var4<< "\t" << I << endl; 	
		}
	return 0;
}

void Ejercicio1()
{	
	rk_vector v ={0.0,0.1,0.2,0.3,0.2};

	int n = 2500;
	int n_i=1;

	double h=0.1;

	double current=-10.000;

	float period=0.0;

	ofstream myfile ("ejercicio_1.txt");

	ofstream freq_Current("freq_current_ej_1.txt");

	for (int i = 0; i < n_i; ++i)
	{	
		period = linear_trigger_neuron(&v, h, current, 0, n, myfile, true);
		freq_Current << current << "\t" << period <<endl;
	}

	myfile.close();
	freq_Current.close();
}


//////////////////////////////////
// 			EJERCICIO 2			//			
//		 t; 	t 				//	
//		 var1;	V				//	
//		 var2;	m				//	
//		 var3;	h				//	
//		 var4;  n				//		
//////////////////////////////////


float HogdkinHuxleyNeuron_desfase(double h, double g_syn, int N, float* period, float* desfase, ofstream& myfile1, ofstream&myfile2, int I)
{	
	rk_vector aux1 ={0,0,0,0,0,0};
	rk_vector aux2 ={0,0,0,0,0,0};

	rk_vector v1 ={0,+80,0.1,0.3,0.1, 0.0};
	rk_vector v2 ={0,-80,0.1,0.7,0.3, 0.0};

	int flag=0, flag2=0;

	double t_i=0.0, t_f=0.0, t_2=0.0;

		for (int i = 0; i < N; ++i)
			{	
				HH_neuron_t(&v1, &aux1, I + I_t(v1.var1, g_syn, v1.var5), h);
				HH_neuron_t(&v2, &aux2, I + I_t(v2.var1, g_syn, v2.var5), h);

				if (v1.var1>0.0-40 && i >0.5*(float)N && aux1.var1< 0.0-40 && flag==0) {t_i=v1.t ; flag++;} 

				if (v1.var1>0.0-40 && i >0.5*(float)N && aux1.var1< 0.0-40 && flag==1 && abs(v1.t - t_i)>h) {t_f=v1.t ; flag++;} 
		
				if (v2.var1>0.0-40 && i >0.5*(float)N && aux2.var1< 0.0-40 && flag2==0) {t_2=v2.t ; flag2++;}
				//if (v2.var1>0.0 && i >0.5*(float)N && aux2.var1< 0.0 && flag2==1) {t_2=v2.t ; flag2++;}


				*period = t_f-t_i;
				//*desfase = (t_2 - t_f);
				//*desfase = (t_2 - t_f) > *period*0.5 ? *period - (t_2 - t_i)  :  t_2 - t_f;

				if (abs(fmod((t_2 - t_f),(*period)))> *period*0.5)
				{
					if (t_2>t_f) *desfase = fmod((t_2 - t_f),(*period)) -*period;
					else *desfase = fmod((t_2 - t_f),(*period))+ *period;
				}
				else *desfase = fmod((t_2 - t_f),(*period));

				myfile1 << v1.t << "\t" << v1.var1 << "\t" << v1.var2 << "\t" << v1.var3 << "\t" << v1.var4<< "\t" << I + I_t(v1.var1, g_syn, v1.var5) << "\t"<< v1.var5<<  endl; 	
				myfile2 << v2.t << "\t" << v2.var1 << "\t" << v2.var2 << "\t" << v2.var3 << "\t" << v2.var4<< "\t" << I + I_t(v2.var1, g_syn, v2.var5) << "\t"<< v2.var5<<  endl; 	


				v1.t+=h; v2.t+=h;
				copy_rk_vector(&aux1, &v1); copy_rk_vector(&aux2, &v2);
			}

	//cout << t_f << "\t" << t_2 << endl;
	
	if (flag==2) return t_f - t_i;

	else {return 0;}
}


void Ejercicio2()
{	

	int n = 50000;
	int n_i= 100;

	double g_syn=0.0;

	double h=0.005;

	float period=0.0, desfase=0.0;

	ofstream myfile1 ("ejercicio_2_1_current_15_in.txt");
	ofstream myfile2 ("ejercicio_2_2_current_15_in.txt");
	
	ofstream freq_Current("gsyn_T_desfase_ej_2_current_15_in.txt");

	for (int i = 0; i < n_i; ++i)
	{	
		g_syn= 2.0*(float)i/((float)n_i);
		HogdkinHuxleyNeuron_desfase(h, g_syn, n, &period, &desfase, myfile1, myfile2, 15);
		freq_Current << g_syn << "\t" << period << "\t" << desfase <<endl;
		myfile1<<"\n\n\n\n\n\n\n\n" <<endl;
		myfile2<<"\n\n\n\n\n\n\n\n" <<endl;
	}



	myfile1.close();
	
	freq_Current.close();
}

int main(int argc, char const *argv[])
{	if(argc==1) cout<<"¿Ejercicio?"<<endl;

	else if(argc>1 && *argv[1]=='1') Ejercicio1();
	else if(argc>1 && *argv[1]=='2') Ejercicio2();
	else{
		cout<<"Unexpected"<<endl;
	}

	return 0;
}