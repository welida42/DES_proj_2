import can
import struct
from pydbus import SessionBus
import asyncio
import time
import numpy as np
from scipy.signal import bessel, lfilter
from piracer.vehicles import PiRacerStandard
from piracer.gamepads import ShanWanGamepad

DBUS_INTERFACE = """
<node>
    <interface name="com.example.Chkout">
        <method name="handleError"/>
    </interface>
</node>
"""

bus = SessionBus()
piracer = PiRacerStandard()
shanwan_gamepad = ShanWanGamepad()

order = 3
b, a = bessel(order, 0.1)

def bessel_filter(data, zi=None):
    if zi is not None:
        y, zf = lfilter(b, a, data, zi=zi)
        return y[0], zf
    else:
        return lfilter(b, a, data)
        
class DbusChkout:
    """
    A class to interact with D-Bus and represent the Chkout interface.
    """
    def handleError(self):
        print("Now Handling error is called")
        start_time = time.time()
        while time.time() - start_time <10:
            try:
                self._dbus = bus.get("com.example.CanData", "/com/example/CanData/Data")
                print("Reconneted successfully!")
                break
            except Exception as e:
                print("Trying to reconnect...")
                time.sleep(0.5)
        else:
            print("Failed to reconnect after multiple attempts.")

class DbusData:
    """A class to interact with D-Bus and represent data."""
    bus = DBUS_INTERFACE

    def __init__(self):
        # Get D-Bus object
        start_time = time.time()
        while(time.time() - start_time < 10):
            try:
                self._dbus = bus.get("com.example.CanData", "/com/example/CanData/Data")
                break
            except Exception as e:
                print("Trying to Connect!!")
                time.sleep(0.5)
        
        self._current_speed = 0.0
        self._current_rpm = 0.0
        self._current_battery = ((((piracer.get_battery_voltage() / 3) - 3.1) / 1.1) * 100)
        self.battery_values = [self._current_battery] * 30
        self._current_throttle = -0.038
        self._current_gear = "OFF"


    def update(self, speed, rpm, battery, gear):
        self._current_speed = speed
        self._current_rpm = rpm
        self.battery_values.append(battery)

        if len(self.battery_values) > 30:
            self.battery_values.pop(0)
        
        battery = self.moving_average(self.battery_values)
        self._current_gear = gear
        print(f"Received RPM: {self._current_rpm}, Speed: {self._current_speed} Battery: {self._current_battery}, Gear: {self._current_gear}")
        self._dbus.setData(speed, rpm, battery, gear)
    
    def moving_average(self, values):
        return np.mean(values)

def receive_can_data(dbus_data):
    can_bus = can.interface.Bus(channel='can0', bustype='socketcan')
    
    zi_speed = [0.0] * order

    loop = asyncio.new_event_loop()  # Create a new event loop

    while True:

        message = loop.run_until_complete(asyncio.to_thread(can_bus.recv))
        if message is not None:
            data = message.data
            rpm, speed = struct.unpack('<ff', data)
            
            # Apply Bessel filter only to speed
            speed, zi_speed = bessel_filter([speed], zi_speed)
            
            if(speed < 0):
                speed = 0

            throttle = 1 
  
            # Continue with the rest of the logic
            if(speed == 0 or rpm == 0):
                gear = "P"
            elif(speed > 0 and throttle > -0.038):
                gear = "D"
            elif(speed > 0 and throttle <= -0.038):
                gear = "R"
            else:
                gear = "OFF"
            
            battery_percentage = ((((piracer.get_battery_voltage() / 3) - 3.1) / 1.1) * 100)

            dbus_data.update(300 * speed, rpm, battery_percentage, gear)


dbus_data = DbusData()
dbus_chkout = DbusChkout()
obj = bus.register_object("/com/example/CanData/Chkout", dbus_chkout, DBUS_INTERFACE)
receive_can_data(dbus_data)
