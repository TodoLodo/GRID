# src/camera.py

import cv2
import numpy as np
import config

class Camera:
    def __init__(self):
        # Initialize the camera
        self.cap = cv2.VideoCapture(0)

        # Initialize background substractor
        self.backsub = cv2.createBackgroundSubtractorKNN(200, 100, False)

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
        frame = frame[self.crop_y:self.crop_y+self.crop_h, self.crop_x:self.crop_x+self.crop_w]

        # Store cropped image for display purposes
        # Resize the cropped frame to fit half the screen width
        resized_frame = cv2.resize(frame, (config.HALF_SCREEN_SIZE, config.SCREEN_SIZE))
        # Convert to RGB format for Pygame
        resized_frame_rgb = cv2.cvtColor(resized_frame, cv2.COLOR_BGR2RGB)
        # Update the global current image
        config.CURRENT_IMAGE = resized_frame_rgb

        # Generate foreground mask
        fg_mask = self.backsub.apply(frame)
        # Apply morphology to denoise and fill holes in mask
        kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))
        fg_mask = cv2.morphologyEx(fg_mask, cv2.MORPH_OPEN, kernel, iterations=2)
        fg_mask = cv2.morphologyEx(fg_mask, cv2.MORPH_CLOSE, kernel, iterations=2)
        fg_mask = cv2.morphologyEx(fg_mask, cv2.MORPH_DILATE, kernel, iterations=20)
        fg_mask = cv2.morphologyEx(fg_mask, cv2.MORPH_ERODE, kernel, iterations=20)
        # Threshold mask to convert to b/w
        _, fg_mask = cv2.threshold(fg_mask, 1, 255, cv2.THRESH_BINARY)
        # Resize mask to grid dimensions
        fg_mask_resize = cv2.resize(fg_mask, (32, 64), interpolation=cv2.INTER_NEAREST)

        # Resize frame to grid dimensions
        frame_resize = cv2.resize(frame, (32, 64), interpolation=cv2.INTER_AREA)
        # Convert to b/w
        frame_resize = cv2.cvtColor(frame_resize, cv2.COLOR_BGR2GRAY) # Convert to grayscale first
        frame_resize = cv2.adaptiveThreshold(frame_resize, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 11, 2)

        # Apply mask over resized image
        grid = cv2.bitwise_and(frame_resize, fg_mask_resize)

        # Save grid to global variable
        config.GRID_IMAGE = grid

        # Scale grid for display
        # Add led gaps
        led_gap = 3
        grid_display = np.zeros((64 + (64 - 1) * led_gap, 32 + (32 - 1) * led_gap, 3), np.uint8)
        grid_display[::(led_gap + 1), ::(led_gap + 1)][grid > 0] = (0, 0, 255)
        # Resize for display and save to global var
        config.SCALED_GRID_IMAGE = cv2.resize(grid_display, (config.HALF_SCREEN_SIZE, config.SCREEN_SIZE), interpolation=cv2.INTER_NEAREST)

    def release(self):
        self.cap.release()
