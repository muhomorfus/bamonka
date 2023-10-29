from functools import reduce

# Текущая таблица
table = []

# Индексы сервисных штук: названия табицы, названий полей и начала данных
NAME = 0
FIELDS = 1
START = 2


# Идентификатор линии
LINE = 0


# Проверяет, пуста ли таблица
def is_table_empty():
    return len(table) == 0


# Очистка таблицы
def clear():
    global table
    table = []


# Инициализация таблицы
def init_table(name, fields):
    global table

    if not is_table_empty():
        raise Exception('you cant set fields to non-empty table')

    table = [None, None]
    fields = [str(f) for f in fields]
    new_fields = [(f, len(f)) for f in fields]
    table[NAME] = name
    table[FIELDS] = new_fields


# Добавлание строки в таблицу
def add_row(*row):
    global table

    # Преобразование кортежа данных в массив строк
    row = [str(cell) for cell in row]
    if is_table_empty():
        raise Exception('you cant add rows to empty table')

    # Подгоняем количество полей в строке под количество полей таблицы
    if len(table[FIELDS]) > len(row):
        row += ['-'] * (len(table[FIELDS]) - len(row))
    elif len(table[FIELDS]) < len(row):
        row = row[:len(table[FIELDS])]
    else:
        pass

    # Изменяем ширины столбцов при необходимости
    for i in range(len(table[FIELDS])):
        if len(row[i]) > table[FIELDS][i][1]:
            table[FIELDS][i] = (table[FIELDS][i][0], len(row[i]))

    table.append(row)


# Добавление линии в таблицу
def add_line():
    table.append(LINE)


# Печать горизонтальной линии таблицы
def print_separator():
    if is_table_empty():
        return
    else:
        lines_arr = ['-' * (f[1] + 2) for f in table[FIELDS]]
        print('+' + '+'.join(lines_arr) + '+')


# Вычисление общей ширины таблицы
def full_width():
    if is_table_empty():
        return 0

    def add_to_width(res, cur):
        return res + 2 + cur[1]

    width = reduce(add_to_width, table[FIELDS], 0)
    width += len(table[FIELDS]) + 1
    return width


# Печать названия таблицы
def print_name():
    global table
    table_width = full_width()
    name = ' ' + table[NAME] + ' '
    print(name.center(table_width, '='))


# Оформление поля-заголовка таблицы
def format_field(text, length, header):
    text = ' ' + text + ' '

    if header:
        result = text.center(length + 2)
        result = '\x1b[1m' + result + '\x1b[0m'
    else:
        result = text.rjust(length + 2)

    return result


# Вывод строки таблицы
def print_row(row, header=False):
    global table

    if row == LINE:
        print_separator()
    else:
        cells = []
        for i in range(len(row)):
            cells.append(
                format_field(row[i], table[FIELDS][i][1], header),
            )

        print('|' + '|'.join(cells) + '|')


# Вывод всей таблицы
def output():
    if is_table_empty():
        return

    print_name()
    print_separator()
    print_row([field[0] for field in table[FIELDS]], header=True)
    print_separator()
    for i in range(START, len(table)):
        if i == len(table) - 1 and table[i] == LINE:
            pass
        else:
            print_row(table[i])
    print_separator()
    print()
