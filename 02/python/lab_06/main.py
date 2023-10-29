import tkinter as tk
import tkinter.messagebox as box
from config import *
import widgets
import figures


points = []
circles = []


def add_point(_):
    try:
        x = float(point_entry.x_entry.get())
        y = float(point_entry.y_entry.get())
    except Exception as e:
        print(e)
        box.showerror('Ошибка ввода', 'Координаты точек должны быть целыми числами')
        point_entry.clear()
    else:
        point = figures.Point(x, y)
        points.append(point)
        history_box.add_point(point)
        point_entry.clear()


def add_circle(_):
    try:
        x = float(circle_entry.x_entry.get())
        y = float(circle_entry.y_entry.get())
        r = float(circle_entry.r_entry.get())
    except Exception as e:
        print(e)
        box.showerror('Ошибка ввода', 'Размерности должны быть целыми числами')
        circle_entry.clear()
    else:
        circle = figures.Circle(x, y, r)
        circles.append(circle)
        history_box.add_circle(circle)
        circle_entry.clear()


def draw_solution(_):
    print('Jopa')
    canvas_window = tk.Toplevel()
    canvas = tk.Canvas(canvas_window, width=1000, height=1000, bg='#ffffff')
    canvas.pack()

    figures.mark_solution(circles, points)
    for circle in circles:
        circle.draw(canvas)

    for point in points:
        point.draw(canvas)


root = tk.Tk()
root.title('Точки и окружности')
root.geometry('{}x{}+300+300'.format(WINDOW_WIDTH, WINDOW_HEIGHT))

point_entry = widgets.PointEntry()
point_entry.arrange()

point_entry.add_button.bind('<Button-1>', add_point)

circle_entry = widgets.CirleEntry()
circle_entry.arrange()

circle_entry.add_button.bind('<Button-1>', add_circle)

draw_button = widgets.DrawButton()
draw_button.arrange()

draw_button.button.bind('<Button-1>', draw_solution)

history_box = widgets.HistoryBox()
history_box.arrange_history()

history_box.clear()

main_menu = tk.Menu(root)

actions_menu = tk.Menu(main_menu)

clear_menu = tk.Menu(main_menu)
clear_menu.add_command(
    label='Очистить поля ввода точки',
    command=point_entry.clear
)
clear_menu.add_command(
    label='Очистить поля ввода окружности',
    command=circle_entry.clear
)


def clear_all():
    point_entry.clear()
    circle_entry.clear()

clear_menu.add_command(
    label='Очистить все поля',
    command=clear_all
)


def clear_all_data():
    global points, circles
    clear_all()
    history_box.clear()
    points = []
    circles = []


clear_menu.add_command(
    label='Очистить все',
    command=clear_all_data
)
main_menu.add_cascade(label='Очистка', menu=clear_menu)


def show_info():
    box.showinfo('Информация', 'Калькулятор окружностей и точек')


def show_author():
    box.showinfo('Автор', 'Alyosha Knyazhev aka %muhomorfus')


info_menu = tk.Menu(main_menu)
info_menu.add_command(label='О программе', command=show_info)
info_menu.add_command(label='Об авторе', command=show_author)
main_menu.add_cascade(label='Информация', menu=info_menu)

root.config(menu=main_menu)
root.mainloop()
