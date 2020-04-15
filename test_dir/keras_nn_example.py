import numpy as np 
np.random.seed(123)

from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Convolution2D, MaxPooling2D

from keras.layers import np_utils

from keras.datasets import mnist


(X_train, y_train), (X_test, y_test) = mnist.load_data()


from matplotlib import pyplot as plt 
plt.imshow([X_train[0]])


X_train = X_train.reshape(X_train.shape[0], 1, 28, 28)
X_test = X_test.reshape(X_test.shape[0], 1, 28, 28)

X_train = X_train.astype('float32')
X_test = X_test.astype('float32')
X_train /= 255
X_test /= 255


	
print (y_train[:10])2
	
print y_train[:10]