import generator
import benchmark
import sorts


# Заполнение массивов
def fill_arrays(*arr_sizes):
    result = []
    generator.init_engine(max(sizes))
    for size in arr_sizes:
        result.append(generator.get_arrays(size))
    return result


runs = int(input('Введите количество запусков > '))
sizes = [
    int(x) for x in input('Введите размеры массивов через пробел > ').split()
]

arrays = fill_arrays(*sizes)


print('Тесты для {:d} запусков сортировки'.format(runs))
print()
benchmark.test('sorted()', sorted, arrays, runs)
benchmark.test('Сортировка Шелла', sorts.shell, arrays, runs)

