# main.py

import pygame
import config
from src.camera import Camera
from src.display import Display
from src.comm import SerialComm
from time import sleep
import time
import threading


def cameraThread(camera: Camera):
    while True:
        start = time.time()
        camera.update()
        sleep(max(0, (1/(config.TARGET_FPS + 5)) - (time.time() - start)))
        print(f"cam fps: {1/(time.time() - start)}")
        
        
def commThread(comm: SerialComm):
    while True:
        start = time.time()
        comm.send_image()
        sleep(max(0, (1/(config.TARGET_FPS + 5)) - (time.time() - start)))
        

def main():
    # Initialize camera and display
    camera = Camera()
    display = Display()
    comm = SerialComm()
    
    threading.Thread(target=cameraThread, name="Camera Thread", args=[camera], daemon=True).start()
    threading.Thread(target=commThread, name="Comm Thread", args=[comm], daemon=True).start()

    # Main application loop
    running = True
    clock = pygame.time.Clock()
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        # Draw the frame on the display
        display.draw_frame()
        
        #print(clock.get_fps())
        
        clock.tick(config.TARGET_FPS)

    # Clean up resources
    camera.release()
    display.close()


if __name__ == "__main__":
    main()
