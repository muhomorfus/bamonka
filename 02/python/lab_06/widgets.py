import tkinter as tk
from config import *


class RowFrame(tk.Frame):
    def __init__(self, size=ROW_SIZES, master=None, **kw):
        super().__init__(master, **kw)

        self.y_shift = self.find_y()
        self.height = size

    def find_y(self):
        slaves = self.master.children
        self.master.update()

        result = 0
        for i in slaves:
            result += slaves[i].winfo_height() + PADDING

        return result

    def arrange_row(self):
        self.place(relwidth=1, height=self.height, y=self.y_shift)


class PointEntry(RowFrame):
    entry_width = (WINDOW_WIDTH - 2 * ROW_SIZES) // 2
    size = ROW_SIZES

    def __init__(self, master=None, **kw):
        super().__init__(3*self.size, master, **kw)

        self.main_label = tk.Label(self, anchor='w')
        self.main_label['text'] = 'Точка'
        self.main_label.config(font=(FONT, FONT_SIZE))

        self.x_label = tk.Label(self, anchor='w')
        self.x_label['text'] = 'x: '
        self.x_label.config(font=(FONT, FONT_SIZE))

        self.x_entry = tk.Entry(self)
        self.x_entry.config(font=(MONO_FONT, FONT_SIZE))

        self.y_label = tk.Label(self, anchor='w')
        self.y_label['text'] = 'y: '
        self.y_label.config(font=(FONT, FONT_SIZE))

        self.y_entry = tk.Entry(self)
        self.y_entry.config(font=(MONO_FONT, FONT_SIZE))

        self.add_button = tk.Button(self, text='Добавить', relief='flat')
        self.add_button.config(font=(FONT, FONT_SIZE))

    def arrange(self):
        self.arrange_row()

        self.main_label.place(x=0, y=0, relheight=0.33)

        self.x_label.place(x=0, y=self.size, relheight=0.33, width=self.size)
        self.x_entry.place(x=self.size, y=self.size, relheigh=0.33, width=self.entry_width)

        self.y_label.place(x=self.size+self.entry_width, y=self.size, relheight=0.33, width=self.size)
        self.y_entry.place(x=2*self.size+self.entry_width, y=self.size, relheigh=0.33, width=self.entry_width)

        self.add_button.place(x=0, y=2*self.size, relheight=0.33, width=WINDOW_WIDTH)

    def clear(self):
        self.x_entry.delete(0, tk.END)
        self.y_entry.delete(0, tk.END)


class CirleEntry(RowFrame):
    center_entry_width = (WINDOW_WIDTH - 5 * ROW_SIZES) // 2
    radius_entry_width = WINDOW_WIDTH - 3 * ROW_SIZES
    size = ROW_SIZES

    def __init__(self, master=None, **kw):
        super().__init__(4*self.size, master, **kw)

        self.main_label = tk.Label(self, anchor='w')
        self.main_label['text'] = 'Окружность'
        self.main_label.config(font=(FONT, FONT_SIZE))

        self.center_label = tk.Label(self, anchor='w')
        self.center_label['text'] = 'Центр: '
        self.center_label.config(font=(FONT, FONT_SIZE))

        self.x_label = tk.Label(self, anchor='w')
        self.x_label['text'] = 'x: '
        self.x_label.config(font=(FONT, FONT_SIZE))

        self.x_entry = tk.Entry(self)
        self.x_entry.config(font=(MONO_FONT, FONT_SIZE))

        self.y_label = tk.Label(self, anchor='w')
        self.y_label['text'] = 'y: '
        self.y_label.config(font=(FONT, FONT_SIZE))

        self.y_entry = tk.Entry(self)
        self.y_entry.config(font=(MONO_FONT, FONT_SIZE))

        self.r_label = tk.Label(self, anchor='w')
        self.r_label['text'] = 'Радиус'
        self.r_label.config(font=(FONT, FONT_SIZE))

        self.r_entry = tk.Entry(self)
        self.r_entry.config(font=(MONO_FONT, FONT_SIZE))

        self.add_button = tk.Button(self, text='Добавить', relief='flat')
        self.add_button.config(font=(FONT, FONT_SIZE))

    def arrange(self):
        self.arrange_row()

        self.main_label.place(x=0, y=0, relheight=0.25)

        self.center_label.place(x=0, y=self.size, relheight=0.25, width=3*self.size)
        self.x_label.place(x=3*self.size, y=self.size, relheight=0.25, width=self.size)
        self.x_entry.place(x=4*self.size, y=self.size, relheight=0.25, width=self.center_entry_width)

        self.y_label.place(x=4*self.size+self.center_entry_width, y=self.size, relheight=0.25, width=self.size)
        self.y_entry.place(x=5*self.size+self.center_entry_width, y=self.size, relheight=0.25, width=self.center_entry_width)

        self.r_label.place(x=0, y=2*self.size, relheight=0.25, width=3*self.size)
        self.r_entry.place(x=3*self.size, y=2*self.size, relheight=0.25, width=self.radius_entry_width)

        self.add_button.place(x=0, y=3*self.size, relheight=0.25, width=WINDOW_WIDTH)
        
    def clear(self):
        self.x_entry.delete(0, tk.END)
        self.y_entry.delete(0, tk.END)
        self.r_entry.delete(0, tk.END)


class HistoryBox(RowFrame):
    def __init__(self, size=WINDOW_WIDTH, master=None, **kw):
        super().__init__(5*ROW_SIZES, master, **kw)

        self.height = size
        self.box = tk.Listbox(self, height=20)
        self.box.config(font=(MONO_FONT, HISTORY_FONT_SIZE))

    def arrange_history(self):
        self.arrange_row()
        self.box.place(x=0, y=0, width=WINDOW_WIDTH)

    def add_point(self, point):
        text = 'Точка: ({}, {})'.format(point.x, point.y)
        self.box.insert(tk.END, text)

    def add_circle(self, circle):
        text = 'Окружность: центр ({}, {}); радиус {}'.format(circle.center.x, circle.center.y, circle.r)
        self.box.insert(tk.END, text)

    def clear(self):
        self.box.delete(0, self.box.size())


class DrawButton(RowFrame):
    size = ROW_SIZES

    def __init__(self, master=None, **kw):
        super().__init__(self.size, master, **kw)

        self.button = tk.Button(self, text='Показать в canvas')
        self.button.config(font=(FONT, FONT_SIZE))

    def arrange(self):
        self.arrange_row()
        self.button.place(x=0, y=0, width=WINDOW_WIDTH, relheight=1)


