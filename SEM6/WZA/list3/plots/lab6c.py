"""
Lab 6c — wykresy V(I_f) po eliminacji z z ukladu x^2+y^2=z^2, f(x,y,z)=0
Indeks 282212: a=2, b=8, c=2, d=2, e=1, f=2
"""
import numpy as np
import matplotlib.pyplot as plt

x = np.linspace(-10, 10, 800)
y = np.linspace(-10, 10, 800)
X, Y = np.meshgrid(x, y)

fig, axes = plt.subplots(2, 3, figsize=(15, 10))
axes = axes.flatten()

cases = [
    # (tytuł, wyrażenie F(x,y) = 0, opis geometryczny)
    # f1 = 3z => z=0 => x^2+y^2=0 => punkt (0,0)
    ("f=(c+1)z=3z\nz=0 => x^2+y^2=0", X**2 + Y**2,
     "Jeden punkt: (0,0)"),

    # f2 = z+d+1=z+3 => z=-3 => x^2+y^2=9 => okrag r=3
    ("f=z+d+1=z+3\nz=-3 => x^2+y^2=9", X**2 + Y**2 - 9,
     "Okrag: x^2+y^2=9, r=3"),

    # f3 = x+z-e-1=x+z-2 => z=2-x => x^2+y^2=(2-x)^2
    # x^2+y^2 = 4-4x+x^2 => y^2=4-4x => y^2+4x-4=0
    ("f=x+z-e-1=x+z-2\ny^2=4-4x (parabola)", Y**2 + 4*X - 4,
     "Parabola: y^2 = 4(1-x)"),

    # f4 = x+y+z+f+1=x+y+z+3 => z=-x-y-3
    # x^2+y^2=(x+y+3)^2 = x^2+y^2+9+2xy+6x+6y
    # 0 = 9+2xy+6x+6y => 2xy+6x+6y+9=0
    ("f=x+y+z+3\n2xy+6x+6y+9=0", 2*X*Y + 6*X + 6*Y + 9,
     "Hiperbola: 2xy+6x+6y+9=0"),

    # f5 = y/2+z+1 => z=-y/2-1 => x^2+y^2=(y/2+1)^2=y^2/4+y+1
    # x^2 + 3y^2/4 - y - 1 = 0
    ("f=y/2+z+1\nx^2+3y^2/4-y-1=0", X**2 + 3*Y**2/4 - Y - 1,
     "Elipsa: x^2 + 3y^2/4 - y - 1 = 0"),
]

for i, (title, F, desc) in enumerate(cases):
    ax = axes[i]
    ax.contour(X, Y, F, levels=[0], colors='blue')
    ax.set_title(title, fontsize=9)
    ax.set_xlabel('x'); ax.set_ylabel('y')
    ax.set_xlim(-8, 8); ax.set_ylim(-8, 8)
    ax.axhline(0, color='k', lw=0.5); ax.axvline(0, color='k', lw=0.5)
    ax.grid(True, alpha=0.3)
    ax.text(0.02, 0.02, desc, transform=ax.transAxes, fontsize=7, va='bottom')

axes[5].axis('off')
plt.suptitle("Lab 6c: V(I_f) po eliminacji z z ukladu x²+y²=z², f(x,y,z)=0\nIndeks 282212", fontsize=12)
plt.tight_layout()
plt.savefig("lab6c_plots.pdf", bbox_inches='tight')
plt.savefig("lab6c_plots.png", dpi=150, bbox_inches='tight')
print("Zapisano: lab6c_plots.pdf / lab6c_plots.png")
plt.show()
