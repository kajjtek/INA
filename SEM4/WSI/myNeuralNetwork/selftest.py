import numpy as np
from sklearn.metrics import confusion_matrix
from sklearn import metrics
import matplotlib.pyplot as plt
import seaborn as sns
from PIL import Image
import os
from networkv2 import networkv2

dir = "./imagesscaled"

net = networkv2.load("./savednetworks/network3.pkl")

images = []

image_files = [f for f in os.listdir(dir) if f.endswith(('jpg', 'jpeg', 'png'))]

for image_file in image_files:
    img_path = os.path.join(dir, image_file)
    img = Image.open(img_path).convert("L")

    img_arr = np.array(img).astype(np.float32)
    img_arr = 255.0-img_arr
    img_arr=img_arr/255.0
    images.append(img_arr)

x =np.array(images)
x = x.reshape(-1, 784,1)

predictions = []
count = 0
for i in range(30):
    prediction = np.argmax(net.feedforward(x[i]))
    predictions.append(prediction)
    print(f"Predykcja dla obrazu {i + 1}: {prediction}")
    
    true_label = i // 3
    
    if prediction == true_label:
        print("Udane rozpoznanie")
        count +=1
    else:
        print("Nieudane rozpoznanie")

print(count)

y_true = [0,0,0,1,1,1,2,2,2,3,3,3,4,4,4,5,5,5,6,6,6,7,7,7,8,8,8,9,9,9]
print(metrics.classification_report(y_pred=predictions, y_true=y_true))

cm = confusion_matrix(y_true, predictions)

# Wizualizacja macierzy pomyłek
plt.figure(figsize=(8, 6))
sns.heatmap(cm, annot=True, fmt='d', cmap="Blues", xticklabels=range(10), yticklabels=range(10))
plt.xlabel('Predykcja')
plt.ylabel('Prawdziwa klasa')
plt.title('Macierz Pomyłek')
plt.show()