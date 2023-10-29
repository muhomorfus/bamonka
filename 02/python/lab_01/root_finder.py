import numpy as np
import table

def Method(method, name):
    t = table.Table(
        name,
        [
            'Номер корня',
            ('Отрезок', 20),
            ('Корень', 16),
            ('Функция', 16),
            ('Ошибка', 12),
            'Итерации'
        ],
    )

    return (method, t)

def method_method(m):
    return m[0]

def method_table(m):
    return m[1]

def RootFinder(f, left, right, precision, max_iters, step):
    return (f, left, right, precision, max_iters, step, [])

def finder_f(rf):
    return rf[0]

def finder_left(rf):
    return rf[1]

def finder_right(rf):
    return rf[2]

def finder_precision(rf):
    return rf[3]

def finder_max_iters(rf):
    return rf[4]

def finder_step(rf):
    return rf[5]

def finder_methods(rf):
    return rf[6]

def add_method(rf, method, name):
    m = Method(method, name)
    rf[6].append(m)

def run_method(rf, method):
    table.print_top(method_table(method))
    num_of_root = 1

    delta = 1e-8
    for i in np.arange(finder_left(rf), finder_right(rf) - delta,
                       finder_step(rf)):
        if finder_f(rf)(i) * finder_f(rf)(i + finder_step(rf)) <= 0:
            root, error, iters = method_method(method)(
                finder_f(rf),
                i,
                i + finder_step(rf),
                finder_precision(rf),
                finder_max_iters(rf)
            )

            root_str = '-'
            if type(root) != str:
                root_str = '{:.7f}'.format(root)

            func_str = '-'
            if root_str != '-':
                func_str = '{:.0e}'.format(finder_f(rf)(root))

            table.print_row(
                method_table(method),
                [
                    num_of_root,
                    '[{:.4f}, {:.4f}]'.format(i, i + finder_step(rf)),
                    root_str,
                    func_str,
                    error,
                    iters,
                ]
            )
            num_of_root += 1

def run(rf):
    for method in finder_methods(rf):
        run_method(rf, method)
