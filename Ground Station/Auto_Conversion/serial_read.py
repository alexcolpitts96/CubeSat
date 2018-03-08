import serial
import os
from datetime import datetime

def readImage():

    # open the serial port
    k22 = serial.Serial('/dev/ttyACM0', 115200)

    # get current time for file name
    time = datetime.now().strftime('%Y-%m-%d-%H-%M-%S')

    # read into these variables from serial port
    byte_last = 0
    byte_cur = 0

    # location to store files
    cwd = os.getcwd()
    image_dir = cwd + "/images/"

    image = open(image_dir + time + '.jpeg', 'wb')

    # run the loop until end of jpeg discovered
    while (byte_last is not b'0xFF') and (byte_cur is not b'0xD9'):

        # run the loop until the start of an image is found
        while (byte_last is not b'0xFF') and (byte_cur is not b'0xD8'):

            # read in bytes
            byte_last = byte_cur
            byte_cur = k22.read()

        # once the start of JPEG has been found start writing to file
        image.write(byte_last)

        byte_last = byte_cur
        byte_cur = k22.read()

        # if (byte_last is b'0xFF') and (byte_cur is b'0xD9'):












    # close the console when done
    k22.close()



