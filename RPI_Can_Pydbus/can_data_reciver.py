import struct
import can

def convert_hex_to_float(hex_val):
	return struct.unpack('<f', bytes.fromhex(hex_val))[0]

def receive_can_msg(channel):
	bus = can.interface.Bus(channel=channel, bustype='socketcan')

	while True:
		message = bus.recv()  #blocking receive
		if message is not None:
			data_hex = message.data.hex()
			RPM = convert_hex_to_float(data_hex[0:8])
			speed = convert_hex_to_float(data_hex[8:16])
			print("RPM : " , round(RPM,2))
			print("Speed : ", round(speed, 1))
receive_can_msg('can0')


