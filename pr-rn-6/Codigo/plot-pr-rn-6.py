import numpy as np
import matplotlib.pyplot as plt

import matplotlib as mpl
mpl.rcParams.update({'font.size': 19,  'figure.figsize': [10, 8],  'font.family': 'serif', 'font.sans-serif': ['Helvetica']})

colormap = plt.cm.gist_ncar


def ejer_1():
	N_vec=[500]#[500, 1000 , 2000, 4000]
	alpha_vec = [0.12, 0.14, 0.16,0.18]

	files = []

	for N in N_vec: 
		for alpha in alpha_vec: 
			files.append("./Data_files/test{}_{}".format(N, alpha))

	f, ax = plt.subplots(2, 2, sharey=True, sharex=True)
	f.subplots_adjust(hspace=0, wspace=0, top=0.95, right=0.95)
	#f.suptitle("Para $N={}$".format(N_vec[0]))
	
	counterx=0
	countery=0
	counter=0
	

	ax[counterx][countery].set_ylabel("$N_{cuentas}/N_{patrones}$")

	for filename  in files:
		binwidth=0.05
		m = np.loadtxt(filename, unpack=True)
		bins=np.arange(min(m), max(m) + binwidth, binwidth)
		weights = np.ones_like(m) / len(m)
		ax[counterx][countery].set_xticks(np.arange(0, 1.2, step=0.2)) 

		#ax[counterx][countery].set_title("$\\alpha$="+str(alpha_vec[counter]))
		
		ax[counterx][countery].hist(m, weights=weights, alpha=0.6, color='red', bins=bins, label="$\\alpha$="+str(alpha_vec[counter]))
		ax[counterx][countery].set_xlim(0.1,1.1)
		ax[counterx][countery].set_ylim(0.01,1.1)
		ax[counterx][countery].legend(loc='upper left', handlelength=0)
		
		counter+=1
		countery+=1
		countery=countery%2
		if counter==2:
				counterx=1
				ax[counterx][countery].set_ylabel("$N_{cuentas}/N_{patrones}$")
		if counterx==1:
				ax[counterx][countery].set_xlabel(" $m_\\mu$")

		
	plt.savefig("../Graficos/{}.png".format(N_vec[0]))


def ejer_2():
	N=4000
	p=40

	files=np.array([])
	mean_files=np.array([])
	stdev_files=np.array([])
	t = np.arange(0.1, 2.1, 0.1)
	beta_vec=[]

	for T in t:
		files=np.append(files, "./Data_files/temp{}".format(T))
		m = np.loadtxt("./Data_files/temp{}".format(T), unpack=True)
		
		mean = np.mean(m)
		stdev= np.std(m)

		beta = T
		mean_files=np.append(mean_files,mean)
		stdev_files=np.append(stdev_files,stdev)
		beta_vec = np.append(beta_vec, beta)

	print(t)
	print(files)
	print(mean_files)
	plt.plot(beta_vec, mean_files, color='red', alpha=0.8)

	plt.ylabel("Overlap $m_\\mu$")
	plt.xlabel("T")
	plt.fill_between(beta_vec, mean_files-stdev_files, mean_files+stdev_files, color='red', alpha=0.3)

	plt.savefig("../Graficos/beta.png")
	
def main():
	ejer_1()
	#ejer_2()
	plt.show()


if __name__== "__main__":
	main()
