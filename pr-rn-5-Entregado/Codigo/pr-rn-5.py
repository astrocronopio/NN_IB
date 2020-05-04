#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

import matplotlib as mpl
mpl.rcParams.update({'font.size': 19,  'figure.figsize': [9, 6.5],  'figure.autolayout': True, 'font.family': 'serif', 'font.sans-serif': ['Helvetica']})

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
"""
#Crear input de 4D con la distribución de la matriz sigma

def input_ejer_1():

	input_4D= np.zeros(4)
	
	#matriz_sigma =[
	#[2,	1, 1, 1],
	#[1,	2, 1, 1],
	#[1,	1, 2, 1],
	#[1,	1, 1, 2]]
	#
	#matriz_sigma_inv= np.array([
	#[ 4/5., -1/5., -1/5., -1/5.],
	#[-1/5.,  4/5., -1/5., -1/5.],
	#[-1/5., -1/5.,  4/5., -1/5.],
	#[-1/5., -1/5., -1/5.,  4/5.]])
	#print(np.dot(matriz_sigma_inv, matriz_sigma))
	
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

"""
#Ejercicio 2
##########################################################3

def input_ejer_2():
	input_2D = np.zeros(2)

	unit= np.random.uniform(0., 1.)

	r= np.sqrt(unit*(1.21-0.81) + 0.81)

	#r= np.sqrt(np.random.uniform(0.81, 1.21))	

	theta= np.pi*np.random.uniform(0, 1)

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

	w_init=np.array(weights_10D, copy=True) #for plotting
	
	input_2D=input_ejer_2()
	########
	#Initial values print
	f = open('weights.dat', 'w')
	ff= open('inputs.dat', 'w')

	for z in range(10):
		f.write("{}\t{}\n ".format(weights_10D[z][0], weights_10D[z][1] ))
		pass
	f.write("\n\n\n\n\n\n\n")
	########################

	output=np.zeros(10)
	max_position= 0

	for x in range(n_epochs):
		ff.write("{}\t{}\n ".format(input_2D[0], input_2D[1] ))

		for y in range(10):
			output[y] = np.dot(input_2D, weights_10D[y])
		
		max_out = 10 # Resetea quien el ganador cada vez que se actualiza
		
		for n in range(10):
			patron1= np.linalg.norm(weights_10D[n] - input_2D)
			if patron1 < max_out:
				max_position=n
				max_out=patron1

		for z in range(10):
			weights_10D[z][0]= weights_10D[z][0] + eta*neighbour_function(z,max_position,sigma)*(input_2D[0] - weights_10D[z][0])
			weights_10D[z][1]= weights_10D[z][1] + eta*neighbour_function(z,max_position,sigma)*(input_2D[1] - weights_10D[z][1])
			pass
		pass
		input_2D=input_ejer_2()

	for k in range(10):
		f.write("{}\t{}\n ".format(weights_10D[k][0], weights_10D[k][1]))
		pass
	f.write("\n\n\n\n\n\n\n")	
	f.close()

	w_end=np.array(weights_10D, copy=True) 

	return np.reshape(np.transpose(w_init), (2,10)) , np.reshape(np.transpose(w_end), (2,10)) 

def ejer_2():

	plt.gca().set_prop_cycle(plt.cycler('color', plt.cm.gnuplot(np.linspace(0, 1, 10))))

	vec= plt.cm.gnuplot(np.linspace(0, 1, 8))

	n_epochs1=100000 
	n_epochs2=10000 
	n_epochs3=2000


	sigma1=0.3  #0.5 es el optimo

	eta= 0.02
	graph_name= "../Graficos/sigma{}eta{}".format("0_3","0_02")
	title_plot="$\\sigma={}$, $\\eta={}$".format(sigma1, eta)

	w_init1, w_end1= ejer_2_evolucion(n_epochs1, sigma1, eta)
	w_init3, w_end3= ejer_2_evolucion(n_epochs3, sigma1, eta)

	ang = np.arange(0, np.pi, 0.01)

	x=np.cos(ang)
	y=np.sin(ang)

	plt.xlabel("x")
	plt.ylabel("y")
	
	plt.title(title_plot)
	plt.scatter(w_init1[0], w_init1[1],  color='black', alpha=0.5, label="Inicial")
	plt.plot(w_init1[0], w_init1[1],  color='black', alpha=0.5)
	
	plt.scatter(w_end1[0], w_end1[1], marker='*', s=50, alpha=0.8 , color= vec[1], label="N={}".format(n_epochs1))
	plt.plot(w_end1[0], w_end1[1], color= vec[1])

	#plt.scatter(w_end2[0], w_end2[1], marker='^', s=50, alpha=0.8 , color= vec[2], label="N={}".format(n_epochs2))
	#plt.plot(w_end2[0], w_end2[1], color= vec[2])

	plt.scatter(w_end3[0], w_end3[1], marker='s', s=50, alpha=0.8 , color= vec[4], label="N={}".format(n_epochs3))
	plt.plot(w_end3[0], w_end3[1], color= vec[4])



	plt.plot(1.1*x, 1.1*y, color='brown', alpha=0.5)
	plt.plot(0.9*x, 0.9*y, color='brown', alpha=0.5)
	plt.axis('equal')
	plt.legend(loc=0, title="Pesos")
	plt.savefig(graph_name)
	plt.show()
	pass


def main():
	#ejer_1()
	ejer_2()



if __name__== "__main__":
	main()
