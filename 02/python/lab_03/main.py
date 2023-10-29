import tkinter as tk
import tkinter.messagebox as box
from config import *
import widgets
import bin_number as bn


root = tk.Tk()
root.title('Двоичный калькулятор')
root.geometry('{}x{}+300+300'.format(WINDOW_WIDTH, WINDOW_HEIGHT))

entry1 = widgets.BinaryEntry()
entry1.arrange()

entry2 = widgets.BinaryEntry()
entry2.arrange()

operations_row = widgets.OperationsRow()
operations_row.arrange()

result_label = widgets.ResultLabel()
result_label.arrange()


def get_numbers():
    entry1.normalize()
    entry2.normalize()

    number1 = bn.BinNumber(entry1.get_text())
    number2 = bn.BinNumber(entry2.get_text())
    
    return number1, number2


def to_result(sym, n1, n2, result):
    return '({}) {} ({}) = {}'.format(n1.text(), sym, n2.text(), result)


def run_plus(_):
    n1, n2 = get_numbers()
    
    result_label.set(to_result('+', n1, n2, n1.sum(n2).text()))


def run_minus(_):
    n1, n2 = get_numbers()

    result_label.set(to_result('-', n1, n2, n1.diff(n2).text()))


def run_multiply(_):
    n1, n2 = get_numbers()

    result_label.set(to_result('*', n1, n2, n1.multiple(n2).text()))


def clear_all_entries():
    entry1.delete_all(None)
    entry2.delete_all(None)
    result_label.set('')


def show_info():
    box.showinfo('Информация', 'Двоичный калькулятор')


def show_author():
    box.showinfo('Автор', 'Alyosha Knyazhev aka %muhomorfus')


operations_row.button_plus.bind('<Button-1>', run_plus)
operations_row.button_minus.bind('<Button-1>', run_minus)
operations_row.button_multiplication.bind('<Button-1>', run_multiply)

main_menu = tk.Menu(root)

actions_menu = tk.Menu(main_menu)
actions_menu.add_command(label='Суммировать', command=lambda: run_plus(None))
actions_menu.add_command(label='Вычесть', command=lambda: run_minus(None))
actions_menu.add_command(label='Умножить', command=lambda: run_multiply(None))
actions_menu.add_separator()
actions_menu.add_command(label='Выход', command=lambda: exit(0))
main_menu.add_cascade(label='Действия', menu=actions_menu)

clear_menu = tk.Menu(main_menu)
clear_menu.add_command(
    label='Очистить первое поле',
    command=lambda: entry1.delete_all(None)
)
clear_menu.add_command(
    label='Очистить второе поле',
    command=lambda: entry2.delete_all(None)
)
clear_menu.add_command(
    label='Очистить все поля',
    command=clear_all_entries
)
main_menu.add_cascade(label='Очистка', menu=clear_menu)

info_menu = tk.Menu(main_menu)
info_menu.add_command(label='О программе', command=show_info)
info_menu.add_command(label='Об авторе', command=show_author)
main_menu.add_cascade(label='Информация', menu=info_menu)

root.config(menu=main_menu)

root.mainloop()
