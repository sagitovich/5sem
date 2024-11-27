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

# Расчёт значений по схеме
for j in range(J - 1):
    for i in range(1, I - 1):  
        array[j + 1][i] = (
            array[j][i]
            - tau / h * array[j][i] * (array[j][i] - array[j][i - 1])
            - eps**2 / 2 * tau / h**3 * (array[j][i + 1] - 2 * array[j][i] + array[j][i - 1])
        )

# Проверка условия устойчивости
for j in range(J):
    for i in range(I):
        if array[j][i] > (h / tau):
            print("Условие устойчивости не выполнилось!")
            break

# Построение сетки
x = np.arange(a, b + h, h)
t = np.arange(c, d + tau, tau)
x, t = np.meshgrid(x, t)

# Построение графика
fig = plt.figure()
ax = fig.add_subplot(projection="3d")
ax.set_xlabel("t")
ax.set_ylabel("x")
ax.set_zlabel("U")
ax.plot_surface(t, x, array)
plt.show()
