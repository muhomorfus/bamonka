def get_width(text):
    return len(text)


def to_center(text, width):
    return text.center(width)


def to_left(text, width):
    return text.ljust(width)


def to_right(text, width):
    return text.rjust(width)


def Field(field):
    name = ''
    width = 0
    
    if type(field) is tuple:
        if len(field) >= 2:
            text = str(field[0])
            name = text
            width = max(field[1], get_width(text))
        elif len(field) == 1:
            text = str(field[0])
            name = text
            width = get_width(text)
    else:
        text = str(field)
        name = text
        width = get_width(text)
            
    return (name, width)


def field_get_width(field):
    return field[1]


def field_get_name(field):
    return field[0]


def Table(name, fields):
    # Название полей таблицы
    fs = []
    for field in fields:
        f = Field(field)
        fs.append(f)

    return (name, fs)


def table_get_fields(tbl):
    return tbl[1]


def table_get_name(tbl):
    return tbl[0]


# Печать горизонтального разделителя
def print_separator(tbl):
    separator = '+'
    for field in table_get_fields(tbl):
        separator += '-' * (field_get_width(field) + 2)
        separator += '+'
    print(separator)


# Вывод строки таблицы
def print_row(tbl, content, align=to_right):
    row = '| '
    for i in range(len(table_get_fields(tbl))):
        if len(content) <= i:
            text = ''
        else:
            text = str(content[i])
        row += align(text, field_get_width(table_get_fields(tbl)[i]))
        row += ' | '
    print(row)
    print_separator(tbl)


# Вывод шапки таблицы
def print_header(tbl):
    print_separator(tbl)
    content = [field_get_name(field) for field in table_get_fields(tbl)]
    print_row(tbl, content, to_center)


# Вывод названия таблицы
def print_name(tbl):
    print(' {} '.format(table_get_name(tbl))
          .center(table_get_full_width(tbl), '='))


# Ширина всей таблицы	
def table_get_full_width(tbl):
    return sum([field_get_width(f) + 2 for f in table_get_fields(tbl)]) + len(table_get_fields(tbl)) + 1


# Вывод названия и шапки таблицы
def print_top(tbl):
    print()
    print_name(tbl)
    print_header(tbl)

