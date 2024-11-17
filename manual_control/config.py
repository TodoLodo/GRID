# config.py

TARGET_FPS = 30

# Grid resolution and led gap size
GRID_WIDTH = 32
GRID_HEIGHT = 64
GRID_LED_GAP = 3

# Screen resolution and dimensions
SCREEN_SIZE = 600
HALF_SCREEN_SIZE = SCREEN_SIZE // 2

# Background color for the display
BACKGROUND_COLOR = (30, 30, 30)

# Global variable to store the current processed image
CURRENT_IMAGE = None
GRID_IMAGE = None
SCALED_GRID_IMAGE = None

# serial comms
COM_PORT = "COM3"
IP = None
PORT = 5588
