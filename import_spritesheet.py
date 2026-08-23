from PIL import Image
import os

png_path = 'spritesheet.png'
icons_path = 'source/icons.c'

if not os.path.exists(png_path):
    print(f"Error: {png_path} not found!")
    exit(1)

im = Image.open(png_path).convert('RGBA')
w, h = im.size
print(f"Reading {png_path} ({w}x{h})...")

pixels = []
for y in range(h):
    for x in range(w):
        r, g, b, a = im.getpixel((x, y))
        if a < 50:
            idx = 0
        else:
            lum = 0.299 * r + 0.587 * g + 0.114 * b
            if lum < 80:
                idx = 1
            elif lum < 180:
                idx = 2
            else:
                idx = 3
        pixels.append(idx)

with open(icons_path, 'w') as f:
    f.write('#include "gfx/spritesheet.h"\n#define t (char)\n\nstatic char buf[] = {\n')
    chunk_size = 32
    for i in range(0, len(pixels), chunk_size):
        chunk = ','.join('t ' + str(v) for v in pixels[i:i+chunk_size])
        if i + chunk_size < len(pixels):
            f.write(chunk + ',\n')
        else:
            f.write(chunk + '\n')
    f.write('};\n\nconst SpriteSheet icons_spritesheet = {\n\t256,\n\t512,\n\tbuf\n};\n')

print(f"Successfully converted {png_path} -> {icons_path} ({len(pixels)} pixels)!")
