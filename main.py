import PIL.ImageGrab
from time import sleep
import ctypes
import keyboard

WIDTH, HEIGHT = PIL.ImageGrab.grab().size

def click():
    ctypes.windll.user32.mouse_event(0x2, 0,0,0,0)
    ctypes.windll.user32.mouse_event(0x4, 0,0,0,0)

def get_pixel(x, y):
    return PIL.ImageGrab.grab().load()[x, y]

if __name__ == "__main__":
    #offset_x, offset_y = map(int, input("Offsets from center: ").split())
    offset_x, offset_y = 2, 2
    #firing_difference = int(input("Firing difference threshold: "))
    firing_difference = 20
    delay = 0
    #delay = float(input("Firing delay after detection: "))

    while True:
        keyboard.wait("c")
        current_sum = sum(get_pixel(WIDTH // 2 + offset_x, HEIGHT // 2 + offset_y))
        while abs(current_sum - sum(get_pixel(WIDTH // 2 + offset_x, HEIGHT // 2 + offset_y))) < firing_difference: pass
        sleep(delay)
        click()
