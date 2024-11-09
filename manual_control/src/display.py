# src/display.py

import pygame
import numpy as np
import config


class Display:
    def __init__(self):
        # Initialize Pygame and set up the screen
        pygame.init()
        self.screen = pygame.display.set_mode(
            (config.SCREEN_SIZE, config.SCREEN_SIZE))
        pygame.display.set_caption("Pygame Camera View")

    def draw_frame(self):
        # Fill the screen with the background color
        self.screen.fill(config.BACKGROUND_COLOR)

        # Check if the current image is available
        if config.CURRENT_IMAGE is not None:
            # Convert the frame to a Pygame surface
            frame_surface = pygame.surfarray.make_surface(
                np.rot90(config.CURRENT_IMAGE))

            # Render the frame on the left half of the screen (2:1 aspect ratio)
            self.screen.blit(frame_surface, (0, 0))
            
            frame_surface = pygame.surfarray.make_surface(
                np.rot90(config.SCALED_GRID_IMAGE))
            
            self.screen.blit(frame_surface, (config.HALF_SCREEN_SIZE, 0))

        # Only left half will display the video; right half remains background color
        pygame.display.flip()

    def close(self):
        pygame.quit()
