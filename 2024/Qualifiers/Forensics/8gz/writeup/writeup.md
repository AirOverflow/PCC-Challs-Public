# Solution 
**Starting from the description provided, we can see there is something hidden in "G" of RGB basically. The 0 refers to the binary. So lets start**
	- Using python/pillow we can extract the value of RGB, so specifically extracting G is pretty easy. 
```
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
```
**This gives a suspecious binary:**
`01010000010000110100001100110010001101000111101100110101010101000011001101000111001101000100111000110000010001110101001000110100010100000100100001011001010111110011000100110101010111110100111000110000010101000101111101000110001100000101001000110011010011100011010100110001010000110101001101111101`
**That gives us the flag**
```
PCC24{5T3G4N0GR4PHY_15_N0T_F0R3N51CS}
```
