import random


# Минимальный и максимальный элементы в массиве
MIN_NUM = -100
MAX_NUM = 100


# Базовый массив генерации
array = []


# Добавление рандомного элемента в базовый массив
def add_random_element():
    global array
    array.append(random.randint(MIN_NUM, MAX_NUM))


# Добавление n элементов в базовый массив
def add_random_elements(n):
    for i in range(n):
        add_random_element()


# Инициализация базового массива
def init_engine(max_length):
    global array
    array = []
    add_random_elements(max_length)
    return array


# Получение массивов трех типов - перемешанный, отсортированный по возрастанию
# и по убыванию заданного размера из базового массива
def get_arrays(length):
    global array

    # Чтобы не возникало ошибок, в случае меньшего размера базового массива
    # просто добавляем в него новые элементы
    if len(array) < length:
        add_random_elements(length - len(array))

    source_arr = array[:length]

    mixed_arr = source_arr[:]
    source_arr.sort()
    sorted_arr = source_arr[:]
    source_arr.sort(reverse=True)
    reversed_arr = source_arr[:]

    return {
        'size': length,
        'mixed': mixed_arr,
        'sorted': sorted_arr,
        'reversed': reversed_arr,
    }
