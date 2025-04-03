import numpy as np
from tensorflow.keras.datasets import mnist
from Network import Network
from networkv2 import networkv2
import os
from PIL import Image

(x_train, y_train), (x_test, y_test) = mnist.load_data()

x_train, x_test = x_train/255.0, x_test/255.0

x_train = x_train.reshape(-1, 784, 1)
x_test = x_test.reshape(-1, 784, 1)
def one_hot_encode(y, num_classes=10):
    return np.eye(num_classes)[y].reshape(-1, num_classes, 1)


y_train = one_hot_encode(y_train)

training_data = list(zip(x_train, y_train))
test_data = list(zip(x_test, y_test))

layers = [784,100,10]

net = networkv2(layers)

evaluation_cost, evaluation_accuracy, training_cost, training_accuracy = net.SGD(training_data, 20, 10, 0.1, lmbda= 5.0, evaluation_data=test_data, monitor_evaluation_accuracy=True,
monitor_evaluation_cost=True,
monitor_training_accuracy=True,
monitor_training_cost=True)

np.save("ev_cost.npy", evaluation_cost)
np.save("ev_acc.npy", evaluation_accuracy)
np.save("tr_cost.npy", training_cost)
np.save("tr_acc.npy", training_accuracy)

net.save("network3.pkl")
