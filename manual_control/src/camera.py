# src/camera.py

import cv2
import numpy as np
import config

class Camera:
    def __init__(self):
        # Initialize the camera
        self.cap = cv2.VideoCapture(0)

        # Crop a 2:1 aspect ration from the center of frame
        # Calculate height and width for cropping
        frame_width = int(self.cap.get(cv2.CAP_PROP_FRAME_WIDTH)) # Source frame width
        frame_height = int(self.cap.get(cv2.CAP_PROP_FRAME_HEIGHT)) # Source frame height

        if frame_width > (frame_height // 2):
            self.crop_w = frame_height // 2 # Cropped width
            self.crop_h = self.crop_w * 2 # Cropped height
        else:
            self.crop_h = frame_width * 2 # Cropped height
            self.crop_w = self.crop_h // 2 # Cropped width
        
        # Calculate offsets for cropping
        self.crop_x = (frame_width - self.crop_w) // 2 # Cropping x offset
        self.crop_y = (frame_height - self.crop_h) // 2 # Cropping y offset

    def update(self):
        # Capture frame from the camera
        ret, frame = self.cap.read()
        if not ret:
            print("No frame")
            return None

        # Crop the frame to a 2:1 aspect ratio
        cropped_frame = frame[self.crop_y:self.crop_y+self.crop_h, self.crop_x:self.crop_x+self.crop_w]

        # Resize the cropped frame to fit half the screen width
        resized_frame = cv2.resize(cropped_frame, (config.HALF_SCREEN_SIZE, config.SCREEN_SIZE))

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
