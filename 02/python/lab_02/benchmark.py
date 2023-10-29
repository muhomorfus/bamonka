import table
import timer


# Запуск тестирования метода с определенным количеством запусков
# и вывод данных в виде таблицы
def test(name, method, arrays, runs=10):
    table.clear()

    table.init_table(
        name,
        ['Количество элементов', 'Порядок элементов', 'Время работы'],
    )

    for triple in arrays:
        table.add_row(
            triple['size'],
            'по возрастанию',
            '{:.6f}'.format(timer.measure(method, triple['sorted'], runs)),
        )

        table.add_row(
            triple['size'],
            'по убыванию',
            '{:.6f}'.format(timer.measure(method, triple['reversed'], runs)),
        )

        table.add_row(
            triple['size'],
            'случайный',
            '{:.6f}'.format(timer.measure(method, triple['mixed'], runs)),
        )

        table.add_line()

    table.output()
    table.clear()
