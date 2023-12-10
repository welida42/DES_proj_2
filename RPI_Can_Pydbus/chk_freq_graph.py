import can
import struct
import asyncio
import time
from scipy.signal import bessel, lfilter
from scipy.fftpack import fft
import numpy as np
import matplotlib.pyplot as plt
from piracer.vehicles import PiRacerStandard
from piracer.gamepads import ShanWanGamepad

piracer = PiRacerStandard()
shanwan_gamepad = ShanWanGamepad()

order = 3
b, a = bessel(order, 0.1)
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

class DbusData:
    def __init__(self):
        self._current_speed = 0.0
        self._current_rpm = 0.0
        self._current_battery = 0
        self._current_throttle = -0.038
        self._current_gear = "OFF"

    def update(self, speed, rpm, battery, gear):
        self._current_speed = speed
        self._current_rpm = rpm
        self._current_battery = battery
        self._current_gear = gear
        print(f"Received RPM: {self._current_rpm}, Speed: {self._current_speed} Battery: {self._current_battery}, Gear: {self._current_gear}")

def plot_time_response(raw, filtered):
    plt.figure()
    plt.plot(raw, label="Raw Speed")
    plt.plot(filtered, label="Filtered Speed")
    plt.legend()
    plt.title("Time Response")
    plt.xlabel("Time")
    plt.ylabel("Speed")
    plt.show()

def plot_frequency_response(signal, fs):
    N = len(signal)
    yf = fft(signal)
    xf = np.linspace(0.0, fs/2.0, N//2)
    plt.figure()
    plt.plot(xf, 2.0/N * np.abs(yf[0:N//2]))
    plt.title("Frequency Response")
    plt.xlabel("Frequency (Hz)")
    plt.ylabel("Amplitude")
    plt.grid()
    plt.show()

def plot_results(raw, filtered, fs):
    fig, axs = plt.subplots(2, 2, figsize=(14,10))
    
    # Time Response
    axs[0, 0].plot(raw, label="Raw Speed")
    axs[0, 0].plot(filtered, label="Filtered Speed")
    axs[0, 0].legend()
    axs[0, 0].set_title("Time Response")
    axs[0, 0].set_xlabel("Time")
    axs[0, 0].set_ylabel("Speed")
    
    # Frequency Response for Raw Signal
    N = len(raw)
    yf_raw = fft(raw)
    xf_raw = np.linspace(0.0, fs/2.0, N//2)
    axs[0, 1].plot(xf_raw, 2.0/N * np.abs(yf_raw[0:N//2]))
    axs[0, 1].set_title("Frequency Response (Raw)")
    axs[0, 1].set_xlabel("Frequency (Hz)")
    axs[0, 1].set_ylabel("Amplitude")

    # Frequency Response for Filtered Signal
    N = len(filtered)
    yf_filtered = fft(filtered)
    xf_filtered = np.linspace(0.0, fs/2.0, N//2)
    axs[1, 0].plot(xf_filtered, 2.0/N * np.abs(yf_filtered[0:N//2]))
    axs[1, 0].set_title("Frequency Response (Filtered)")
    axs[1, 0].set_xlabel("Frequency (Hz)")
    axs[1, 0].set_ylabel("Amplitude")

    plt.tight_layout()
    plt.show()

def receive_can_data(dbus_data):
    can_bus = can.interface.Bus(channel='can0', bustype='socketcan')

    zi_speed = [0.0] * order
    raw_speeds = []
    filtered_speeds = []

    loop = asyncio.new_event_loop()  # Create a new event loop
    start_time = time.time()

    while time.time() - start_time < 60:  # Collect data for 1 minute
        message = loop.run_until_complete(asyncio.to_thread(can_bus.recv))
        if message is not None:
            data = message.data
            rpm, speed = struct.unpack('<ff', data)

            # Store the raw speed
            raw_speeds.append(speed)
            
            # Apply Bessel filter only to speed
            speed, zi_speed = bessel_filter([speed], zi_speed)

            # Store the filtered speed
            filtered_speeds.append(speed)

            if(speed < 0):
                speed = 0

            throttle = 1 
            if(speed == 0 or rpm == 0):
                gear = "P"
            elif(speed > 0 and throttle > -0.038):
                gear = "D"
            elif(speed > 0 and throttle <= -0.038):
                gear = "R"
            else:
                gear = "OFF"

            battery_percentage = ((((piracer.get_battery_voltage() / 3) - 2.5) / 1.7) * 100)
            dbus_data.update(300 * speed, rpm, battery_percentage, gear)

    # After collecting data for 1 minute, plot the results
    plot_time_response(raw_speeds, filtered_speeds)
    plot_frequency_response(raw_speeds, 1)  # Assuming a sampling frequency of 1 Hz for simplicity
    plot_frequency_response(filtered_speeds, 1)
    plot_results(raw_speeds, filtered_speeds, 1)

dbus_data = DbusData()
receive_can_data(dbus_data)
