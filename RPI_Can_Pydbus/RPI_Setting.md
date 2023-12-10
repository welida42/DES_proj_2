# Setting Raspberry Pi 

### Using
###### - Waveshare_JetRacer (PiRacer_Standard AI Kit)
###### - Raspberry Pi 4B
###### - WaveShare_2-CH CAN FD HAT
###### - WaveShare_7.9inch DSI LCD

## Connect Rpi with CAN FD HAT
  1. Enable I2C interface
  2. Install libraries
  
  ```bash
  #bcm2835

  wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.60.tar.gz
  tar zxvf bcm2835-1.60.tar.gz 
  cd bcm2835-1.60/
  sudo ./configure
  sudo make
  sudo make check
  sudo make install
  # For more information, please refer to the official website: http://www.airspayce.com/mikem/bcm2835/
  ```

  ```bash
  #Wiring Pi

  sudo apt-get install wiringpi
  #For Raspberry Pi 4B may need to be upgraded:
  wget https://project-downloads.drogon.net/wiringpi-latest.deb
  sudo dpkg -i wiringpi-latest.deb
  gpio -v
  # Run gpio -v and version 2.52 will appear. If it does not appear, the installation is wrong
  ```

  ```bash
  #Python Libraries

  sudo apt-get update
  sudo apt-get install python3-pip
  sudo apt-get install python3-pil
  sudo apt-get install python3-numpy
  sudo pip3 install RPi.GPIO
  sudo pip3 install spidev 
  sudo pip3 install python-can
  ```

  3. Configure CAN
  ```bash
  sudo ip link set can0 up type can bitrate 1000000 dbitrate 8000000 restart-ms 1000 berr-reporting on fd on
  sudo ip link set can1 up type can bitrate 1000000 dbitrate 8000000 restart-ms 1000 berr-reporting on fd on
  ```

  ### Reference
  [WaveShare_2-CH CAN FD HAT](https://www.waveshare.com/wiki/2-CH_CAN_FD_HAT)
  
## Connect Rpi with DSI LCD

### S/W Setting
  1. Install Drive
  ```bash
  git clone https://github.com/waveshare/Waveshare-DSI-LCD
  cd Waveshare-DSI-LCD
  ``` 
  2. Enter uname -a in the terminal to view the kernel version and cd to the corresponding file directory
  #5.15.84 then run the following command
  ```bash
  cd 5.15.84
  ```
  3. Please check the bits of your system, enter the 32 directory for 32-bit systems, and enter the 64 directory for 64-bit systems
  ```bash
  cd 64
  ```
  4. Enter your corresponding model command to install the driver, pay attention to the selection of the I2C DIP switch
#7.9inch DSI LCD 400×1280 driver：
  ```bash
  sudo bash ./WS_xinchDSI_MAIN.sh 79 I2C0
  ``` 
  5. When the driver installation is complete and no error is prompted, restart and load the DSI driver and it can be used normally
  ```bash
  sudo reboot
  ```
### Rotation
In the start menu, select: Preferences->Screen Configuration->Configure->Screens->DSI-1->Orientation, select the corresponding angle in it, click "√", select Yes, and restart.

### Touch to rotate
```bash
sudo nano /boot/config.txt
```
#90°：  <br/>
dtoverlay=WS_xinchDSI_Touch,invertedy,swappedxy  <br/>
#180°：  <br/>
dtoverlay=WS_xinchDSI_Touch  <br/>
#270°：  <br/>
dtoverlay=WS_xinchDSI_Touch,invertedx,swappedxy  <br/>
#0°：  <br/>
dtoverlay=WS_xinchDSI_Touch,invertedx,invertedy  <br/>

#### If the LCD does not rotate even after the above method, or if the power is turned off and on, it becomes initialized
  1. Install xrandr 
  ```bash
  sudo apt-get install x11-xserver-utils
  ```

  2. Open file "./bashrc" 
  ```bash
  sudo vim .bashrc
  ```
  3. Write below command bottom of the file
  ```bash
  xrandr --output DSI-1 --rotate (left/right/normal/inverted)
  ```

### Reference
[WaveShare_7.9inch DSI LCD](https://www.waveshare.com/wiki/7.9inch_DSI_LCD)

## Setting Arduino for CAN connection with Rpi

### Circuit Diagram

<img src = "https://github.com/Ho-mmd/DES_Project2/assets/55338823/2ebd3967-d65f-4534-bec2-42016171c267" width = 70% height = 70%>

### Simple Description of the code
###### - can_data_reciver.py: Receive CAN Data using python

## Setting RPI for Dbus between Python and Qt

### DBus
##### D-Bus is a system for IPC (InterProcess Communication). It has layers called libdbus, message bus daemon executable and Wrapper libraries. It is open source software, widely used ad interprocess communication for Linux desktops.

### Simple Description of the code
###### - Dbus_test_without_piracer.py: Send data to Qt using DBus without CAN
###### - gamepad.py: Receive CAN Data 

## Get speed and battery data and Reduce theirs noise
1. [Get Speed Data from Arduino Using CAN](https://github.com/Ho-mmd/DES_Project2/blob/main/Arduino/Arduino_Setting.md)
2. [Get Battery Data from Pi-racer](https://github.com/Ho-mmd/DES_Project2/blob/main/Arduino/Arduino_Setting.md)
3. Using Bessel Filter for Speed and MA(Moving Average) Filter for Battery to reduce noise

### Bessel Filter
Filters constructed from the Bessel function to obtain uniform time delay independent of frequency. It's a analog linear filter shows linear response characteristics over a wide frequency range.

### MA Filter
Moving average filter is a technique of sequentially filtering data by obtaining an average of number of adjacent data from continuous data. When new data comes in, the oldest data is discarded.

### Compare Data between raw and filtered
<img src = "https://github.com/Ho-mmd/DES_Project2/assets/55338823/06043239-4373-479a-b7b9-d1376b813cea" width = 47.5%>
<img src = "https://github.com/Ho-mmd/DES_Project2/assets/55338823/cf57114b-3b22-4adb-80b7-897fd0509108" width = 45%>

### Simple Description of the code 
###### - chk_freq_graph.py: Plot raw speed and filtered speed to show the graph

## Merge All Process

### Simple Description of the code 
###### - can_recv_Pydbus.py: Receive Can Data and Send it to Qt using DBus
###### - gamepad.py: Connect Controller with Pi-racer
