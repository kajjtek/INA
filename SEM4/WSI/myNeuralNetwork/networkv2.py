import random
import numpy as np
import pickle


class CrossEntropy:

    @staticmethod
    def fn(a, y):
        return np.sum(np.nan_to_num(-y*np.log(a)-(1-y)*np.log(1-a)))
    
    @staticmethod
    def delta(a, y):
        return (a-y)

class QuadraticCost:

    @staticmethod
    def fn(a, y):
        return 0.5*np.linalg.norm(a-y)**2
    
    @staticmethod
    def delta(z,a,y):
        return (y-a)*sigmoid_derivative(z)

class networkv2:
    def __init__(self,layers, cost=CrossEntropy):
        self.num_layers=len(layers)
        self.layers=layers
        self.starting_variables_configuration()
        self.cost=cost
        print(cost.__name__)

    def starting_variables_configuration(self):
        self.weights = [np.random.randn(y,x)/np.sqrt(x) for y,x in zip(self.layers[1:],self.layers[:-1])]
        self.biases = [np.random.randn(x,1) for x in self.layers[1:]]

    def old_variables_configuration(self):
        self.weights = [np.random.randn(y,x) for y,x in zip(self.layers[1:],self.layers[:-1])]
        self.biases=[np.random.randn(x,1) for x in self.layers[1:]]

    def feedforward(self, a):
        for w,b in zip(self.weights, self.biases):
            a = sigmoid(np.dot(w,a)+b)
        return a
    
    def SGD(self, training_data, epochs, batch_size, eta, lmbda = 0.0, evaluation_data=None,
            monitor_evaluation_cost=False,
            monitor_evaluation_accuracy=False,
            monitor_training_cost=False,
            monitor_training_accuracy=False):
        if evaluation_data: n_test = len(evaluation_data)
        n=len(training_data)
        evaluation_cost, evaluation_accuracy = [], []
        training_cost, training_accuracy = [], []
        for j in range(epochs):
            random.shuffle(training_data)
            batches = [training_data[k:k+batch_size] for k in range(0,n,batch_size)]
            for batch in batches:
                self.update_network(batch, eta, lmbda, len(training_data))
            print("Epoch {0} training complete".format(j))
            if monitor_training_cost:
                cost = self.total_cost(training_data, lmbda)
                training_cost.append(cost)
                print("Cost on training data: {0}".format(cost))
            if monitor_training_accuracy:
                accuracy = self.accuracy(training_data, convert=True)
                training_accuracy.append(accuracy)
                print("Accuracy on training data: {0}/{1}".format(accuracy, n))
            if monitor_evaluation_cost:
                cost = self.total_cost(evaluation_data, lmbda, convert=True)
                evaluation_cost.append(cost)
                print("Cost on evaluation data: {0}".format(cost))
            if monitor_evaluation_accuracy:
                accuracy = self.accuracy(evaluation_data)
                evaluation_accuracy.append(accuracy)
                print("Accuracy on evaluation data: {0}/{1}".format(accuracy, n_test))
        return evaluation_cost, evaluation_accuracy, training_cost, training_accuracy    

    def update_network(self, batch, eta, lmbda, n):
        general_weight_gradient = [np.zeros(w.shape) for w in self.weights]
        general_bias_gradient = [np.zeros(b.shape) for b in self.biases]
        for x,y in batch:
            delta_weight, delta_bias = self.backpropagate(x,y)
            general_weight_gradient = [nw + dnw for nw,dnw in zip(general_weight_gradient,delta_weight)]
            general_bias_gradient = [nb + dnb for nb, dnb in zip(general_bias_gradient, delta_bias)]
        self.weights = [(1-eta*(lmbda/n))*w-(eta/len(batch))*nw for w,nw in zip(self.weights, general_weight_gradient)]
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
        error=(self.cost).delta(activations[-1], y)
        gradient_b[-1]=error
        gradient_w[-1]=np.dot(error, activations[-2].transpose())

        for l in range (2,self.num_layers):
            z = z_inputs[-l]
            error=np.dot(self.weights[-l+1].transpose(),error)*sigmoid_derivative(z)
            gradient_w[-l]=np.dot(error,activations[-l-1].transpose())
            gradient_b[-l]=error
        return (gradient_w,gradient_b)    

    def accuracy(self, data, convert=False):
        if convert:
            results = [(np.argmax(self.feedforward(x)),np.argmax(y)) for x,y in data]
        else:
            results = [(np.argmax(self.feedforward(x)),y) for x,y in data]
        return sum(int(x==y) for x,y in results)

    def total_cost(self, data, lmbda, convert=False):
        cost=0.0
        for x,y in data:
            a=self.feedforward(x)
            if convert: y = one_hot_encode(y)
            cost += self.cost.fn(a,y)/len(data)
        cost += 0.5*(lmbda/len(data))*sum(np.linalg.norm(w)**2 for w in self.weights)
        return cost

    def evaluate(self, test_data):
        test_results = [(np.argmax(self.feedforward(x)),y) for x,y in test_data]
        return sum(int(x==y) for x,y in test_results)

    def save(self, filename):
        data = {
            "layers":self.layers,
            "weights": self.weights,
            "biases": self.biases,
            "cost": self.cost.__name__
        }
        with open(filename, "wb") as f:
            pickle.dump(data,f)

    @staticmethod
    def load(filename):
        with open(filename, "rb") as f:
            data = pickle.load(f)

        cost_class = CrossEntropy if data["cost"] == "CrossEntropy" else QuadraticCost
        net = networkv2(data["layers"], cost=cost_class)
        net.weights = data["weights"]
        net.biases = data["biases"]
        return net

def sigmoid(z):
    return 1.0/(1.0+np.exp(-z))
    
def sigmoid_derivative(z):
    return sigmoid(z)*(1-sigmoid(z))

def one_hot_encode(y, num_classes=10):
    return np.eye(num_classes)[y].reshape(-1, num_classes, 1)
