import numpy as np
import matplotlib.pyplot as plt
import config as cfg
import data
from scipy.misc import derivative


def find_extremes(x, y):
    x_extremes = []
    y_extremes = []
    for i in range(1, len(y) - 1):
        if y[i - 1] < y[i] and y[i + 1] < y[i]:
            y_extremes.append(y[i])
            x_extremes.append(x[i])
        elif y[i - 1] > y[i] and y[i + 1] > y[i]:
            y_extremes.append(y[i])
            x_extremes.append(x[i])

    return x_extremes, y_extremes


def find_inflection_points(x, func):
    x_arr = []
    for i in range(len(x)):
        if abs(derivative(func, x[i], cfg.DX, 2)) < cfg.EPS:
            x_arr.append(x[i])

    x_result = np.array(x_arr)
    y_result = func(x_result)
    return x_result * 2, y_result


def make_plot_1(ax):
    ax.set_ylabel('Function', fontname='JetBrains Mono', fontsize=12)
    ax.set_xlabel('Argument', fontname='JetBrains Mono', fontsize=12)
    ax.set_title('Sultanov func', fontname='JetBrains Mono', fontsize=16)
    ax.set_xlim(cfg.PLOT_2_X_RANGE)

    x = np.linspace(*cfg.PLOT_2_X_RANGE, 1000)
    u_log_x = np.abs(x) + 1
    y = x * np.sin(np.log10(u_log_x) * x / 2) + np.exp(np.log2(u_log_x))
    ax.plot(x, y, color='blue')

    x_extremes, y_extremes = find_extremes(x, y)
    ax.plot(x_extremes, y_extremes, 'r*')


def make_plot_2(ax):
    ax.set_xlim(cfg.PLOT_2_X_RANGE)

    x = np.linspace(*cfg.PLOT_2_X_RANGE, 1000)
    y_1 = np.cos(x / 2)
    ax.plot(x, y_1, label='$\\cos \\frac{x}{2}$')

    x_inflections, y_inflections = find_inflection_points(x / 2, np.cos)
    ax.plot(x_inflections, y_inflections, 'go')

    u_log_x = np.abs(x) + 1
    y_2 = np.log2(u_log_x)
    ax.plot(x, y_2, linestyle='--', color='#22A637', label='$\\log_2 x$')
    plt.legend(loc='upper left')


def make_plot_3(ax):
    ax.set_xlim(cfg.PLOT_3_X_RANGE)
    ax.set_facecolor('seashell')

    x = np.linspace(*cfg.PLOT_3_X_RANGE, 1000)
    y_1 = np.cos(x)
    y_2 = -(x / 5 + 1) ** 2 + 3
    ax.plot(x, y_1, color='#C03030')
    ax.plot(x, y_2, color='#2020FF', linestyle='dotted', linewidth=3)
    ax.fill_between(x, y_1, y_2, where=y_2 > y_1, color='#FFE000', alpha=0.6)


def make_plot_4(ax):
    width = 0.3
    x = np.arange(len(data.LANG_NAMES))
    ax.bar(
        x - width / 2,
        data.SALARY_2019,
        width,
        label='Начало 2019',
        color='red',
    )
    ax.bar(
        x + width / 2,
        data.SALARY_2020,
        width,
        label='Конец 2020',
        color='yellowgreen',
    )

    ax.set_title('Сравнение зарплат')
    ax.set_xticks(x)
    ax.set_xticklabels(data.LANG_NAMES)
    ax.legend()


def make_plot_5(ax):
    ax.set_title('Популярность языков')
    ax.pie(
        data.PYPL_VALUES,
        labels=data.PYPL_NAMES,
        autopct='%1.1f%%',
        explode=data.PYPL_EXPLODES,
        wedgeprops={'lw': 1, 'ls': 'solid', 'edgecolor': 'black'},
        rotatelabels=False
    )


figure_1 = plt.figure(
    figsize=(15, 9)
)
figure_1.canvas.set_window_title('Sultanovs`s functions')

figure_1_grid = (4, 2)

make_plot_1(plt.subplot2grid(figure_1_grid, (0, 0), colspan=2, rowspan=2))
make_plot_2(plt.subplot2grid(figure_1_grid, (3, 0)))
make_plot_3(plt.subplot2grid(figure_1_grid, (3, 1)))

figure_2 = plt.figure(
    figsize=(8, 9)
)
figure_2.canvas.set_window_title('Statistics')

figure_2_grid = (2, 1)

make_plot_4(plt.subplot2grid(figure_2_grid, (0, 0)))
make_plot_5(plt.subplot2grid(figure_2_grid, (1, 0)))

plt.show()
