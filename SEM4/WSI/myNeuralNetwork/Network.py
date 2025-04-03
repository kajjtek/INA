import random
import numpy as np

class Network:
    def __init__(self,layers):
        self.num_layers=len(layers)
        self.layers=layers
        self.biases=[np.random.randn(x,1) for x in layers[1:]]
        self.weights=[np.random.randn(y,x) for x,y in zip(layers[:-1],layers[1:])]

    def feedforward(self, a):
        for w,b in zip(self.weights, self.biases):
            a = sigmoid(np.dot(w,a)+b)
        return a
    
    def SGD(self, training_data, epochs, batch_size, eta, test_data=None):
        if test_data: n_test = len(test_data)
        n=len(training_data)
        for j in range(epochs):
            random.shuffle(training_data)
            batches = [training_data[k:k+batch_size] for k in range(0,n,batch_size)]
            for batch in batches:
                self.update_network(batch, eta)
            if test_data:
                print("Epoch {0}: {1} / {2}".format(j, self.evaluate(test_data), n_test))
            else:
                print("Epoch {0} complete".format(j))

    def update_network(self, batch, eta):
        general_weight_gradient = [np.zeros(w.shape) for w in self.weights]
        general_bias_gradient = [np.zeros(b.shape) for b in self.biases]
        for x,y in batch:
            delta_weight, delta_bias = self.backpropagate(x,y)
            general_weight_gradient = [nw + dnw for nw,dnw in zip(general_weight_gradient,delta_weight)]
            general_bias_gradient = [nb + dnb for nb, dnb in zip(general_bias_gradient, delta_bias)]
        self.weights = [w-(eta/len(batch))*nw for w,nw in zip(self.weights, general_weight_gradient)]
        self.biases = [b-(eta/len(batch))*nb for b,nb in zip(self.biases, general_bias_gradient)]

    def backpropagate(self, x , y):
        gradient_w = [np.zeros(w.shape) for w in self.weights]
        gradient_b = [np.zeros(b.shape) for b in self.biases]

        activation = x
        activations = [x]
        z_inputs = []
        for w,b in zip(self.weights, self.biases):
            z = np.dot(w,activation)+b
            z_inputs.append(z)
            activation = sigmoid(z)
            activations.append(activation)
        error=(activations[-1]-y) * sigmoid_derivative(z_inputs[-1])
        gradient_b[-1]=error
        gradient_w[-1]=np.dot(error, activations[-2].transpose())

        for l in range (2,self.num_layers):
            z = z_inputs[-l]
            error=np.dot(self.weights[-l+1].transpose(),error)*sigmoid_derivative(z)
            gradient_w[-l]=np.dot(error,activations[-l-1].transpose())
            gradient_b[-l]=error
        return (gradient_w,gradient_b)    


    def evaluate(self, test_data):
        test_results = [(np.argmax(self.feedforward(x)),y) for x,y in test_data]
        return sum(int(x==y) for x,y in test_results)

def sigmoid(z):
    return 1.0/(1.0+np.exp(-z))
    
def sigmoid_derivative(z):
    return sigmoid(z)*(1-sigmoid(z))
