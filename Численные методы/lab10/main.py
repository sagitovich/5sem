import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import griddata


# Граничные условия
def Ux0(x): return x
def Ux1(x): return x + 10
def U0y(y): return y
def U1y(y): return y + 10


# Правая часть уравнения
def f(x, y): return y * (10 - x)


# Общая функция для отображения результата
def plot_result(U, h, method_name):
    p = len(U)  # Число узлов
    x = np.linspace(0, 10, p)  # Координаты узлов x
    y = np.linspace(0, 10, p)  # Координаты узлов y
    x, y = np.meshgrid(x, y)
    z = np.array(U)

    fig = plt.figure(figsize=(8, 6))
    ax = fig.add_subplot(111, projection='3d')
    ax.set_title(f"Метод: {method_name}, Сетка: {p-1}x{p-1}")
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_zlabel("U")

    # Интерполяция для более плавной поверхности
    grid_x, grid_y = np.mgrid[0:10:100j, 0:10:100j]
    z_interp = griddata((x.flatten(), y.flatten()), z.flatten(), (grid_x, grid_y), method='cubic')

    # Поверхность
    surface = ax.plot_surface(grid_x, grid_y, z_interp, cmap="viridis", rstride=1, cstride=1, edgecolor='none', alpha=0.9)

    # Настраиваем деления осей
    if h == 2.0:  # Для сетки 5x5
        ax.set_xticks(np.linspace(0, 10, 6))  # 0, 2, 4, 6, 8, 10
        ax.set_yticks(np.linspace(0, 10, 6))
    elif h == 1.0:  # Для сетки 10x10
        ax.set_xticks(np.linspace(0, 10, 11))  # 0, 1, 2, ..., 10
        ax.set_yticks(np.linspace(0, 10, 11))

    fig.colorbar(surface, ax=ax, shrink=0.5, aspect=10)
    plt.show()


# Метод простой итерации
def simple_iteration(h=0.1, epsilon=0.01):
    p = int(10 / h) + 1  # Число узлов
    U = [[0] * p for _ in range(p)]

    # Граничные условия
    for i in range(p):
        x = h * i
        U[i][0] = Ux0(x)
        U[i][-1] = Ux1(x)
    for j in range(1, p):
        y = h * j
        U[0][j] = U0y(y)
        U[-1][j] = U1y(y)

    # Итерации
    while True:
        max_diff = 0
        Un = [row[:] for row in U]
        for i in range(1, p - 1):
            for j in range(1, p - 1):
                x, y = h * i, h * j
                Un[i][j] = (U[i + 1][j] + U[i - 1][j] + U[i][j + 1] + U[i][j - 1] - h * h * f(x, y)) / 4
                max_diff = max(max_diff, abs(Un[i][j] - U[i][j]))
        U = Un
        if max_diff < epsilon:
            break

    plot_result(U, h, "Простая итерация")


# Метод Зейделя
def zeidel(h=0.1, epsilon=0.01):
    p = int(10 / h) + 1
    U = [[0] * p for _ in range(p)]

    # Граничные условия
    for i in range(p):
        x = h * i
        U[i][0] = Ux0(x)
        U[i][-1] = Ux1(x)
    for j in range(1, p):
        y = h * j
        U[0][j] = U0y(y)
        U[-1][j] = U1y(y)

    # Итерации
    while True:
        max_diff = 0
        for i in range(1, p - 1):
            for j in range(1, p - 1):
                x, y = h * i, h * j
                new_value = (U[i + 1][j] + U[i - 1][j] + U[i][j + 1] + U[i][j - 1] - h * h * f(x, y)) / 4
                max_diff = max(max_diff, abs(new_value - U[i][j]))
                U[i][j] = new_value
        if max_diff < epsilon:
            break

    plot_result(U, h, "Зейдель")


# Вызовы функций
simple_iteration(h=2.0)  # Сетка 5x5
simple_iteration(h=1.0)  # Сетка 10x10

zeidel(h=2.0)  # Сетка 5x5
zeidel(h=1.0)  # Сетка 10x10
