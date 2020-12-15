
from time import sleep
from machine import Pin
led = Pin(16, Pin.OUT)
for _ in range(10):
    
    led.value(0)
    sleep(1)
    led.value(1)
    sleep(1)