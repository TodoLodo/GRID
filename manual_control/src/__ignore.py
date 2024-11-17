import numpy as np

img_arr = np.zeros(1600, dtype=np.uint8)

# Prepare the image array with patterns
for i in range(64):
    for j in range(0, 25, 5):
        header_index = j + i * 25
        row_index = header_index + 1
        col_index = row_index + 1
        
        img_arr[header_index] = 255
        img_arr[row_index] = i
        img_arr[col_index] = int(j/5) * 7
        
for n, b in enumerate(img_arr):
    if n % 5 == 0:
        print()
    print(int(b), end=" ")
    
     

arr = np.array([1,2,3,4,5,6,7,8,9,10,11,12,13])
i = 5

#print(arr[i-3:i], arr[i])