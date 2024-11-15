from math import sin


def double_recalculation(y1, y2, eps):
    for x in y2:
        if y1.get(x, False):
            if abs(y1[x] - y2[x]) > eps:
                return False
    return True


def f_1(x, y):  # уравнение 1
    return 1 - sin(2 * x + y)


def f_2(x, y):  # уравнение 2
    return 1 + (1 - x) * sin(y)


def Euler_Cauchy(f, a, b, n_, y_0):
    h = (b - a) / n_
    y = {a: y_0}
    for i in range(1, n_ + 1):
        y[a + i * h] = y[a + (i - 1) * h] + (h / 2) * (
                f(a + (i - 1) * h, y[a + (i - 1) * h]) +
                f(a + i * h, y[a + (i - 1) * h] + h * f(a + (i - 1) * h, y[a + (i - 1) * h])))
    return y


def Runge_Kutt_n(f, x_n, y_n, y, h):
    k_1 = f(x_n, y_n)
    k_2 = f(x_n + h / 2, y_n + (h / 2) * k_1)
    k_3 = f(x_n + h / 2, y_n + (h / 2) * k_2)
    k_4 = f(x_n + h, y_n + h * k_3)
    return y + (h / 6) * (k_1 + 2 * k_2 + 2 * k_3 + k_4)


def Runge_Kutt_4(f, a, b, n_, y_0):
    h = (b - a) / n_
    y = {a: y_0}
    for i in range(1, n_ + 1):
        y[a + i * h] = Runge_Kutt_n(f, a + (i - 1) * h, y[a + (i - 1) * h], y[a + (i - 1) * h], h)
    return y


def Adams_3(f, a, b, n_, y_0, g_0):
    h = (b - a) / n_
    Y, G = {a: y_0}, {a: g_0}
    G[a + h] = Runge_Kutt_n(f, a, Y[a], G[a], h)
    Y[a + h] = Y[a] + h * G[a]
    G[a + 2 * h] = Runge_Kutt_n(f, a + h, Y[a + h], G[a + h], h)
    Y[a + 2 * h] = Y[a + h] + h * G[a + h]
    for i in range(3, n_ + 1):
        G[a + i * h] = G[a + (i - 1) * h] + (h / 12) * (
                23 * f(a + (i - 1) * h, Y[a + (i - 1) * h]) - 16 * f(a + (i - 2) * h, Y[a + (i - 2) * h]) + 5 * f(
                    a + (i - 3) * h, Y[a + (i - 3) * h]))
        Y[a + i * h] = Y[a + (i - 1) * h] + (h / 12) * (
                23 * G[a + (i - 1) * h] - 16 * G[a + (i - 2) * h] + 5 * G[a + (i - 3) * h])
    return Y


def Adams_4(f, a, b, n_, y_0, g_0):
    h = (b - a) / n_
    Y, G = {a: y_0}, {a: g_0}
    G[a + h] = Runge_Kutt_n(f, a, Y[a], G[a], h)
    Y[a + h] = Y[a] + h * G[a]
    G[a + 2 * h] = Runge_Kutt_n(f, a + h, Y[a + h], G[a + h], h)
    Y[a + 2 * h] = Y[a + h] + h * G[a + h]
    G[a + 3 * h] = Runge_Kutt_n(f, a + 2 * h, Y[a + 2 * h], G[a + 2 * h], h)
    Y[a + 3 * h] = Y[a + 2 * h] + h * G[a + 2 * h]
    for i in range(4, n_ + 1):
        G[a + i * h] = G[a + (i - 1) * h] + (h / 24) * (
                55 * f(a + (i - 1) * h, Y[a + (i - 1) * h]) - 59 * f(a + (i - 2) * h, Y[a + (i - 2) * h]) + 37 * f(
                    a + (i - 3) * h, Y[a + (i - 3) * h]) - 9 * f(a + (i - 4) * h, Y[a + (i - 4) * h]))
        Y[a + i * h] = Y[a + (i - 1) * h] + (h / 24) * (
                55 * G[a + (i - 1) * h] - 59 * G[a + (i - 2) * h] + 37 * G[a + (i - 3) * h] - 9 * G[a + (i - 4) * h])
    return Y


def print_comparison(Y_prev, Y_last):
    # Получаем все точки для последней итерации
    keys_last = sorted(Y_last.keys())
    # Подмножество точек для предпоследней итерации, выбирая через одну начиная со второй
    keys_prev = sorted(Y_prev.keys())[1::2]  # Каждую вторую точку начиная со второй

    print(f"{'X_к':<10} | {'Y_к':<10} | {'Y_к last':<10} | {'Разность':<10}")
    print("-" * 48)

    for i, x_last in enumerate(keys_last):
        y_last = Y_last[x_last]

        # Проверяем, нужно ли выводить значение для предпоследней итерации
        if i % 2 == 1 and (i // 2) < len(keys_prev):  # только для каждой второй строки начиная со второй
            x_prev = keys_prev[i // 2]
            y_prev = Y_prev.get(x_prev, "")
            difference = abs(y_prev - y_last) if y_prev != "" else ""
            print(f"{x_last:<10.3f} | {y_prev:<10.3f} | {y_last:<10.3f} | {difference:<10.3f}")
        else:
            # Если нет значения для предпоследней итерации, оставляем пустое место
            print(f"{x_last:<10.3f} | {'':<10} | {y_last:<10.3f} | {'':<10}")


# Параметры
a_, b_ = 0, 0.5

# Уравнение 1
n = 2
prev_ = Euler_Cauchy(f_1, a_, b_, n, 0)
while True:
    n *= 2
    next_ = Euler_Cauchy(f_1, a_, b_, n, 0)
    if double_recalculation(next_, prev_, 0.001):
        break
    prev_ = next_
print(f'{48 * "="}\nЭйлер-Коши для уравнения 1:')
print_comparison(prev_, next_)

n = 2
prev = Runge_Kutt_4(f_1, a_, b_, n, 0)
while True:
    n *= 2
    next_ = Runge_Kutt_4(f_1, a_, b_, n, 0)
    if double_recalculation(next_, prev, 0.001):
        break
    prev = next_
print(f'{48 * "="}\nРунге-Кутта 4 для уравнения 1:')
print_comparison(prev, next_)

# Уравнение 2
n = 4
prev = Adams_3(f_2, a_, b_, n, 0, 1)
while True:
    n *= 2
    next_ = Adams_3(f_2, a_, b_, n, 0, 1)
    if double_recalculation(next_, prev, 0.001):
        break
    prev = next_
print(f'{48 * "="}\nАдамс 3 для уравнения 2:')
print_comparison(prev, next_)

n = 4
prev = Adams_4(f_2, a_, b_, n, 0, 1)
while True:
    n *= 2
    next_ = Adams_4(f_2, a_, b_, n, 0, 1)
    if double_recalculation(next_, prev, 0.001):
        break
    prev = next_
print(f'{48 * "="}\nАдамс 4 для уравнения 2:')
print_comparison(prev, next_)
