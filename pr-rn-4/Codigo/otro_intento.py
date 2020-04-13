import matplotlib.pyplot as plt
import numpy as np

import matplotlib as mpl
mpl.rcParams.update({'font.size': 18,  'figure.figsize': [8, 6],  'figure.autolayout': True})

colormap = plt.cm.gist_ncar
plt.gca().set_prop_cycle(plt.cycler('color', plt.cm.jet(np.linspace(0, 1, 10))))


def activation_function_ej1 (x):
    return np.tanh(x)

def activation_deriv_ej1(x):
    return 1 - np.tanh(x)*np.tanh(x)


#############################################################################
# N_input: Numero de neuronas de input
# N_input: Numero de neuronas en LA UNICA hidden layer
# N_output: Numero de neuronas en la salida

def XOR_NN(N_input, N_hidden, N_output, epochs, eta, inputs, expected): 
	error_v= []
	sum_error=0.0
	

	#Random weights
	hidden_w = np.random.uniform(size=(N_input,N_hidden))
	output_w = np.random.uniform(size=(N_hidden,N_output))	
	
	hidden_bias =np.random.uniform(size=(1,N_hidden))
	output_bias =np.random.uniform(size=(1,N_output))
	
	print("\nhidden weights init: ")
	print(*hidden_w)
	print("\noutput weights init: ")
	print(*output_w)
	
	
	#Training algorithm
	for every_epoch in range(epochs):
		predicted_output=[]
		for index in range(len(inputs)):
			#Forward 
			hidden_layer_activation = np.dot(inputs[index],hidden_w) + hidden_bias # suma el bias
			hidden_layer_output = activation_function_ej1(hidden_layer_activation)
	
			output_layer_activation = np.dot(hidden_layer_output,output_w) + output_bias
			predicted_output.append(activation_function_ej1(output_layer_activation))
		
			#Backpropation
			
			error = expected[index] - predicted_output[index] #ultima capa
			
			#Error
			if(every_epoch>0): #Tiro los primeros 20 porque siempre son cualquier cosa
				for e in error:
					sum_error+=error[0]*error[0]*0.5
				error_v.append(sum_error)
				sum_error=0
		
			#Output NN
			deriv_predicted_output = error * activation_deriv_ej1(predicted_output[index])
			
			error_hidden_layer = deriv_predicted_output.dot(output_w.T)
			deriv_hidden_layer = error_hidden_layer * activation_deriv_ej1(hidden_layer_output)
		
			#Update Weight
			output_w +=  eta* hidden_layer_output.T.dot(deriv_predicted_output)  #Ese Transpose de mierda, como te pasaste por alto hdp
			hidden_w +=  eta* np.dot(inputs[index],deriv_hidden_layer.T) 
			
			output_bias += eta*np.sum(deriv_predicted_output,axis=0,keepdims=True)
			hidden_bias += eta*np.sum(deriv_hidden_layer,axis=0,keepdims=True)
	
	print("\nhidden weights end: ")
	print(*hidden_w)
	print("\noutput weights end: ")
	print(*output_w)
	
	print("\nSalida de la NN despues de {0} epochs: {1} ".format(epochs, predicted_output))

	plt.plot(error_v)



#******************************************************************************

def ejer_1():
	thr=0.2
	epochs=1000
	eta=0.01

	#Input y output_expected datasets
	inputs = np.array([[-1,-1],[-1,1],[1,-1],[1,1]])
	expected = np.array([[1],[-1],[-1],[1]])
	plt.figure(1)
	plt.ylabel("Error")
	plt.xlabel("Epoch")
	
	for x in range(2):
		XOR_NN(2,2,1, epochs, eta, inputs, expected)
	pass

	#plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left', borderaxespad=0.)
	plt.show()


def ejer_2():
	thr=0.2
	epochs=500
	eta=0.05

	#Input y output_expected datasets
	inputs = np.array([[-1,-1],[-1,1],[1,-1],[1,1]])
	expected = np.array([[1],[-1],[-1],[1]])
	plt.figure(1)
	plt.ylabel("Error")
	plt.xlabel("Epoch")
	#plt.ylim(bottom=10E-6)
	
	for x in range(10):
		XOR_NN(2,2,1,thr+ 0.4*x, epochs, eta, inputs, expected)
	pass

	plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left', borderaxespad=0., title="Bias")
	plt.show()

def main():
	ejer_1()


if __name__== "__main__":
	main()