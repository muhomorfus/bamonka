import pygame
import random
from config import *
import math
import datetime


# Get bottom y of sector
def get_bottom(x, houses):
    for house in houses:
        if x < house.left_x + house.width and x + SECTOR_WIDTH > house.left_x:
            return HEIGHT - house.height

    return HEIGHT


# Get UFO y position (sin)
def get_ufo_y(x):
    return int((MAX_UFO_Y - MIN_UFO_Y) * math.sin(x / (WIDTH / 10)) + (
                MIN_UFO_Y + MAX_UFO_Y) / 2)


# Snowflake class
class Snowflake:
    def __init__(self, x, y, radius):
        self.y = y
        self.x = x
        self.radius = radius

    def move(self):
        self.y += STEP

    def render(self, sc):
        pygame.draw.circle(sc, WHITE, (self.x, self.y), self.radius)


# Sector class
# All entire screen splitted into special sectors
# In this sectors generation snowflakes, then they are falling
# Then special white rectangle increases its size
class Sector:
    def __init__(self, x_start, width, bottom=0):
        self.max_timer = random.randint(MIN_TIMER, MAX_TIMER)
        self.timer = 0
        self.x_start = x_start
        self.width = width
        self.snow_height = 0
        self.bottom = bottom
        self.snowflakes = []

    def re_init_timer(self):
        self.max_timer = random.randint(MIN_TIMER, MAX_TIMER)
        self.timer = 0

    def render(self, sc):
        for i in range(len(self.snowflakes)):
            self.snowflakes[i].render(sc)

        pygame.draw.rect(sc, WHITE, (
            self.x_start, self.bottom - self.snow_height, self.width,
            self.snow_height))

    def process(self):
        for i in range(len(self.snowflakes)):
            self.snowflakes[i].move()

        i = 0
        while i < len(self.snowflakes):
            if self.snowflakes[i].y > self.bottom - self.snow_height:
                self.snow_height += self.snowflakes[i].radius

                self.snowflakes.pop(i)
            else:
                i += 1

        self.new_snowflake()

    def new_snowflake(self):
        if self.snow_height < MAX_SNOW_HEIGHT:
            if self.timer >= self.max_timer:
                radius = random.randint(MIN_SNOWFLAKE_RADIUS,
                                        MAX_SNOWFLAKE_RADIUS)
                x = int(self.x_start + self.width / 2)
                y = INIT_SNOWFLAKE_Y

                self.snowflakes.append(Snowflake(x, y, radius))
                self.re_init_timer()

            self.timer += 1


# Moon class
class Moon:
    def __init__(self, x, y, radius):
        self.x = x
        self.y = y
        self.radius = radius

    def render(self, sc):
        pygame.draw.circle(sc, MOON, (self.x, self.y), self.radius)
        pygame.draw.circle(sc, MOON_HOLE, (
            self.x - self.radius // 15, self.y - self.radius // 15),
                           self.radius // 8)
        pygame.draw.circle(sc, MOON_HOLE, (
            self.x - self.radius // 3, self.y - self.radius // 2),
                           self.radius // 6)
        pygame.draw.circle(sc, MOON_HOLE, (
            self.x + self.radius // 2, self.y - self.radius // 3),
                           self.radius // 9)
        pygame.draw.circle(sc, MOON_HOLE, (
            self.x - self.radius // 3, self.y + self.radius // 3),
                           self.radius // 10)
        pygame.draw.circle(sc, MOON_HOLE, (
            self.x - self.radius // 2, self.y + self.radius // 30),
                           self.radius // 14)
        pygame.draw.circle(sc, MOON_HOLE, (
            self.x + self.radius // 7, self.y + self.radius // 4),
                           self.radius // 14)
        pygame.draw.circle(sc, MOON_HOLE, (
            self.x + self.radius // 7 * 4, self.y + self.radius // 4),
                           self.radius // 8)
        pygame.draw.circle(sc, MOON_HOLE, (
            self.x + self.radius // 20, self.y + self.radius // 7 * 5),
                           self.radius // 12)


# UFO class (moving by sin)
class Ufo:
    def __init__(self, x, y, width, height):
        self.x = x
        self.y = y
        self.width = width
        self.height = height

    def move(self):
        self.x += UFO_X_STEP
        if self.x >= WIDTH:
            self.x = -3 * self.width

        self.y = get_ufo_y(self.x)

    def render(self, sc):
        pygame.draw.circle(sc, UFO_CAB, (
            self.x + self.width // 2, self.y + self.height // 2),
                           self.height // 2)
        pygame.draw.ellipse(sc, UFO_BODY, (
            self.x, self.y + self.height * 1 // 2, self.width,
            self.height // 2))


# Home window size
class Window:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        random.seed(datetime.datetime.timestamp(datetime.datetime.utcnow()))
        self.light = random.random() >= 0.5

    def flip(self):
        self.light = not self.light

    def render(self, sc):
        color = WINDOW_MAIN_DARK

        if self.light:
            color = WINDOW_MAIN_LIGHT

        pygame.draw.rect(sc, color,
                         (self.x, self.y, WINDOW_SIZE, WINDOW_SIZE))
        pygame.draw.rect(sc, WINDOW_OUTLINE,
                         (self.x, self.y, WINDOW_SIZE, WINDOW_SIZE),
                         OUTLINE_WIDTH)


# House size
class House:
    def __init__(self, x, floors_num, windows_num):
        self.floors_num = floors_num
        self.windows_num = windows_num
        self.width = windows_num * WINDOW_SIZE + (
                    windows_num + 1) * WINDOW_RADDING
        self.height = floors_num * WINDOW_SIZE + (
                    floors_num + 1) * WINDOW_RADDING
        self.windows = [[] for _ in range(floors_num)]
        self.timer = 0
        self.max_timer = UPDATE_WINDOWS_TIMER
        self.left_x = x - self.width
        self.right_x = x

        # Creating windows
        window_y = HEIGHT
        for i in range(floors_num):
            window_y -= WINDOW_SIZE + WINDOW_RADDING
            window_x = self.left_x + WINDOW_RADDING
            for j in range(windows_num):
                self.windows[i].append(Window(window_x, window_y))
                window_x += WINDOW_SIZE + WINDOW_RADDING

    # Update windows state
    def update(self):
        if self.timer >= self.max_timer:
            for k in range(CHANGE_WINDOWS):
                i = random.randint(0, len(self.windows) - 1)
                j = random.randint(0, len(self.windows[i]) - 1)
                self.windows[i][j].flip()
            self.timer = 0
        else:
            self.timer += 1

    def render(self, sc):
        top_y = HEIGHT - self.height
        pygame.draw.rect(sc, HOUSE_MAIN,
                         (self.left_x, top_y, self.width, self.height))
        pygame.draw.rect(sc, HOUSE_OUTLINE,
                         (self.left_x, top_y, self.width, self.height),
                         OUTLINE_WIDTH)

        for i in range(self.floors_num):
            for j in range(self.windows_num):
                self.windows[i][j].render(sc)


# Creating window
pygame.init()
pygame.mixer.init()

# Sizing
screen = pygame.display.set_mode((WIDTH, HEIGHT))

# Name of window
pygame.display.set_caption("Snow")

# Pygame clock
clock = pygame.time.Clock()

# Creating houses
house_x = WIDTH
houses = [House(house_x, 3, 2)]
house_x -= houses[-1].width + SECTOR_WIDTH
houses.append(House(house_x, 4, 3))
house_x -= houses[-1].width + SECTOR_WIDTH
houses.append(House(house_x, 5, 1))
house_x -= houses[-1].width + SECTOR_WIDTH
houses.append(House(house_x, 2, 2))

# Creating sectors
sectors = []
for i in range(SECTORS):
    sectors.append(Sector(i * SECTOR_WIDTH, SECTOR_WIDTH,
                          get_bottom(i * SECTOR_WIDTH, houses)))

# Creating moon
moon = Moon(100, 100, 50)

# Creating UFO
ufo = Ufo(200, 200, 100, 30)

# Game loop
running = True
while running:

    # FPS ticking
    clock.tick(FPS)
    # Handling events
    for event in pygame.event.get():
        # Check for closing window
        if event.type == pygame.QUIT:
            running = False

    # Updating
    for i in range(SECTORS):
        sectors[i].process()
    ufo.move()
    for i in range(len(houses)):
        houses[i].update()

    # Rendering
    screen.fill(NIGHT_BLUE)
    moon.render(screen)
    ufo.render(screen)
    for i in range(SECTORS):
        sectors[i].render(screen)
    for i in range(len(houses)):
        houses[i].render(screen)

    # Shows all updates
    pygame.display.flip()

pygame.quit()
