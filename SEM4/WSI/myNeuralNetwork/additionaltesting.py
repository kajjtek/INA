import tensorflow as tf
from tensorflow.keras.datasets import mnist
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Flatten, Dropout
from tensorflow.keras.utils import to_categorical
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
    image = Image.open(img_path).convert('L')  # Convert to grayscale
    image = image.resize((28, 28))              # Resize to MNIST format
                    
                    # Convert to numpy array and normalize
    img_array = np.array(image)
                    
                    # Invert colors if needed (MNIST has white digits on black)
    if np.mean(img_array) > 127:
        img_array = 255 - img_array
                        
    img_array = img_array / 255.0
                    
                    # Add to dataset
    images.append(img_array.reshape(28, 28, 1))

x =np.array(images)

# (x_train, y_train), (x_test, y_test) = mnist.load_data()


# x_train = x_train / 255.0
# x_test = x_test / 255.0


# y_train = to_categorical(y_train, 10)
# y_test = to_categorical(y_test, 10)


# model = Sequential([
#     Flatten(input_shape=(28, 28)),     
#     Dense(128, activation='relu'), 
#     Dropout(0.2),
#     Dense(10, activation='softmax')        
# ])


# model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])


# model.fit(x_train, y_train, epochs=10, validation_data=(x_test, y_test))

# Simple CNN for the MNIST Dataset
import tensorflow as tf
from tensorflow.keras import layers, models
import numpy as np
from sklearn.metrics import classification_report, precision_score, recall_score, confusion_matrix

tf.experimental.numpy.experimental_enable_numpy_behavior()

# learn images, learn lables, test ...
(li, ll), (ti, tl) = tf.keras.datasets.mnist.load_data()

print(tf.shape(li))

li = tf.reshape(li, (60000, 28, 28, 1)).astype('float32') / 255
ti = tf.reshape(ti, (10000, 28, 28, 1)).astype('float32') / 255

model = models.Sequential()

model.add(layers.Input(shape = (28, 28, 1)))

model.add(layers.Conv2D(64, kernel_size=(3, 3), activation='relu', padding='same'))
model.add(layers.MaxPooling2D(pool_size=(2, 2)))

model.add(layers.Conv2D(128, kernel_size=(3, 3), activation='relu', padding='same'))
model.add(layers.MaxPooling2D(pool_size=(2, 2)))

model.add(layers.Flatten())

model.add(layers.Dense(128, activation='relu'))
model.add(layers.Dropout(0.3))

model.add(layers.Dense(64, activation='relu'))
model.add(layers.Dense(10, activation='softmax'))

model.compile(optimizer=tf.keras.optimizers.AdamW(learning_rate = 0.001),
              loss='sparse_categorical_crossentropy',
              metrics=['accuracy'])

early_end = tf.keras.callbacks.EarlyStopping(
        monitor = 'val_accuracy',
        patience = 3,
        restore_best_weights = True
)

model.fit(li, ll, 
          epochs=10, 
          batch_size=64, 
          validation_split=0.2,
          callbacks = [early_end])

model.save("modeladd.keras")


y_test2 = [0,0,0,1,1,1,2,2,2,3,3,3,4,4,4,5,5,5,6,6,6,7,7,7,8,8,8,9,9,9]
y_test2 = np.array(y_test2)
y_prediction = model.predict(x)
y_pred_labels = np.argmax(y_prediction, axis=1)


cm = confusion_matrix(y_test2, y_pred_labels)

plt.figure(figsize=(8, 6))
sns.heatmap(cm, annot=True, fmt='d', cmap="Blues", xticklabels=range(10), yticklabels=range(10))
plt.xlabel('Predykcja')
plt.ylabel('Prawdziwa klasa')
plt.title('Macierz Pomyłek')
plt.show()