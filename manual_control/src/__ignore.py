import sys
import serial

img_arr = bytearray(384)

""" ser = serial.Serial('COM3', 115200) """

def initBits():
    for i in range(64):
        for j in range(6):
            byte_i = j + i * 6

            if j == 0 and i == 0:
                img_arr[byte_i] |= (3<<6)
            elif j == 0:
                img_arr[byte_i] |= (1<<6)
            elif j == 5 and i == 63:
                img_arr[byte_i] |= (1<<7)

initBits()            

def setBit(row, col, val):
    byte_i = row * 6 + col // 6

    if val:
        img_arr[byte_i] |= 1 << (col % 6)
    else:
        img_arr[byte_i] &= ~(1 << (col % 6))

setBit(63, 2, 1)

def setPattern(img):
    # 32uint x 64

    for row in range(64):
        img_row = img[row]
        for B_col in range(6):
            byte_i = B_col + row * 6
            bit_pattern = (img_row & ((2**6 - 1) << (B_col * 6))) >> (B_col * 6)

            img_arr[byte_i] &= 3<<6
            img_arr[byte_i] |= bit_pattern


def printImg():
    # ANSI escape codes for colors
    PURPLE = '\033[95m'  # Purple color
    WHITE = '\033[97m'   # White color
    RESET = '\033[0m'    # Reset to default color

    for n, byte in enumerate(img_arr):
        # Extract the first two MSBs and the rest of the bits
        msb_part = format((byte >> 6) & 0b11, '02b')  # First two MSBs
        lsb_part = format(byte & 0b00111111, '06b')   # Remaining 6 bits

        # Print with color coding
        print(f"{PURPLE}{msb_part}{RESET}{WHITE}{lsb_part}{RESET}", end=" ")

        # Print a newline after every 6 bytes for formatting
        if n % 6 == 5:
            print()


printImg()