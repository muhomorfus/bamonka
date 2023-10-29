# Window parameters
WIDTH = 800
HEIGHT = 600

# FPS
FPS = 120

# Time snows is falling
TIME_ALL = 7

# Step of snowflake in 1 frame
STEP = HEIGHT / (TIME_ALL * FPS)

# Number of vertical sectors
SECTORS = 100

# Width of 1 sector
SECTOR_WIDTH = WIDTH // SECTORS

# Snowflakes radius
MIN_SNOWFLAKE_RADIUS = SECTOR_WIDTH // 7
MAX_SNOWFLAKE_RADIUS = SECTOR_WIDTH // 3

# Start position of all snowflakes
INIT_SNOWFLAKE_Y = MAX_SNOWFLAKE_RADIUS

# Duration between generation of new snowflakes
MIN_TIMER = FPS // 2
MAX_TIMER = FPS * 3

# Max height of snow before disabling generation of snowflakes
MAX_SNOW_HEIGHT = HEIGHT // 20

# Colors
WHITE = (255, 255, 255)
MOON = (255, 216, 59)
MOON_HOLE = (243, 196, 50)
NIGHT_BLUE = (37, 40, 80)
UFO_BODY = (130, 130, 130)
UFO_CAB = (164, 150, 242)
HOUSE_MAIN = (173, 89, 68)
HOUSE_OUTLINE = (136, 69, 53)
WINDOW_MAIN_LIGHT = (255, 205, 117)
WINDOW_MAIN_DARK = (50, 50, 50)
WINDOW_OUTLINE = (51, 26, 0)

# UFO position borders
MIN_UFO_Y = 100
MAX_UFO_Y = 130

# Time of flying UFO from border to border (Y axis)
TIME_UFO_Y = 1

# Step at 1 frame
UFO_Y_STEP = (MAX_UFO_Y - MIN_UFO_Y) / (FPS * TIME_UFO_Y)

# Time of flying UFO between screen borders (X axis)
TIME_UFO_X = 6

# Step at 1 frame
UFO_X_STEP = WIDTH / (FPS * TIME_UFO_X)

# Size of window in houses
WINDOW_SIZE = 40
WINDOW_RADDING = 16

# Size of outline
OUTLINE_WIDTH = 3

# Windows lights updates
UPDATE_WINDOWS_TIMER = FPS * 2
CHANGE_WINDOWS = 3
