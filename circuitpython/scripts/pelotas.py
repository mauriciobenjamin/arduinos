import board
import busio
import adafruit_ssd1306

i2c = busio.I2C(board.SCL, board.SDA)

oled = adafruit_ssd1306.SSD1306_I2C(128, 64, i2c)

def draw_circle(xpos0, ypos0, rad, col=1):
    x = rad - 1
    y = 0
    dx = 1
    dy = 1
    err = dx - (rad << 1)
    while x >= y:
        oled.pixel(xpos0 + x, ypos0 + y, col)
        oled.pixel(xpos0 + y, ypos0 + x, col)
        oled.pixel(xpos0 - y, ypos0 + x, col)
        oled.pixel(xpos0 - x, ypos0 + y, col)
        oled.pixel(xpos0 - x, ypos0 - y, col)
        oled.pixel(xpos0 - y, ypos0 - x, col)
        oled.pixel(xpos0 + y, ypos0 - x, col)
        oled.pixel(xpos0 + x, ypos0 - y, col)
        if err <= 0:
            y += 1
            err += dy
            dy += 2
        if err > 0:
            x -= 1
            dx += 2
            err += dx - (rad << 1)

class Pelota:
    def __init__(self, pos_x, pos_y, radio, dx, dy):
        self.pos_x = pos_x
        self.pos_y = pos_y
        self.radio = radio
        self.dx = dx
        self.dy = dy
    def mov(self, v):
        draw_circle(self.pos_x, self.pos_y, self.radio, col=0)
        if self.pos_x + self.radio >= oled.width or self.pos_x - self.radio < 0:
            self.dx = -self.dx
        if self.pos_y + self.radio >= oled.height or self.pos_y - self.radio < 0:
            self.dy = -self.dy
        self.pos_x = self.pos_x + v * self.dx
        self.pos_y = self.pos_y + v * self.dy
        draw_circle(self.pos_x, self.pos_y, self.radio, col=1)

pelota1 = Pelota(15, 15, 7, 1, 1)
pelota2 = Pelota(30, 32, 8, 1, -1)
pelota3 = Pelota(22, 21, 4, -1, -1)
pelota4 = Pelota(90, 50, 6, -1, 1)
pelota5 = Pelota(80, 20, 5, 0, 1)
pelota6 = Pelota(70, 30, 6, 2, 1)

while True:
    pelota1.mov(2)
    pelota2.mov(3)
    pelota3.mov(4)
    pelota4.mov(1)
    pelota5.mov(4)
    pelota6.mov(3)
    oled.show()
