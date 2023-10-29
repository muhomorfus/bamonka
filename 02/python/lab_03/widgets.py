import tkinter as tk
from config import *


class RowFrame(tk.Frame):
    def __init__(self, size=ROW_SIZES, master=None, **kw):
        super().__init__(master, **kw)

        self.y_shift = self.find_y()
        self.size = size

    def find_y(self):
        slaves = self.master.children
        self.master.update()

        result = 0
        for i in slaves:
            result += slaves[i].winfo_height() + PADDING

        return result

    def arrange_row(self):
        self.place(relwidth=1, height=self.size, y=self.y_shift)


class BinaryEntry(RowFrame):
    entry_width = WINDOW_WIDTH - 5 * ROW_SIZES
    size = ROW_SIZES

    def __init__(self, master=None, **kw):
        super().__init__(self.size, master, **kw)

        self.entry = tk.Label(self, border=1,
                              borderwidth=1, relief='groove')
        self.entry.config(font=(MONO_FONT, FONT_SIZE))
        self.button_1 = tk.Button(self, text='1', relief='flat')
        self.button_1.config(font=(FONT, FONT_SIZE))
        self.button_0 = tk.Button(self, text='0', relief='flat')
        self.button_0.config(font=(FONT, FONT_SIZE))
        self.button_neg = tk.Button(self, text='-',
                                    relief='flat')
        self.button_neg.config(font=(FONT, FONT_SIZE))
        self.button_dot = tk.Button(self, text='.',
                                    relief='flat')
        self.button_dot.config(font=(FONT, FONT_SIZE))
        self.button_clear = tk.Button(self, text='<',
                                      relief='flat')
        self.button_clear.config(font=(FONT, FONT_SIZE))

    def arrange(self):
        self.arrange_row()
        self.entry.place(x=0, y=0, relheight=1, width=self.entry_width)
        self.button_1.place(x=self.entry_width, y=0, relheight=1,
                            width=self.size)
        self.button_0.place(x=self.entry_width + self.size, y=0, relheight=1,
                            width=self.size)
        self.button_neg.place(x=self.entry_width + 2 * self.size, y=0,
                              relheight=1,
                              width=self.size)
        self.button_dot.place(x=self.entry_width + 3 * self.size, y=0,
                              relheight=1,
                              width=self.size)
        self.button_clear.place(x=self.entry_width + 4 * self.size,
                                y=0, relheight=1, width=self.size)

        self.button_1.bind('<Button-1>', self.add_1)
        self.button_0.bind('<Button-1>', self.add_0)
        self.button_neg.bind('<Button-1>', self.make_negative)
        self.button_dot.bind('<Button-1>', self.add_dot)
        self.button_clear.bind('<Button-1>', self.delete_one)
        self.button_clear.bind('<Button-3>', self.delete_all)

    def add_1(self, _):
        self.entry['text'] += '1'

    def add_0(self, _):
        self.entry['text'] += '0'

    def add_dot(self, _):
        if not self.entry['text'].__contains__('.'):
            self.entry['text'] += '.'

    def delete_one(self, _):
        self.entry['text'] = self.entry['text'][:-1]

    def delete_all(self, _):
        self.entry['text'] = ''

    def get_text(self):
        return self.entry['text']

    def normalize(self):
        text = self.entry['text']
        minus = '-' if text.startswith('-') else ''
        text = text.lstrip(minus)
        text = text.lstrip('0')

        if text == '':
            text = '0'
            minus = ''
        elif text.startswith('.'):
            text = '0' + text
        elif text.endswith('.'):
            text = text + '0'

        self.entry['text'] = minus + text

    def make_negative(self, _):
        self.normalize()
        text = self.entry['text']

        if text == '0':
            pass
        elif text.startswith('-'):
            text = text.lstrip('-')
        else:
            text = '-' + text

        self.entry['text'] = text


class OperationsRow(RowFrame):
    width = WINDOW_WIDTH // 3
    height = ROW_SIZES

    def __init__(self, **kw):
        super().__init__(borderwidth=1, **kw)

        self.button_plus = tk.Button(self, text='+', font=FONT_SIZE)
        self.button_plus.config(font=(FONT, FONT_SIZE))
        self.button_minus = tk.Button(self, text='-', font=FONT_SIZE)
        self.button_minus.config(font=(FONT, FONT_SIZE))
        self.button_multiplication = tk.Button(self, text='*', font=FONT_SIZE)
        self.button_multiplication.config(font=(FONT, FONT_SIZE))

    def arrange(self):
        self.arrange_row()

        self.button_plus.place(x=0, y=0, width=self.width, height=self.height)
        self.button_minus.place(x=self.width, y=0, width=self.width,
                                height=self.height)
        self.button_multiplication.place(x=2 * self.width, y=0,
                                         width=self.width, height=self.height)


class ResultLabel(RowFrame):
    height = ROW_SIZES
    width = WINDOW_WIDTH

    def __init__(self, **kw):
        super().__init__(**kw)

        self.label = tk.Label(self, text='', font=FONT_SIZE, relief='flat')
        self.label.config(font=(MONO_FONT, FIELD_FONT_SIZE))

    def arrange(self):
        self.arrange_row()

        self.label.place(x=0, y=0, width=self.width, height=self.height)

    def set(self, val):
        self.label['text'] = val
