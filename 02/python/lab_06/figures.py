import colors
import math


class Circle:
    def __init__(self, x, y, r):
        self.center = Point(x, y)
        self.r = r
        self.color = colors.BLACK

    def mark(self):
        self.color = colors.RED

    def unmark(self):
        self.color = colors.BLACK

    def draw(self, canvas):
        left = self.center.x - self.r
        right = self.center.x + self.r

        top = self.center.y - self.r
        bottom = self.center.y + self.r
        canvas.create_arc(left, top, right, bottom, style='arc', outline=self.color,
                     start=0, extent=180, width=2)
        canvas.create_arc(left, top, right, bottom, style='arc', outline=self.color,
                     start=0, extent=-180, width=2)

    def is_inside(self, point):
        return self.center.distance(point) < self.r

    def is_outside(self, point):
        return self.center.distance(point) > self.r


class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def distance(self, other):
        return math.sqrt((self.x - other.x)**2 + (self.y - other.y)**2)

    def draw(self, canvas):
        canvas.create_oval(self.x, self.y, self.x, self.y, width=5, fill=colors.BLUE, outline=colors.BLUE)


def mark_solution(circles, points):
    min_index = -1
    min_diff = 0
    for i in range(len(circles)):
        inside = 0
        outside = 0
        for point in points:
            inside += 1 if circles[i].is_inside(point) else 0
            outside += 1 if circles[i].is_outside(point) else 0
        diff = abs(outside - inside)
        if min_index < 0 or diff < min_diff:
            min_diff = diff
            min_index = i

    if min_index != -1:
        circles[min_index].mark()
