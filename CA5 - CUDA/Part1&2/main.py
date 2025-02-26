from PIL import Image
import multiprocessing
import numpy as np
import time
import matplotlib.pyplot as plt

sobel_x_kernel = [
   [-1, 0, 1],
   [-2, 0, 2],
   [-1, 0, 1]
]

sobel_y_kernel = [
   [-1, -2, -1],
   [ 0,  0,  0],
   [ 1,  2,  1]
]

def apply_kernel(image_array, kernel, y_range):
   result = []
   kernel = np.array(kernel)
   
   for y in y_range:
       for x in range(1, image_array.shape[1] - 1):
           window = image_array[y-1:y+2, x-1:x+2]
           value = np.sum(window * kernel)
           result.append((y, x, int(abs(value))))
   
   return result

def sobel_filter_parallel(image, num_processes):
   image_array = np.array(image)
   height, width = image_array.shape
   
   chunk_size = (height - 2) // num_processes
   y_ranges = []
   for i in range(num_processes):
       start = 1 + i * chunk_size
       end = start + chunk_size if i < num_processes - 1 else height - 1
       y_ranges.append(range(start, end))

   with multiprocessing.Pool(processes=num_processes) as pool:
       results_x = pool.starmap(apply_kernel, 
           [(image_array, sobel_x_kernel, y_range) for y_range in y_ranges])
       results_y = pool.starmap(apply_kernel, 
           [(image_array, sobel_y_kernel, y_range) for y_range in y_ranges])

   output_array = np.zeros((height-2, width-2), dtype=np.uint8)
   
   for result_x, result_y in zip(results_x, results_y):
       for (y, x, gx), (_, _, gy) in zip(result_x, result_y):
           magnitude = min(255, int((gx**2 + gy**2)**0.5))
           output_array[y-1, x-1] = magnitude

   return Image.fromarray(output_array)

def sobel_filter_serial(image):
   image_array = np.array(image)
   height, width = image_array.shape
   kernel_x = np.array(sobel_x_kernel)
   kernel_y = np.array(sobel_y_kernel)
   
   output_array = np.zeros((height-2, width-2), dtype=np.uint8)
   
   for y in range(1, height-1):
       for x in range(1, width-1):
           window = image_array[y-1:y+2, x-1:x+2]
           gx = np.sum(window * kernel_x)
           gy = np.sum(window * kernel_y)
           magnitude = min(255, int((gx**2 + gy**2)**0.5))
           output_array[y-1, x-1] = magnitude
           
   return Image.fromarray(output_array)

if __name__ == '__main__':
   input_image = Image.open("img.jpg").convert("L")
   process_counts = [2, 4, 8, 16, 32]
   speedups = []
   efficiencies = []
   
   start_time = time.time()
   output_image_serial = sobel_filter_serial(input_image)
   serial_time = time.time() - start_time
   output_image_serial.save("sobel_img_serial.jpg")
   print(f"Serial Execution Time: {serial_time} seconds")
   
   for num_processes in process_counts:
       start_time = time.time()
       output_image_parallel = sobel_filter_parallel(input_image, num_processes)
       parallel_time = time.time() - start_time
    #    output_image_serial.save("sobel_img_parallel_" + str(num_processes) + ".jpg")
       
       speedup = serial_time / parallel_time
       efficiency = speedup / num_processes
       
       speedups.append(speedup)
       efficiencies.append(efficiency)
       
       print(f"\nNumber of Processes: {num_processes}")
       print(f"Parallel Time: {parallel_time:.2f} seconds")
       print(f"Speedup: {speedup:.2f}x")
       print(f"Efficiency: {efficiency:.2f}")
   
   plt.figure(figsize=(12, 5))
   
   plt.subplot(1, 2, 1)
   plt.plot(process_counts, speedups, 'b-o')
   plt.xlabel('Number of Processes')
   plt.ylabel('Speedup')
   plt.title('Speedup vs Number of Processes')
   plt.grid(True)
   
   plt.subplot(1, 2, 2)
   plt.plot(process_counts, efficiencies, 'r-o')
   plt.xlabel('Number of Processes')
   plt.ylabel('Efficiency')
   plt.title('Efficiency vs Number of Processes')
   plt.grid(True)
   
   plt.tight_layout()
   plt.savefig('plots.png')