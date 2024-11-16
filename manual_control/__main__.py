# main.py

import pygame
import config
from src.camera import Camera
from src.display import Display
from src.comm import SerialComm
from time import sleep
import time

def main():
    # Initialize camera and display
    camera = Camera()
    display = Display()
    comm = SerialComm()

    # Main application loop
    running = True
    while running:
        s = time.time()
        # Handle Pygame events
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        # Capture and process the camera frame
        camera.update()

        # Draw the frame on the display
        display.draw_frame()
        
        comm.send_image()
        
        print(f"fps: {1/(time.time() - s)}")

    # Clean up resources
    camera.release()
    display.close()

if __name__ == "__main__":
    main()
