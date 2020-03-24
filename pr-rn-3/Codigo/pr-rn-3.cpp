#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>

using namespace std;


void isi_and_N_dist( const char* spikes)
{
	ifstream spikes_file(spikes);
	ofstream isi_distribution("isi_dist.dat");
	ofstream n_distribution("N_dist.dat");

	string line_spikes;

	float isi_2=0.0, isi=0.0;
	float N_isi=0;

	float N_2=0.0, N=0.0;
	float N_N=0;

	int counter=0,  N_spikes=0;

		while (!spikes_file.eof() )
		{	getline(spikes_file,line_spikes);

			if (line_spikes.size()) //Si el tamano no es nulo
			{	for (std::string::size_type i = 0; i < line_spikes.size(); i++) 
				{	
					switch(line_spikes[i]) 
					{	case '0':
							if (counter) counter++; break;

						case '1':
							if (counter) {
								isi_distribution << counter << endl;
								isi+=counter;
								isi_2+= counter*counter;
								N_isi++;
								counter=0;
							}

							else  {counter++;}
							N_spikes++;
							break;
					}
				}
			
			n_distribution << N_spikes << endl;
			N+=1.0*N_spikes;
			N_2+=1.0*N_spikes*N_spikes;
			N_N++;
			N_spikes=0;		
			}	


			
		}

	isi= isi/N_isi;
	isi_2=isi_2/N_isi;

	N= N/N_N;
	N_2=N_2/N_N;

	double CV = sqrt(isi_2 - isi*isi)/ isi;
	double F  = (N_2 - N*N)/N;

	cout << "ISI medio: "<< isi << endl;
	cout << "ISI varianza: "<< isi_2 - isi*isi << endl;

	cout << "Factor CV "<< CV << endl;

	cout << "N medio: "<< N << endl;
	cout << "N varianza: "<< N_2 - N*N << endl;
	cout << "N medio: "<< N << endl; 
	cout << "Factor de Fano "<< F<< endl;



	spikes_file.close();
	isi_distribution.close();
	n_distribution.close();
}


void q_rs(const char* stimulus, const char* hist_spikes, const char* filtro)
{
	ifstream stimulus_file(stimulus);
	ifstream hist_spikes_file(hist_spikes);
	ofstream filtro_file(filtro);

	int counter=0, t_s=0, t_r=0, t_evaluate=0, spikes=0;
	float sig=0.0, sum=0.0;

	string line_signal, line_spikes;

	vector<float> vector_signal;
	vector<int> vector_spikes;

		while (!hist_spikes_file.eof() )
		{
			getline(hist_spikes_file,line_spikes);			
			stringstream sspikes(line_spikes);			
			sspikes >> t_r >> spikes; 
			vector_spikes.push_back(spikes);
		}

		while (!stimulus_file.eof())
		{	
			getline(stimulus_file,line_signal);			
			stringstream ssenal(line_signal);			
			ssenal >> t_s >> sig; 
			vector_signal.push_back(sig);
		}	

	//longitud signal
			int N_signal = 10001;


	for (int i = -500; i < 10000; ++i) //Este es tau
	{
		for (int j = 0; j < N_signal; ++j) //Esto recorre el valor de la señal
		{	t_evaluate= j+i;
			
			if (   t_evaluate>=0 
				&& t_evaluate<N_signal 
				&& vector_spikes[j]!=0) sum+=vector_signal[t_evaluate];
				
		}
		filtro_file << i << "\t" << "\t" << sum << endl;

		sum=0.0;
	}


	stimulus_file.close();
	hist_spikes_file.close();
	filtro_file.close();
}


int main(int argc, char const *argv[])
{	
	//isi_and_N_dist("spikes.dat");

	q_rs("stimulus.dat", "sum_spikes_por_celda.dat", "filtro.dat");

	return 0;
}