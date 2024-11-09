# src/camera.py

import cv2
import numpy as np
import config


class Camera:
    def __init__(self):
        # Initialize the camera
        self.camera = cv2.VideoCapture(0)

    def update(self):
        # Capture frame from the camera
        ret, frame = self.camera.read()
        if not ret:
            print("Failed to capture frame from camera.")
            return None

        # Get dimensions and calculate a 2:1 crop from the center
        height, width = frame.shape[:2]
        target_width = height // 2
        crop_x = (width - target_width) // 2  # Center crop horizontally
        crop_y = 0                            # Full vertical crop for 2:1 aspect

        # Crop the frame to a 2:1 aspect ratio
        cropped_frame = frame[crop_y:crop_y+height, crop_x:crop_x+target_width]

        # Resize the cropped frame to fit half the screen width
        resized_frame = cv2.resize(
            cropped_frame, (config.HALF_SCREEN_SIZE, config.SCREEN_SIZE))

        # Convert to RGB format for Pygame
        resized_frame_rgb = cv2.cvtColor(resized_frame, cv2.COLOR_BGR2RGB)

        # Update the global current image
        config.CURRENT_IMAGE = resized_frame_rgb
        
        resized_image = cv2.resize(cv2.cvtColor(config.CURRENT_IMAGE, cv2.COLOR_BGR2GRAY), (32, 64))
        threshold_value = 127
        _, thresholded_image = cv2.threshold(resized_image, threshold_value, 255, cv2.THRESH_BINARY)
        
        config.GRID_IMAGE = thresholded_image
        
        config.SCALED_GRID_IMAGE = cv2.resize(thresholded_image, (config.HALF_SCREEN_SIZE, config.SCREEN_SIZE), interpolation=cv2.INTER_NEAREST)

    def release(self):
        self.camera.release()
