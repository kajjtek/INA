"""
Lab 6e — Trysektrisa Maclaurina
r = b*(4*cos(t) + 1/cos(t)), b=8
Kartezjanskie: (x^2+y^2)(x-8) - 32x^2 = 0
Czyli: x^3 + xy^2 - 40x^2 - 8y^2 = 0
"""
import numpy as np
import matplotlib.pyplot as plt

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(13, 6))

b = 8

# --- Wykres 1: biegunowy ---
theta = np.linspace(-np.pi/2 + 0.05, np.pi/2 - 0.05, 2000)
# Uwaga: 1/cos(t) -> nieskonczonosc przy t -> +/-pi/2
# Wybieramy galaz gdzie r > 0
with np.errstate(divide='ignore', invalid='ignore'):
    r = b * (4*np.cos(theta) + 1.0/np.cos(theta))
mask = np.isfinite(r) & (r > 0)
r = r[mask]; theta_m = theta[mask]
xp = r * np.cos(theta_m)
yp = r * np.sin(theta_m)
ax1.plot(xp, yp, 'b-', lw=2)
ax1.set_title(f"Trysektrisa biegunowa\nr = {b}(4cos θ + 1/cos θ)")
ax1.set_xlabel('x'); ax1.set_ylabel('y')
ax1.set_xlim(-5, 80); ax1.set_ylim(-40, 40)
ax1.axhline(0, color='k', lw=0.5); ax1.axvline(0, color='k', lw=0.5)
ax1.grid(True, alpha=0.3)

# --- Wykres 2: kartezjanski (kontur) ---
x = np.linspace(-5, 75, 1000)
y = np.linspace(-40, 40, 1000)
X, Y = np.meshgrid(x, y)
# (x^2+y^2)(x-8) - 32x^2 = 0
F = (X**2 + Y**2)*(X - b) - 4*b*X**2
cs = ax2.contour(X, Y, F, levels=[0], colors='crimson')
ax2.set_title(f"Trysektrisa kartezjanska\n(x²+y²)(x-{b}) = {4*b}x²")
ax2.set_xlabel('x'); ax2.set_ylabel('y')
ax2.axhline(0, color='k', lw=0.5); ax2.axvline(0, color='k', lw=0.5)
ax2.set_xlim(-5, 80); ax2.set_ylim(-40, 40)
ax2.grid(True, alpha=0.3)

# Asymptota pionowa x = b
ax2.axvline(b, color='green', ls='--', lw=1, label=f'asymptota x={b}')
ax2.legend(fontsize=9)

plt.suptitle("Lab 6e: Trysektrisa Maclaurina (b=8) — Kajetan Plewa 282212\n"
             "Sluzy do: trisektowania dowolnego kata (trojpodzial geometryczny)", fontsize=11)
plt.tight_layout()
plt.savefig("lab6e_plots.pdf", bbox_inches='tight')
plt.savefig("lab6e_plots.png", dpi=150, bbox_inches='tight')
print("Zapisano: lab6e_plots.pdf / lab6e_plots.png")
plt.show()
