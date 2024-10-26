import math


def f(x_):
    return math.log(x_, 10)


def df(x_):
    return 1 / (math.log(10) * x_)


def ddf(x_):
    return -1 / (math.log(10) * (x_ ** 2))


a = 3.4
b = 4.3
n = 4
h = (b - a) / (n - 1)

x = [a + i * h for i in range(n)]

y1_left = [(f(x[i]) - f(x[i] - h)) / h for i in range(n)]
y1_right = [(f(x[i] + h) - f(x[i])) / h for i in range(n)]
y1_center = [(f(x[i] + h) - f(x[i] - h)) / (2 * h) for i in range(n)]
y2 = [(f(x[i] + h) - 2 * f(x[i]) + f(x[i] - h)) / (h * h) for i in range(n)]

y1_exact = [df(x[i]) for i in range(n)]
y2_exact = [ddf(x[i]) for i in range(n)]

e1_left = [abs(y1_left[i] - y1_exact[i]) for i in range(n)]
e1_right = [abs(y1_right[i] - y1_exact[i]) for i in range(n)]
e1_center = [abs(y1_center[i] - y1_exact[i]) for i in range(n)]
e2 = [abs(y2[i] - y2_exact[i]) for i in range(n)]

for i in range(n):
    print(f"Точное значение первой производной в точке {x[i]:.3f} равно: {y1_exact[i]:.4f}")
print("\n")

for i in range(n):
    print(f"Значение производной левой в точке {x[i]:.3f} равно: {y1_left[i]:.4f} с погрешностью {e1_left[i]:.4f}")
print("\n")

for i in range(n):
    print(f"Значение производной правой в точке {x[i]:.3f} равно: {y1_right[i]:.4f} с погрешностью {e1_right[i]:.4f}")
print("\n")

for i in range(n):
    print(f"Значение производной центральной в точке {x[i]:.3f} равно: {y1_center[i]:.4f} с погрешностью {e1_center[i]:.4f}")
print("\n\n")

for i in range(n):
    print(f"Точное значение второй производной в точке {x[i]:.3f} равно: {y2_exact[i]:.4f}")
print("\n")

for i in range(n):
    print(f"Значение второй производной в точке {x[i]:.3f} равно: {y2[i]:.4f} с погрешностью {e2[i]:.4f}")
