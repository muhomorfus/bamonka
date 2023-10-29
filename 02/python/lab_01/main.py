from math import sin, pi
import root_finder as finder
import finding_methods as methods

BORDER = pi

try:
    left = float(input('Введите левую границу диапазона > '))
    right = float(input('Введите правую границу диапазона > '))
    precision = float(input('Введите точность > '))
    iters = int(input('Введите количество итераций > '))
    step = float(input('Введите шаг > '))

    if step > BORDER:
        step = BORDER / 2
except:
    print('Неправильный ввод')
else:
    root_finder = finder.RootFinder(sin, left, right, precision, iters, step)
    finder.add_method(
        root_finder,
        methods.sec_method,
        'Метод секущих',
    )
    finder.add_method(
        root_finder,
        methods.brenth_method,
        'scipy.optimize.brenth',
    )
    finder.run(root_finder)
