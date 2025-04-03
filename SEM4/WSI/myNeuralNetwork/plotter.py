import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.metrics import confusion_matrix
import numpy as np

ev_cost = np.load("./data/ev_cost.npy")
ev_acc = np.load("./data/ev_acc.npy")
tr_cost = np.load("./data/tr_cost.npy")
tr_acc = np.load("./data/tr_acc.npy")
tr_acc = tr_acc / 60000
ev_acc = ev_acc / 10000

print("Best accuracy for training: ", np.max(tr_acc))

epochs = range(1, len(ev_cost) + 1)
print("Validation accuracy (ev_acc):", ev_acc)


plt.figure(figsize=(12, 5))

# Wykres kosztu (loss) dla treningu i testu
plt.subplot(1, 2, 1)
plt.plot(epochs, ev_cost, label='Validation cost')
plt.plot(epochs, tr_cost, label='Training cost')
plt.xlabel('Epoch')
plt.ylabel('Cost')
plt.title('Cost vs epochs')
plt.legend()

# Wykres dokładności (accuracy) dla treningu i testu
plt.subplot(1, 2, 2)
plt.plot(epochs, ev_acc, label='Validation accuracy')
plt.plot(epochs, tr_acc, label='Training accuracy')
plt.xlabel('Epoch')
plt.ylabel('Accuracy')
plt.title('Accuracy vs epochs')
plt.legend()

plt.tight_layout()
plt.show()
