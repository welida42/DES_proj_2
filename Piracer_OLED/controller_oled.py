import time
import board
import busio
import adafruit_ssd1306
from PIL import Image, ImageDraw, ImageFont
import socket
from piracer.vehicles import PiRacerStandard
from piracer.gamepads import ShanWanGamepad

# Declared OLED
WIDTH = 128
HEIGHT = 32
i2c = busio.I2C(board.SCL, board.SDA)
oled = adafruit_ssd1306.SSD1306_I2C(WIDTH, HEIGHT, i2c)

# Get IP Address
def get_ip():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        # doesn't even have to be reachable
        s.connect(('10.255.255.255', 1))
        IP = s.getsockname()[0]
    except:
        IP = '127.0.0.1'
    finally:
        s.close()
    return IP

# Display Battery Level and IP Address in OLED
def display_info(ip, battery):
    # Clear the display
    oled.fill(0)
    oled.show()

    # Create blank image for drawing
    image = Image.new("1", (oled.width, oled.height))
    draw = ImageDraw.Draw(image)

    # Load default font
    font = ImageFont.load_default()

    # Display IP address and battery level
    draw.text((0, 0), ip, font=font, fill=255)
    draw.text((0, 10), f"Battery: {str(round(battery, 1))}%", font=font, fill=255)
 
    oled.image(image)
    oled.show()

if __name__ == '__main__':
    piracer = PiRacerStandard()
    shanwan_gamepad = ShanWanGamepad()

    try:
        while True:
            # Control Piracer using Controller
            gamepad_input = shanwan_gamepad.read_data()
            throttle = gamepad_input.analog_stick_left.y
            steering = gamepad_input.analog_stick_right.y

            if abs(throttle) == 0 and abs(steering) == 0:
                # Input X
                ip = get_ip()
                battery = (((piracer.get_battery_voltage() / 3) - 2.5) / 1.7) * 100
                display_info(ip, battery)
            else:
                # Input O
                piracer.set_throttle_percent(throttle * 0.5)
                piracer.set_steering_percent(steering)
            
            print(f'throttle: {throttle}, steering: {steering}')
   
            # Delay
            #time.sleep(0.1)

    except KeyboardInterrupt:
        # Ctrl+C 
        piracer.set_throttle_percent(0)
        piracer.set_steering_percent(0)

