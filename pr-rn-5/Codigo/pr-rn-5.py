#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

import matplotlib as mpl
mpl.rcParams.update({'font.size': 19,  'figure.figsize': [9, 6],  'figure.autolayout': True, 'font.family': 'serif', 'font.sans-serif': ['Helvetica']})

colormap = plt.cm.gist_ncar

from matplotlib.patches import FancyArrowPatch
from mpl_toolkits.mplot3d import proj3d

class Arrow3D(FancyArrowPatch):
    def __init__(self, xs, ys, zs, *args, **kwargs):
        FancyArrowPatch.__init__(self, (0,0), (0,0), *args, **kwargs)
        self._verts3d = xs, ys, zs

    def draw(self, renderer):
        xs3d, ys3d, zs3d = self._verts3d
        xs, ys, zs = proj3d.proj_transform(xs3d, ys3d, zs3d, renderer.M)
        self.set_positions((xs[0],ys[0]),(xs[1],ys[1]))
        FancyArrowPatch.draw(self, renderer)


#Ejercicio 1
##########################################################3

#Crear input de 4D con la distribución de la matriz sigma

def input_ejer_1():

	input_4D= np.zeros(4)
	"""
	matriz_sigma =[
	[2,	1, 1, 1],
	[1,	2, 1, 1],
	[1,	1, 2, 1],
	[1,	1, 1, 2]]

	matriz_sigma_inv= np.array([
	[ 4/5., -1/5., -1/5., -1/5.],
	[-1/5.,  4/5., -1/5., -1/5.],
	[-1/5., -1/5.,  4/5., -1/5.],
	[-1/5., -1/5., -1/5.,  4/5.]])

	#print(np.dot(matriz_sigma_inv, matriz_sigma))
	"""
	#La magia de hacer que tenga la distribucíón que quiera
	matriz_eigval_square=[5, 1., 1., 1.]

	eigvec_1=[ 0.5, 0.5 , 0.5, 0.5]
	eigvec_2=[1, 0 , 0, -1]/np.sqrt(2)
	eigvec_3=[1, 0 , -1, 0]/np.sqrt(2)
	eigvec_4=[1, -1 , 0, 0]/np.sqrt(2)

	matriz_U=np.matrix.transpose(np.array([eigvec_1, eigvec_2, eigvec_3, eigvec_4]))

	input_4D=np.random.normal([0,0,0,0], matriz_eigval_square)

	input_4D= np.dot(matriz_U, input_4D)
	
	return input_4D

#Hacer la función que acctualice los pesos

def delta_weight(eta, output, input_i, w_i):
	return eta*output*(input_i - output*w_i )
	
def output_function(pesos, entradas):
	return np.dot(pesos, entradas)

#La función donde está el loop

def ejer_1_evolucion(n_epochs):

	weights_4D=np.random.uniform(0, 0.05, 4)
	input_4D=input_ejer_1()

	#n_epochs=5000
	
	eta=0.001
	f = open('weights_2.dat', 'w')
	for x in range(n_epochs):
		input_4D=input_ejer_1()
		output=output_function(weights_4D, input_4D)

		for y in range(4):
			weights_4D[y] = weights_4D[y] + delta_weight(eta, output, input_4D[y], weights_4D[y])
			f.write("{}\t".format(weights_4D[y]))
		f.write("\n")

	f.close()
	pass

def ejer_1():
	ejer_1_evolucion(1000)

	w1, w2, w3, w4 = np.loadtxt('weights_2.dat', unpack=True)
	fig = plt.figure(3)
	
	fig.gca().set_prop_cycle(plt.cycler('color', plt.cm.gnuplot(np.linspace(0, 1, 10))))
	
	ax = fig.add_subplot(111, projection='3d')
	
	ax.set_xlabel("$w_1$", labelpad=15)
	ax.set_ylabel("$w_2$", labelpad=15)
	ax.set_zlabel("$w_3$", labelpad=15)

	#ax.quiver(0,0,0,0.3, 0.3, 0.3, edgecolor='pink')

	a = Arrow3D([0.0, 0.5], [0., 0.5],[0., 0.5], mutation_scale=25, lw=1, arrowstyle="-|>", color="red")
	ax.add_artist(a)
	
	ax.scatter(w3,w2,w4, color='black', s=.2)
	ax.plot(w3,w2,zdir='z', alpha=0.5)
	ax.plot(w4,w3,zdir='x', alpha=0.5)

	plt.gca().set_prop_cycle(plt.cycler('color', plt.cm.jet(np.linspace(0, 1, 10))))

	fig.savefig("../Graficos/todos_los_pesos_3D.png")


	plt.figure(1)
	plt.ylabel("Peso")
	plt.xlabel(u"Épocas")
	
	plt.plot(w3, label="1")
	plt.plot(w2, label="2")
	plt.plot(w4, label="3")
	plt.plot(w1, label="4")
	plt.legend(loc=0, title="Pesos")

	plt.savefig("../Graficos/todos_los_pesos.png")

	#plt.plot(w1[-1]*np.ones(len(w1)))
	#plt.plot(w2[-1]*np.ones(len(w1)))
	plt.legend(loc=0, title="Pesos")
	plt.figure(2)
	
	plt.ylabel("$w_3$")
	plt.xlabel("$w_2$")
	
	plt.plot(w4, w2)
	
	plt.savefig("../Graficos/pesos_3_2.png")

	plt.show()


#Ejercicio 2
##########################################################3

def input_ejer_2():
	input_2D = np.zeros(2)
	r= np.random.uniform(0.9, 1.1)
	theta= np.random.uniform(0, np.pi )

	input_2D[0] = r*np.cos(theta)
	input_2D[1] = r*np.sin(theta)

	return input_2D

def neighbour_function(i, i_max, sigma):
	return  np.exp(-(i-i_max)*(i-i_max)*0.5/(sigma*sigma))/(2.*np.pi*np.sqrt(sigma))

def ejer_2_evolucion(n_epochs, sigma, eta):
	##Init##
	weights_10D= np.reshape(np.zeros(20), (2,10))
	weights_10D[1]= 0.5*np.ones(10)
	weights_10D[0]= np.arange(-0.9, 1.1, 0.2)
	weights_10D = np.reshape(np.transpose(weights_10D), (10,2))

	input_2D=input_ejer_2()
	########
	#Initial values print
	f = open('weights_2.dat', 'w')
	ff= open('outputs.dat', 'w')

	for z in range(10):
		f.write("{}\t{}\n ".format(weights_10D[z][0], weights_10D[z][1] ))
		pass
	f.write("\n\n\n\n\n\n\n")
	########################


	output=np.zeros(10)

	max_position_1= 0
	max_position= 0

	
	for x in range(n_epochs):
		ff.write("{}\t{}\n ".format(input_2D[0], input_2D[1] ))

		for y in range(10):
			output[y] = np.dot(input_2D, weights_10D[y])
		
		max_out = np.linalg.norm(weights_10D[4] - input_2D)
		for n in range(10):
			patron1= np.linalg.norm(weights_10D[n] - input_2D)
			if patron1 < max_out:
				max_position=n
				max_out=patron1
		pass

		for z in range(10):
			weights_10D[z][0]= weights_10D[z][0] + eta*neighbour_function(z,max_position,sigma)*(input_2D[0] - weights_10D[z][0])
			weights_10D[z][1]= weights_10D[z][1] + eta*neighbour_function(z,max_position,sigma)*(input_2D[1] - weights_10D[z][1])
			pass
		pass
		input_2D=input_ejer_2()

	for k in range(10):
		f.write("{}\t{}\n ".format(weights_10D[k][0], weights_10D[k][1] ))
		pass
	f.write("\n\n\n\n\n\n\n")	
	f.close()
	pass

def ejer_2():
	
	ejer_2_evolucion(2000000, 1, 0.1)

	init_output=[0.0152030416538, -0.00884214837752, 0.0984331365616, 0.0710199881398, 0.0380696032158, 0.0975260269559, 0.038536549101, 0.0242109649504, 0.012897654721, 0.0533665983356]
	fin_output=[0.0171729266256, -0.00986066253998, 0.569925390034, 0.0792224229837, 0.0419902274485, 0.108876534646, 0.0425649694269, 0.0272354389493, 0.0144095231746, 0.0594173667181]


	plt.plot(init_output)
	plt.plot(fin_output)
	#plt.show()
	pass
	

#main

def main():
	#ejer_1()
	ejer_2()



if __name__== "__main__":
	main()
