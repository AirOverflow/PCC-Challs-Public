from PIL import Image

def extract_green_channel(image_path):
    img = Image.open(image_path)
    if img.mode != 'RGB':
        img = img.convert('RGB')
    width, height = img.size
    green_values = []
    for y in range(height):
        for x in range(width):
            r, g, b = img.getpixel((x, y))
            green_values.append(g)
    binary_green_values = ''.join(format(g, '08b') for g in green_values)
    
    return binary_green_values
image_path = '../dist/8gz.png'
binary_green_data = extract_green_channel(image_path)
print(binary_green_data)
