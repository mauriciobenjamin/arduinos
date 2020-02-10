import board
import time
from adafruit_onewire.bus import OneWireBus
from adafruit_ds18x20 import DS18X20
import digitalio

bus = OneWireBus(board.D5)
sensor = DS18X20(bus, bus.scan()[0])

rele = digitalio.DigitalInOut(board.D6)
rele.direction = digitalio.Direction.OUTPUT

while True:
    t = sensor.temperature
    print('Temperatura: {0:0.3f} °C'.format(t))
    
    if t < 60:
        rele.value = False
    else:
        rele.value = True
        
    time.sleep(1.0)

