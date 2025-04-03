from tensorflow.keras.datasets import mnist
from sklearn.ensemble import RandomForestClassifier
import numpy as np
from sklearn.metrics import confusion_matrix
from sklearn import metrics
import matplotlib.pyplot as plt
import seaborn as sns
from PIL import Image
import os

dir = "../myNeuralNetwork/imagesscaled"


images = []

image_files = [f for f in os.listdir(dir) if f.endswith(('jpg', 'jpeg', 'png'))]

for image_file in image_files:
    img_path = os.path.join(dir, image_file)
    img = Image.open(img_path).convert("L")

    img_arr = np.array(img).astype(np.float32) /255.0
    img_arr = 1 - img_arr
    images.append(img_arr)

x =np.array(images)
x = x.reshape(-1, 784)

print(x.shape)

(x_train, y_train), (x_test, y_test) = mnist.load_data()

x_train, x_test = x_train/255.0, x_test/255.0

x_train = x_train.reshape(-1, 784)
x_test = x_test.reshape(-1, 784)
# def one_hot_encode(y, num_classes=10):
#     return np.eye(num_classes)[y].reshape(-1, num_classes, 1)


# y_train = one_hot_encode(y_train)

# training_data = list(zip(x_train, y_train))
# test_data = list(zip(x_test, y_test))

model = RandomForestClassifier(n_estimators=100, criterion="entropy", random_state=42)
model.fit(X=x_train,y=y_train)

y_test2 = [0,0,0,1,1,1,2,2,2,3,3,3,4,4,4,5,5,5,6,6,6,7,7,7,8,8,8,9,9,9]

y_prediction = model.predict(x_test)

print(metrics.classification_report(y_pred=y_prediction, y_true=y_test))


cm = confusion_matrix(y_test, y_prediction)

plt.figure(figsize=(8, 6))
sns.heatmap(cm, annot=True, fmt='d', cmap="Blues", xticklabels=range(10), yticklabels=range(10))
plt.xlabel('Predykcja')
plt.ylabel('Prawdziwa klasa')
plt.title('Macierz Pomyłek')
plt.show()