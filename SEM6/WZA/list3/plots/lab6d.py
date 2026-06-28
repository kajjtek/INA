"""
Lab 6d — Spirala Pascala / Kardioda
(x^2+y^2-2x)^2 = z^2*(x^2+y^2)  z plaszczyznacx+2y+3z=0
Eliminacja x => krzywa w (y,z)
Plus: pierwsze rownanie z=a=2 => kardioda
"""
import numpy as np
import matplotlib.pyplot as plt

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(13, 6))

# --- Wykres 1: uklad ze wspolnego rownania po eliminacji x ---
# Z x + 2y + 3z = 0  => x = -2y - 3z
# Podstawiamy do (x^2+y^2-2x)^2 = z^2*(x^2+y^2)
y = np.linspace(-6, 6, 1000)
z_vals = np.linspace(-6, 6, 1000)
Y2, Z2 = np.meshgrid(y, z_vals)
X2 = -2*Y2 - 3*Z2
F = (X2**2 + Y2**2 - 2*X2)**2 - Z2**2*(X2**2 + Y2**2)
ax1.contour(Y2, Z2, F, levels=[0], colors='royalblue')
ax1.set_title("Uklad po eliminacji x\n(x=-2y-3z, krzywa w y,z)")
ax1.set_xlabel('y'); ax1.set_ylabel('z')
ax1.axhline(0, color='k', lw=0.5); ax1.axvline(0, color='k', lw=0.5)
ax1.grid(True, alpha=0.3)

# --- Wykres 2: samo pierwsze rownanie z z=a=2 ---
# (x^2+y^2-2x)^2 = 4*(x^2+y^2)
# To jest rownoanie kardiody (w kartezjanskich, po przesunieciu)
x = np.linspace(-4, 6, 800)
y = np.linspace(-5, 5, 800)
X3, Y3 = np.meshgrid(x, y)
G = (X3**2 + Y3**2 - 2*X3)**2 - 4*(X3**2 + Y3**2)
ax2.contour(X3, Y3, G, levels=[0], colors='crimson')
ax2.set_title("Pierwsze rownanie z z=a=2\n(x^2+y^2-2x)^2 = 4(x^2+y^2) — kardioda")
ax2.set_xlabel('x'); ax2.set_ylabel('y')
ax2.axhline(0, color='k', lw=0.5); ax2.axvline(0, color='k', lw=0.5)
ax2.grid(True, alpha=0.3)

# Dodaj biegunowa wersje kardiody dla potwierdzenia
theta = np.linspace(0, 2*np.pi, 1000)
r = 2*(1 + np.cos(theta))          # kardioda r=a(1+cos t) z a=2 (skalowanie)
# Rownoanie (x^2+y^2-2x)^2=4(x^2+y^2) w biegunowym to r^2=2r*cos+2r => r=2cos+2=2(1+cos)
xp = r * np.cos(theta)
yp = r * np.sin(theta)
ax2.plot(xp, yp, 'b--', lw=1, label='r=2(1+cos θ) — biegunowo')
ax2.legend(fontsize=8)

plt.suptitle("Lab 6d: Spirala Pascala — Kajetan Plewa 282212", fontsize=12)
plt.tight_layout()
plt.savefig("lab6d_plots.pdf", bbox_inches='tight')
plt.savefig("lab6d_plots.png", dpi=150, bbox_inches='tight')
print("Zapisano: lab6d_plots.pdf / lab6d_plots.png")
plt.show()
