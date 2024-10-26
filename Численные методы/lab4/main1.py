import math

# Программа для определения значения функции в заданных точках
i = 7
while i < 9:
    print(f'Значение функции в точке {i}: {math.log(i, 10):.4f}')
    i += 0.5
print(f'Точное значение функции в точке x = 7.2: {math.log(7.2, 10):.4f}\n')


# Программа для вычисления методом Лагранжа
# Способ, зная только начальные значения:
def lagrange_interpolation(x_, y_, x_val):
    result = 0
    for _ in range(len(x_)):
        term = y_[_]
        for j in range(len(x_)):
            if j != _:
                term *= (x_val - x_[j]) / (x_[_] - x_[j])
        result += term
    return result


x = [7, 7.5, 8, 8.5]
y = [math.log(7, 10), math.log(7.5, 10), math.log(8, 10), math.log(8.5, 10)]
print(f"Вычисления методом Лагранжа в точке x = 7.2: {lagrange_interpolation(x, y, 7.2):.4f}")


# Убираем функцию с неверным многочленом
# Программа для вычислений погрешностей
print(f"Абсолютная погрешность вычислений равна: {abs(lagrange_interpolation(x, y, 7.2) - math.log(7.2, 10)):.4f}")


# Погрешность формулы Лагранжа:
def error_lagrange(x_):
    error = (math.log(7.2, 10) / 5) * abs((x_ - 7) * (x_ - 7.5) * (x_ - 8) * (x_ - 8.5))
    return error


print(f"Оценка погрешности формулы Лагранжа: |Rn(x)| <= {error_lagrange(7.2):.4f}")