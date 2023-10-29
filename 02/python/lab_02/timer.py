import timeit
import sorts


# Замер времени выполнения функции над массивом
def measure(method, arr, runs):
    array = arr[:]
    closure = sorts.sort_closure(array, method)
    return timeit.timeit(closure, number=runs)
