import errors
from scipy import optimize
from math import sin, cos


# Изменение границ диапазона
def set_borders(f, a, b, x):
    if f(a) * f(x) <= 0:
        return a, x
    elif f(x) * f(b) <= 0:
        return x, b


# Метод секущих
def sec_method(f, a, b, precision, max_iter):
    if f(a) * f(b) > 0:
        return 0, errors.arithmetic_error, 0
    
    left = a
    right = b

    iterations = 1
    last_root = a

    while iterations <= max_iter:
        try:
            k = (f(right) - f(left)) / (right - left)
            b = f(left) - k * left

            sec_root = -b / k
        except:
            return '-', errors.arithmetic_error, iterations
        else:
            if f(sec_root) == 0:
                return sec_root, errors.success, iterations

            if iterations == 1:
                last_root = sec_root
                left, right = set_borders(f, left, right, sec_root)
                iterations += 1
                continue
            if abs(sec_root - last_root) < precision:
                return sec_root, errors.success, iterations
            left, right = set_borders(f, left, right, sec_root)
            last_root = sec_root
            iterations += 1

    return '-', errors.max_iters_exceed_error, '> {}'.format(max_iter)


# Обертка для scipy.optimize.brenth
def brenth_method(f, a, b, precision, max_iter):
    try:
        output = optimize.brenth(
            f=f,
            a=a,
            b=b,
            xtol=precision,
            maxiter=max_iter,
            full_output=True
        )[1]
    except RuntimeError:
        return '-', errors.max_iters_exceed_error, '> {}'.format(max_iter)
    except:
        return '-', errors.other_error, 0
    else:
        iters = output.iterations
        if iters > max_iter:
            iters = max_iter
            
        return output.root, errors.success, iters
