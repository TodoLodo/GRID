# src/camera.py

import cv2
import numpy as np
import config
import threading


class Camera:
    def __init__(self):
        # Initialize the camera
        self.cap = cv2.VideoCapture(1)

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
        
    def __current_image_update(self, frame):
        # Store cropped image for display purposes
        # Resize the cropped frame to fit half the screen width
        resized_frame = cv2.resize(frame, (config.HALF_SCREEN_SIZE, config.SCREEN_SIZE))
        # Convert to RGB format for Pygame
        resized_frame_rgb = cv2.cvtColor(resized_frame, cv2.COLOR_BGR2RGB)
        # Update the global current image
        config.CURRENT_IMAGE = resized_frame_rgb
    
    def __grid_image_update(self, frame):
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
        fg_mask_resize = cv2.resize(fg_mask, (config.GRID_WIDTH, config.GRID_HEIGHT), interpolation=cv2.INTER_NEAREST)

        # Resize frame to grid dimensions
        frame_resize = cv2.resize(frame, (config.GRID_WIDTH, config.GRID_HEIGHT), interpolation=cv2.INTER_AREA)
        # Convert to b/w
        frame_resize = cv2.cvtColor(frame_resize, cv2.COLOR_BGR2GRAY) # Convert to grayscale first
        frame_resize = cv2.adaptiveThreshold(frame_resize, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 11, 2)

        # Apply mask over resized image
        #grid = cv2.bitwise_and(frame_resize, fg_mask_resize)
        grid = frame_resize

        # Save grid to global variable
        config.GRID_IMAGE = grid
        
        """ print("===================================")
        for row in config.GRID_IMAGE:
            for n, col in enumerate(row):
                print(1 if col == 255 else 0, end="")
                if n % 8 == 7:
                    print(" ", end="")
            print() """
    
    def __scaled_grid_image_update(self):
        if config.GRID_IMAGE is not None:
            grid = config.GRID_IMAGE
            # Scale grid for display
            # Add led gaps
            grid_display = np.zeros((config.GRID_HEIGHT + (config.GRID_HEIGHT - 1) * config.GRID_LED_GAP, config.GRID_WIDTH + (config.GRID_WIDTH - 1) * config.GRID_LED_GAP, 3), np.uint8)
            grid_display[::(config.GRID_LED_GAP + 1), ::(config.GRID_LED_GAP + 1)][grid > 0] = (0, 0, 255)
            # Resize for display and save to global var
            config.SCALED_GRID_IMAGE = cv2.resize(grid_display, (config.HALF_SCREEN_SIZE, config.SCREEN_SIZE), interpolation=cv2.INTER_NEAREST)
        
    def update(self):
        # Capture frame from the camera
        ret, frame = self.cap.read()
        if not ret:
            print("No frame")
            return None

        # Crop the frame to a 2:1 aspect ratio
        frame = frame[self.crop_y:self.crop_y+self.crop_h, self.crop_x:self.crop_x+self.crop_w]

        threading.Thread(target=self.__current_image_update, args=[frame], daemon=True).start()
        threading.Thread(target=self.__grid_image_update, args=[frame], daemon=True).start()
        threading.Thread(target=self.__scaled_grid_image_update, daemon=True).start()
        
    def release(self):
        self.cap.release()
