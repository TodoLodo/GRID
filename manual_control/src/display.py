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

        # Check if current image is available
        if config.CURRENT_IMAGE is not None:
            # Check if scaled grid image is available
            if config.SCALED_GRID_IMAGE is not None:
                # Combine both images
                combined_image = np.concatenate(
                    (config.SCALED_GRID_IMAGE, config.CURRENT_IMAGE),
                    axis=1)
                
                # Draw onto screen
                combined_image = pygame.surfarray.make_surface(np.rot90(combined_image))
                self.screen.blit(combined_image, (0, 0))
            
            else:
                # Scaled grid not availabe, render current image only
                frame_surface = pygame.surfarray.make_surface(np.rot90(config.CURRENT_IMAGE))
                self.screen.blit(frame_surface, (0, 0))

        pygame.display.flip()

    def close(self):
        pygame.quit()
