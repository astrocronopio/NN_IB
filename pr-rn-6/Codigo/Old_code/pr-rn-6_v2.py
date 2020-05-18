#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
import threading as th

import matplotlib as mpl
mpl.rcParams.update({'font.size': 19,  'figure.figsize': [11, 6.5],  'figure.autolayout': True, 'font.family': 'serif', 'font.sans-serif': ['Helvetica']})

colormap = plt.cm.gist_ncar
########################################################################################################
def init_matriz(p,N):
	return  np.sign(np.random.uniform(-1, 1., (p,N)))


def funcion_auxiliar(patrones,matrix_j, i,j, p, N):
	aux_sum=0
	
	for mu in range(p): 
		aux_sum += patrones[mu][i]*patrones[mu][j]/N	
	
	print("terminado {} {} N={}, p={}".format(i,j, N, p))

	matrix_j[i][j]= aux_sum 
	matrix_j[j][i]= aux_sum

########################################################################################################
def conexion_matrix(patrones, p, N):
	matrix_j=np.zeros((N,N))
	aux_sum=0

	threads= list()

	for i in range(N) :
		for j in range(i+1, N):
			x = th.Thread(target=funcion_auxiliar, args=(patrones, matrix_j, i, j, p, N))
			threads.append(x)
			x.start()

	for x in threads :
		x.join()

	return matrix_j

########################################################################################################
def iterar_determ_dynamic(matrix_j, patron_i_mu, N_epochs, N):
	punto_fijo_i_mu= np.copy(patron_i_mu)

	for _ in range(N_epochs):
		unit = int(np.random.uniform(-0.5, N-.5))
		h_i_mu = np.dot(matrix_j[unit],patron_i_mu)
		punto_fijo_i_mu[unit]= np.sign(h_i_mu)

	return punto_fijo_i_mu

########################################################################################################
def overlap_s_xi(patron_i_mu, punto_fijo_i_mu, N):
	return np.dot(patron_i_mu, punto_fijo_i_mu)/N

########################################################################################################
def evol_hopfield_noiseless(N, alpha, N_epochs, datafile):
	p = int(alpha*N)

	m= np.ones(p)
	
	patrones = init_matriz(p,N)
	matrix_j = conexion_matrix(patrones, p, N)

	outfile = open(datafile, 'w')

	for mu in range(p):	
		punto_fijo_i_mu = iterar_determ_dynamic(matrix_j, patrones[mu], N_epochs, N)
		m[mu] = overlap_s_xi(patrones[mu], punto_fijo_i_mu, N)
		outfile.write("{}\n".format(m[mu]))

	
	print("Terminando N={}, alpha={}".format(N, alpha))
	weights = np.ones_like(m) / len(m)
	plt.hist(m, weights=weights, alpha=0.8, label="N={}, $\\alpha$={}".format(N,alpha))
	plt.legend(loc=0, ncol=2)
	pass

########################################################################################################
def ejer_1():
	N_vec=[500, 1000]# , 2000, 4000]
	alpha_vec = [0.12]#, 0.14, 0.16, 0.18]
	N_epochs=10

	files = []

	for N in N_vec :
		for alpha in alpha_vec: 
			evol_hopfield_noiseless(N, alpha, N_epochs, "test{}{}".format(N, alpha))
			files.append("test{}{}".format(N, alpha))

	plt.figure(2)
	for filename  in files:
		m = np.loadtxt(filename, unpack=True)
		weights = np.ones_like(m) / len(m)
		plt.hist(m, weights=weights, alpha=0.8, label="N={}, $\\alpha$={}".format(N,alpha))
		plt.legend(loc=0, ncol=2)
		pass
	pass


################################################################################################
################################################################################################
################################################################################################

def pr(beta, h_i, signo):
	den = np.exp(signo*beta*h_i)
	num = np.exp(beta*h_i) + np.exp(-beta*h_i)
	return den/num


def evol_hopfield_noise(N, p, T):
	pass

def ejer_2():
	N=4000
	p=40
	t = np.arange(0.1, 2.1, 0.1)

	print(t)

	pass

def main():
	ejer_1()
	plt.show()
	#ejer_2()


if __name__== "__main__":
	main()
