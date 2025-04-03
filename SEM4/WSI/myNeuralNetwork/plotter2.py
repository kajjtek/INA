import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.metrics import confusion_matrix
from sklearn import metrics
import numpy as np
from networkv2 import networkv2
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
print(training_data[2][0])

net = networkv2.load("./savednetworks/network2.pkl")


y_pred = [np.argmax(net.feedforward(x)) for x, _ in test_data]  # Predykcje jako indeksy klas
y_true = [y for _, y in test_data]

print(metrics.classification_report(y_pred=y_pred, y_true=y_true))

cm = confusion_matrix(y_true, y_pred)

# Wizualizacja macierzy pomyłek
plt.figure(figsize=(8, 6))
sns.heatmap(cm, annot=True, fmt='d', cmap="Blues", xticklabels=range(10), yticklabels=range(10))
plt.xlabel('Predykcja')
plt.ylabel('Prawdziwa klasa')
plt.title('Macierz Pomyłek')
plt.show()