# src/serial_comm.py

import serial
import config
import numpy as np
import cv2


class SerialComm(serial.Serial):
    def __init__(self, port=config.COM_PORT, baudrate=115200, timeout=1):
        # Initialize the serial port
        
        self.img_arr = bytearray(384)
        
        for i in range(64):
            for j in range(6):
                byte_i = j + i * 6

                if j == 0 and i == 0:
                    self.img_arr[byte_i] |= (3<<6)
                elif j == 0:
                    self.img_arr[byte_i] |= (1<<6)
                elif j == 5 and i == 63:
                    self.img_arr[byte_i] |= (1<<7)
        
        super().__init__(port=port, baudrate=baudrate, timeout=timeout)
        
    def __setPattern(self, img):
        # 32uint x 64

        for row in range(64):
            img_row = img[row]
            for B_col in range(6):
                byte_i = B_col + row * 6
                bit_pattern = (img_row & ((2**6 - 1) << (B_col * 6))) >> (B_col * 6)

                self.img_arr[byte_i] &= 3<<6
                self.img_arr[byte_i] |= bit_pattern
                
    def __printImg(self):
        # ANSI escape codes for colors
        PURPLE = '\033[95m'  # Purple color
        WHITE = '\033[97m'   # White color
        RESET = '\033[0m'    # Reset to default color

        print()
        for n, byte in enumerate(self.img_arr):
            # Extract the first two MSBs and the rest of the bits
            msb_part = format((byte >> 6) & 0b11, '02b')  # First two MSBs
            lsb_part = format(byte & 0b00111111, '06b')   # Remaining 6 bits

            # Print with color coding
            print(f"{PURPLE}{msb_part}{RESET}{WHITE}{lsb_part}{RESET}", end=" ")

            # Print a newline after every 6 bytes for formatting
            if n % 6 == 5:
                print()


    def send_image(self):
        # Ensure there's an image to send
        if config.GRID_IMAGE is not None:
            # Flatten the image array into a bytearray
            uint32_img = np.zeros(64, dtype=np.uint32)

            # Convert each row of 32 pixels into a uint32
            for row_index in range(64):
                row_bits = 0  # Start with an empty 32-bit integer
                for col_index in range(32):
                    # Set the bit if the pixel is white (255)
                    if config.GRID_IMAGE[row_index, col_index] == 255:
                        row_bits |= (1 << (31 - col_index))  # Set the corresponding bit
                # Store the resulting 32-bit integer in the array
                uint32_img[row_index] = row_bits
                
            self.__setPattern(uint32_img)
            
            self.__printImg()

            # Send the bytearray over serial
            #self.write(self.img_arr)
            print("Image data sent over serial.")
        else:
            print("No image data to send.")
