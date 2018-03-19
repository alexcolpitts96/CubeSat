import os
import serial
from _datetime import datetime

# get location and storage directory
cwd = os.getcwd()
storage_dir = cwd + '/storage/'

# create directory if it doesn't exist
dir_exist = os.path.exists(storage_dir)
if not dir_exist:
    os.mkdir(storage_dir)

# connect to serial device
try:
    ser = serial.Serial("/dev/ttyACM0", 115200)
    print("Connected to: " + ser.name)
except serial.SerialException:
    print("Error: Port Failed To Open")
    exit(1)

# open the serial port
ser.open()

# variables for reading in bytes
cur_time = datetime.now().strftime("%Y-%m-%d-%H-%M-%S")
last = 0
cur = 0

# setting up file to write to
output_name = storage_dir + cur_time + ".jpeg"
output = open(output_name, 'wb')

# read until 0xFFD8 is found, start logging to file
while last is not 0xFF and cur is not 0xD8:
    last = cur
    cur = ser.read()

# write out 0xFF 0xD8
output.write(last)
output.write(cur)

# read until 0xFFD9 is found, stop logging to file
while last is not 0xFF and cur is not 0xD9:

    # read in byte
    last = cur
    cur = ser.read()

    # write out byte
    output.write(cur)

# close the file when end has been reached
output.close()

print("Image Captured")
exit(0)