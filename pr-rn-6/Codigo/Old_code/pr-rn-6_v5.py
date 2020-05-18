#!/usr/bin/env python
# -*- coding: utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt

import matplotlib as mpl
mpl.rcParams.update({'font.size': 19,  'figure.figsize': [11, 6.5],  'figure.autolayout': True, 'font.family': 'serif', 'font.sans-serif': ['Helvetica']})

colormap = plt.cm.gist_ncar
########################################################################################################

def init_matriz(p,N): return  np.sign(np.random.uniform(-2, 2., (p,N)))

########################################################################################################

def conexion_matrix(patrones, p, N):
	matrix_j=np.zeros((N,N))

	for i in range(N) : 
		for j in range(i+1,N):
			aux_sum=0
			for mu in range(p): aux_sum += patrones[mu][i]*patrones[mu][j]/N
			matrix_j[i][j]= aux_sum
			matrix_j[j][i]= aux_sum

	return matrix_j

########################################################################################################

def iterar_determ_dynamic(matrix_j, patron_i_mu, N_epochs, N):
	punto_fijo_i_mu= np.copy(patron_i_mu)
	flag=0

	# for unit in range(N):
	# 	for t in range(N_epochs):
	# 		h_i_mu=0
	# 		for mu in range(N): h_i_mu += matrix_j[unit][mu]*punto_fijo_i_mu[mu]
	# 		punto_fijo_i_mu[unit] = np.sign(h_i_mu)
			
	while flag<N:
		flag=0
		for unit in range(N):
			h_i_mu=0
		
			for mu in range(N): h_i_mu += matrix_j[unit][mu]*punto_fijo_i_mu[mu]

			if punto_fijo_i_mu[unit]*np.sign(h_i_mu)<0: 
				print("{}\t{}\tFlag={}\n".format(punto_fijo_i_mu[unit], np.sign(h_i_mu), flag))
				punto_fijo_i_mu[unit] = np.sign(h_i_mu)
			else: 	flag+=1
	return punto_fijo_i_mu

########################################################################################################

def overlap_s_xi(patron_i_mu, punto_fijo_i_mu, N): return np.dot(patron_i_mu, punto_fijo_i_mu)/N

########################################################################################################
def evol_hopfield_noiseless(N, alpha, N_epochs, datafile):
	p = int(alpha*N)
	m= np.zeros(p)
	
	patrones = init_matriz(p,N)
	matrix_j = conexion_matrix(patrones, p, N)

	outfile = open(datafile, 'w')

	for mu in range(p):	
		punto_fijo_i_mu = iterar_determ_dynamic(matrix_j, patrones[mu], N_epochs, N)
		m[mu] = overlap_s_xi(patrones[mu], punto_fijo_i_mu, N)
		print("overlap {}: {}\n".format(mu, m[mu]))
		outfile.write("{}\n".format(m[mu]))

	
	print("Terminando N={}, alpha={}".format(N, alpha))
	weights = np.ones_like(m)/len(m)
	plt.hist(m, weights=weights, alpha=0.8, label="N={}, $\\alpha$={}".format(N,alpha))
	plt.legend(loc=0, ncol=2)

########################################################################################################
def ejer_1():
	N_vec=[500]#, 1000]# , 2000, 4000]
	alpha_vec = [0.12]#, 0.14, 0.16, 0.18]
	N_epochs=100

	files = []

	for N in N_vec: 
		for alpha in alpha_vec: 
			evol_hopfield_noiseless( N, alpha, N_epochs, "test{}_{}".format(N, alpha))
			files.append("test{}_{}".format(N, alpha))

	plt.figure(2)
	for filename  in files:
		m = np.loadtxt(filename, unpack=True)
		weights = np.ones_like(m) / len(m)
		plt.hist(m, weights=weights, alpha=0.8, label=filename)
		plt.legend(loc=0, ncol=2)



################################################################################################
################################################################################################
################################################################################################


def main():
	ejer_1()
	plt.show()


if __name__== "__main__":
	main()
