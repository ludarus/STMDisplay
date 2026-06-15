#converts a png image to rgb565

from PIL import Image

img = Image.open("vmc logo displayable.png").convert("RGB")
width, height = img.size

print("const uint8_t image[] = {")

for x in range(width):
    for y in range(height):

        r, g, b = img.getpixel((x, y))

        rgb565 = ((r & 0xF8) << 8) | \
                 ((g & 0xFC) << 3) | \
                 (b >> 3)

        print(
            f"0x{rgb565 >> 8:02X}, "
            f"0x{rgb565 & 0xFF:02X},"
        )

print("};")
