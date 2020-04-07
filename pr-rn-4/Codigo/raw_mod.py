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

def XOR_NN(N_input, N_hidden, N_output, thr, epochs, eta, inputs, expected): 
	error_v= []
	sum_error=0.0

	#Random weights
	hidden_w = np.random.uniform(size=(N_input,N_hidden))
	output_w = np.random.uniform(size=(N_hidden,N_output))	
	
	hidden_bias =np.ones(shape=(1,N_hidden))
	output_bias =np.ones(shape=(1,N_output))
	
	print("hidden weights init: {} \n".format(*hidden_w))
	print("output weights init: {} \n".format(*output_w))
	
	
	#Training algorithm
	for every_epoch in range(epochs):

		#Forward 
		hidden_layer_activation = np.dot(inputs,hidden_w) + hidden_bias # suma el bias
		hidden_layer_output = activation_function_ej1(hidden_layer_activation)


		output_layer_activation = np.dot(hidden_layer_output,output_w) + output_bias
		predicted_output = activation_function_ej1(output_layer_activation)
	
		#Backpropation
		
		error = expected - predicted_output
		
		#Error
		if(every_epoch>0): #Tiro los primeros 20 porque siempre son cualquier cosa
			for e in error:
				sum_error+=error[0]*error[0]*0.5
			sum_error=sum_error/len(error)
			error_v.append(sum_error)
			sum_error=0
	
		#Output NN
		deriv_predicted_output = error * activation_deriv_ej1(predicted_output)
		
		error_hidden_layer = deriv_predicted_output.dot(output_w.T)
		deriv_hidden_layer = error_hidden_layer * activation_deriv_ej1(hidden_layer_output)
	
		#Update Weight
		output_w +=  eta* hidden_layer_output.T.dot(deriv_predicted_output)  #Ese Transpose de mierda, como te pasaste por alto hdp
		hidden_w +=  eta* inputs.T.dot(deriv_hidden_layer) 
		output_bias += eta*np.sum(deriv_predicted_output,axis=0,keepdims=True)
		hidden_bias += eta*np.sum(deriv_hidden_layer,axis=0,keepdims=True)
	
	print("hidden weights end : {} \n".format(*hidden_w))
	print("output weights end : {} \n".format(*output_w))
	
	print("\nSalida de la NN despues de {0} epochs con bias {1}: {2} ".format(epochs, "%1.1f"%thr, *predicted_output))

	label_s= "%.1f" % thr
	plt.plot(error_v, label=label_s)



#******************************************************************************

def ejer_1():
	thr=0.2
	epochs=2000
	eta=0.01

	#Input y output_expected datasets
	inputs = np.array([[-1,-1],[-1,1],[1,-1],[1,1]])
	expected = np.array([[1],[-1],[-1],[1]])
	plt.figure(1)
	plt.ylabel("Error")
	plt.xlabel("Epoch")
	
	for x in range(10):
		XOR_NN(2,2,1,x, epochs, eta, inputs, expected)
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
	plt.ylim(bottom=10E-6)
	
	for x in range(10):
		XOR_NN(2,2,1,thr+ 0.4*x, epochs, eta, inputs, expected)
	pass

	plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left', borderaxespad=0., title="Bias")
	plt.show()

def main():
	ejer_1()
	#ejer_2()

if __name__== "__main__":
	main()