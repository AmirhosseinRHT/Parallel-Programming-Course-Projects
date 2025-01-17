from PIL import Image

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

def apply_kernel(pixels, kernel, width, height):
    new_pixels = []
    for y in range(1, height - 1):
        for x in range(1, width - 1):
            gx = (
                kernel[0][0] * pixels[(y-1)*width + (x-1)] +
                kernel[0][1] * pixels[(y-1)*width + x] +
                kernel[0][2] * pixels[(y-1)*width + (x+1)] +
                kernel[1][0] * pixels[y*width + (x-1)] +
                kernel[1][1] * pixels[y*width + x] +
                kernel[1][2] * pixels[y*width + (x+1)] +
                kernel[2][0] * pixels[(y+1)*width + (x-1)] +
                kernel[2][1] * pixels[(y+1)*width + x] +
                kernel[2][2] * pixels[(y+1)*width + (x+1)]
            )
            new_pixels.append(int(abs(gx)))
    return new_pixels

def sobel_filter(image):
    width, height = image.size
    pixels = list(image.getdata())
    gx = apply_kernel(pixels, sobel_x_kernel, width, height)
    gy = apply_kernel(pixels, sobel_y_kernel, width, height)
    gradient_magnitude = [min(255, int((gx[i]**2 + gy[i]**2)**0.5)) for i in range(len(gx))]
    new_image = Image.new("L", (width - 2, height - 2))
    new_image.putdata(gradient_magnitude)
    return new_image

input_image = Image.open("img.jpg").convert("L")
output_image = sobel_filter(input_image)
output_image.save("sobel_img.jpg")
