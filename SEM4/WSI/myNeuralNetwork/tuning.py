import optuna
from Network import Network
import numpy as np
from tensorflow.keras.datasets import mnist

(x_train, y_train), (x_test, y_test) = mnist.load_data()

x_train, x_test = x_train/255.0, x_test/255.0

x_train = x_train.reshape(-1, 784, 1)
x_test = x_test.reshape(-1, 784, 1)
def one_hot_encode(y, num_classes=10):
    return np.eye(num_classes)[y].reshape(-1, num_classes, 1)


y_train = one_hot_encode(y_train)

training_data = list(zip(x_train, y_train))
test_data = list(zip(x_test, y_test))


def objective(trial):
    # Sample architecture: number of hidden layers and neurons per layer.
    n_hidden_layers = trial.suggest_int("n_hidden_layers", 1, 3)
    hidden_layer_sizes = []
    for i in range(n_hidden_layers):
        neurons = trial.suggest_int(f"n_neurons_layer_{i}", 50, 100)
        hidden_layer_sizes.append(neurons)
    
    # Define the network layers. Assuming input size 784 (e.g., MNIST) and 10 output classes.
    layers = [784] + hidden_layer_sizes + [10]
    
    # Sample other hyperparameters.
    eta = trial.suggest_loguniform('eta', 1, 4)
    epochs = trial.suggest_int('epochs', 30, 50)
    batch_size = trial.suggest_categorical('batch_size', [10,16])
    
    # Create a new network instance with the sampled architecture.
    net = Network(layers)
    
    # Train the network.
    # Make sure training_data and test_data are defined in your environment.
    net.SGD(training_data, epochs, batch_size, eta)
    
    # Evaluate performance and return the accuracy (higher is better).
    accuracy = net.evaluate(test_data) / len(test_data)
    return accuracy

# Create a study and specify that we want to maximize accuracy.
study = optuna.create_study(direction='maximize')
study.optimize(objective, n_trials=50)
