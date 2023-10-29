# Замыкание для сортировок
def sort_closure(arr, method):
    def closure():
        _ = method(arr)
    return closure


# Сортировка вставками для шага
def insertion_sort_step(arr, d):
    for i in range(d, len(arr), 1):
        j = i - d
        while j >= 0 and arr[j] > arr[j + d]:
            arr[j], arr[j + d] = arr[j + d], arr[j]
            j -= d


# Сортировка Шелла
def shell(arr):
    d = len(arr) // 2
    while d > 0:
        insertion_sort_step(arr, d)
        d //= 2
    return arr


