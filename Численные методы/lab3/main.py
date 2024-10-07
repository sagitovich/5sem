import math
import sympy as sp


def foo(x):
    return x * math.log(x)


def F(x):
    return (x * x / 2) * math.log(x) - (x * x / 4)


def leftRect(n, f, a, b):
    summ = 0
    h = (b - a) / n
    for i in range(n):
        summ += f(a + i * h)
    return summ * h


def rightRect(n, f, a, b):
    summ = 0
    h = (b - a) / n
    for i in range(1, n + 1):
        summ += f(a + i * h)
    return summ * h


def centreRect(n, f, a, b):
    summ = 0
    h = (b - a) / n
    for i in range(1, n + 1):
        summ += f(a + ((i - 1 / 2) * h))
    return summ * h


def trapezia(n, f, a, b):
    summ = 0
    h = (b - a) / n
    for i in range(1, n):
        summ += f(a + i * h)
    return h * ((f(a) + f(b)) / 2 + summ)


def simpson(N, f, a, b):
    n = N
    N = 2 * N
    h = (b - a) / N
    summ_odd = 0
    for i in range(1, n + 1):
        summ_odd += f(a + (2 * i - 1) * h)
    summ_even = 0
    for i in range(1, n):
        summ_even += f(a + (2 * i) * h)
    return h / 3 * (f(a) + 2 * summ_even + 4 * summ_odd + f(b))


def exact_integral():
    x = sp.symbols('x')
    f = x * sp.log(x)
    exact_value = sp.integrate(f, (x, 2, 6))
    return exact_value.evalf()


eps = 10 ** (-4)
expectedRes = exact_integral()

points = 2
res_prev = leftRect(1, foo, 2, 6)
res_next = leftRect(2, foo, 2, 6)
while abs(res_prev - res_next) > eps:
    points *= 2
    res_prev = res_next
    res_next = leftRect(points, foo, 2, 6)
print(f'Метод левых прямоугольников\n'
      f'Значение: {res_next}\n'
      f'Точек разбиения: {points}\n'
      f'Величина последнего шага: {3 / points}\n'
      f'Относительная погрешность: {abs((res_next - expectedRes) / expectedRes) * 100}%\n')


points = 2
res_prev = rightRect(1, foo, 2, 6)
res_next = rightRect(2, foo, 2, 6)
while abs(res_prev - res_next) > eps:
    points *= 2
    res_prev = res_next
    res_next = rightRect(points, foo, 2, 6)
print(f'Метод правых прямоугольников\n'
      f'Значение: {res_next}\n'
      f'Точек разбиения: {points}\n'
      f'Величина последнего шага: {3 / points}\n'
      f'Относительная погрешность: {abs((res_next - expectedRes) / expectedRes) * 100}%\n')


points = 2
res_prev = centreRect(1, foo, 2, 6)
res_next = centreRect(2, foo, 2, 6)
while abs(res_prev - res_next) > eps:
    points *= 2
    res_prev = res_next
    res_next = centreRect(points, foo, 2, 6)
print(f'Метод средних прямоугольников\n'
      f'Значение: {res_next}\n'
      f'Точек разбиения: {points}\n'
      f'Величина последнего шага: {3 / points}\n'
      f'Относительная погрешность: {abs((res_next - expectedRes) / expectedRes) * 100}%\n')


points = 2
res_prev = trapezia(1, foo, 2, 6)
res_next = trapezia(2, foo, 2, 6)
while abs(res_prev - res_next) > eps:
    points *= 2
    res_prev = res_next
    res_next = trapezia(points, foo, 2, 6)
print(f'Метод трапеций\n'
      f'Значение: {res_next}\n'
      f'Точек разбиения: {points}\n'
      f'Величина последнего шага: {3 / points}\n'
      f'Относительная погрешность: {abs((res_next - expectedRes) / expectedRes) * 100}%\n')


points = 2
res_prev = simpson(1, foo, 2, 6)
res_next = simpson(2, foo, 2, 6)
while abs(res_prev - res_next) > eps:
    points *= 2
    res_prev = res_next
    res_next = simpson(points, foo, 2, 6)
print(f'Метод Симпсона\n'
      f'Значение: {res_next}\n'
      f'Точек разбиения: {points}\n'
      f'Величина последнего шага: {3 / points}\n'
      f'Относительная погрешность: {abs((res_next - expectedRes) / expectedRes) * 100}%\n')


print(f'Точное значение интеграла: {expectedRes}')
