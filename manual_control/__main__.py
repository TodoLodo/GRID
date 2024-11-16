# main.py

import pygame
import config
from src.camera import Camera
from src.display import Display
from src.comm import SerialComm
from time import sleep

def main():
    # Initialize camera and display
    camera = Camera()
    display = Display()
    comm = SerialComm()

    # Main application loop
    running = True
    while running:
        # Handle Pygame events
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        # Capture and process the camera frame
        camera.update()

        # Draw the frame on the display
        display.draw_frame()
        
        comm.send_image()

        if comm.in_waiting > 0:  # Check if data is available
            line = comm.readline().decode('utf-8', errors='replace').strip()  # Read and strip extra spaces/newlines
            if line:
                """ print(line) """
        
        """ print(config.grid_image) """

    # Clean up resources
    camera.release()
    display.close()

if __name__ == "__main__":
    main()
