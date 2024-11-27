import matplotlib.pyplot as plt
import numpy as np

# Параметры
h = 0.1
tau = 0.01
eps = 0.01
a, b, c, d = 0, 1, 0, 1

# Размеры сетки
I = int((b - a) / h) + 1
J = int((d - c) / tau) + 1

# Инициализация массива
array = np.zeros((J, I))

# Начальные условия
for i in range(I):
    if a + i * h >= 0.5:
        array[0][i] = 2
    else:
        array[0][i] = 4

# Проверка условия устойчивости
for j in range(J - 1):
    for i in range(I - 1):
        array[j + 1][i] = (
                array[j][i]
                + tau / (2 * h) * (array[j][i - 1] ** 2 - array[j][i] ** 2)
                - 0.01 * (array[j][i + 1] - 2 * array[j][i] + array[j][i - 1])
        )

# Построение сетки
x = np.arange(a, b + h, h)
t = np.arange(c, d + tau, tau)
x, t = np.meshgrid(x, t)

# Построение графика
fig = plt.figure()
ax = fig.add_subplot(projection="3d")
ax.set_xlabel("x")
ax.set_ylabel("t")
ax.set_zlabel("U")
ax.plot_surface(x, t, array)
plt.show()
