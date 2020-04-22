from keras.models import Sequential
from keras.layers.core import Dense, Dropout, Activation
from keras.optimizers import SGD
import numpy as np 

X = np.array([[1,1],[1,-1],	[-1,1],	[-1,-1]])
y = np.array([[1],	[-1],	[-1],	[1]])

model = Sequential()
model.add(Dense(2, input_dim=2))
model.add(Activation('tanh'))
model.add(Dense(1))
model.add(Activation('tanh'))

sgd = SGD(lr=0.1)
model.compile(loss='mean_squared_error', optimizer=sgd)

model.fit(X, y, batch_size=1, epochs=1000)
print(model.predict_proba(X))
"""
[[ 0.0033028 ]
 [ 0.99581173]
 [ 0.99530098]
 [ 0.00564186]]
"""