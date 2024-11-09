import config

def printImg(img_arr):
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
