import can
import struct
from pydbus import SessionBus
import time
import asyncio
from scipy.signal import bessel, lfilter
from piracer.vehicles import PiRacerStandard
from piracer.gamepads import ShanWanGamepad

bus = SessionBus()

DBUS_INTERFACE = """
<node>
    <interface name="com.example.Chkout">
        <method name="handleError">
        </method>
    </interface>
</node>
"""

class DbusData:
    bus = DBUS_INTERFACE
    
    def handleError(self):    
        start_time = time.time()
        while (1):
            if(time.time() - start_time <= 1):
                print("Trying to reconnect...")
                try:
                    bus.get("com.example.CanData", "/com/example/CanData/Data")
                    print("Reconnected successfully!")
                    return
                except Exception as e:
                    print("Connection attempt failed!")
                    time.sleep(0.1)
            else:
                sys.exit(1)

    def __init__(self):
        start_time = time.time()
        while(time.time() - start_time < 10):
            try:
                self._dbus = bus.get("com.example.CanData", "/com/example/CanData/Data")
                break
            except Exception as e:
                print("Trying to Connect!!")
                time.sleep(0.5)

        self._current_speed = -1
        self._current_rpm = 0
        self._current_battery = 100

    def update(self, speed, rpm, battery):
        self._current_speed = speed
        self._current_rpm = rpm
        self._current_battery = battery

        print(f"Sending RPM: {self._current_rpm}, Speed: {self._current_speed}, Battery: {self._current_battery}, Throttle: {self._current_throttle}, Gear: {self._current_gear}")
        
        self._dbus.setData(speed, rpm, battery)

def receive_can_data(dbus_data):
    can_bus = can.interface.Bus(channel='can0', bustype='socketcan')
   
    zi_speed = [0.0] * order

    loop = asyncio.new_event_loop()  # Create a new event loop

    while True:
        message = loop.run_until_complete(asyncio.to_thread(can_bus.recv))  # Use the loop to run the asyncio.to_thread call
        
        if message is not None:
            data = message.data
            rpm, speed = struct.unpack('<ff', data)
            
            # Apply Bessel filter only to speed
            speed, zi_speed = bessel_filter([speed], zi_speed)

            battery_percentage = ((((piracer.get_battery_voltage() / 3) - 2.5) / 1.7) * 100)
            dbus_data.update(300 * speed, rpm, battery_percentage)
        dbus_data.update(dbus_data._current_speed + 10, dbus_data._current_rpm + 1, 
                         dbus_data._current_battery, dbus_data._current_gear)
        

dbus_data = DbusData()
receive_can_data(dbus_data)
