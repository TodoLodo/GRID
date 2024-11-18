import serial
import config
import numpy as np
import cv2
import os


class SerialComm(serial.Serial):
    def __init__(self, port=config.COM_PORT, baudrate=921600, timeout=1):
        # Initialize the serial port
        self.img_arr = np.zeros(1600, dtype=np.uint8)

        # Prepare the image array with patterns
        for i in range(64):
            for j in range(0, 25, 5):
                header_index = j + i * 25
                row_index = header_index + 1
                col_index = row_index + 1

                self.img_arr[header_index] = 255
                self.img_arr[row_index] = i
                self.img_arr[col_index] = int(j/5) * 7

        try:
            super().__init__(port=port, baudrate=baudrate, timeout=timeout)
        except serial.SerialException:
            print(f"Failed to connect to port {port}. Setting port to None.")
            self.port = None

    def reconnect(self):
        """Attempt to reconnect to the configured port."""
        if self.port is None and config.COM_PORT:
            try:
                self.port = config.COM_PORT
                self.open()
                print(f"Reconnected to port {config.COM_PORT}.")
            except serial.SerialException as e:
                print(f"Failed to reconnect to port {config.COM_PORT}: {e}")
                self.port = None

    def __setPattern(self, img):
        # 32 x 64
        for row in range(64):
            for col in range(0, 32, 7):
                payload_index = int(col/7) * 5 + 3 + row * 25
                checksum_index = payload_index + 1
                payload_arr = img[row, col:col+7]
                for i, p in enumerate(payload_arr):
                    if p:
                        self.img_arr[payload_index] |= np.uint8(1<<(6 - i))
                    else:
                        self.img_arr[payload_index] &= ~np.uint8(1<<(6 - i))
                        
                    # checksum
                    self.img_arr[checksum_index] = (np.bitwise_xor.reduce(self.img_arr[payload_index-2:payload_index+1]) | 0x80) & 0xfe
                    
    def __printImg(self):
        # ANSI escape codes for colors
        PURPLE = '\033[95m'  # Purple color
        WHITE = '\033[97m'   # White color
        RESET = '\033[0m'    # Reset to default color

        # os.system("cls")

        for n, b in enumerate(self.img_arr):
            if n % 5 == 0:
                print()
            print(int(b), end=" ")

    def send_image(self):
        # Ensure there's an image to send
        if config.GRID_IMAGE is not None:
            # Flatten the image array into a bytearray
            self.__setPattern(config.GRID_IMAGE)
            
            #self.__printImg()

            # Attempt to reconnect if the port is None
            if self.port is None:
                # print("Serial port is not open. Attempting to reconnect...")
                """ self.reconnect() """

            # Send the bytearray over serial
            if self.is_open:
                self.write(self.img_arr)
                """ print("\n\nImage data sent over serial.\n\n") """
            else:
                ...
                # print("Failed to send image data. Serial port is not connected.")
        else:
            print("No image data to send.")
