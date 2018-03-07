# Convert Putty Log to JPEG Image
# Author: Alex Colpitts
# Date: March 5, 2018

# NOTE: User must be part of dialout user group
#       Check using the following: $id -Gn <username>
#       Add using the following: $sudo usermod -a -G dialout <username>
#       User must logout and back in to gain access

import time
from putty2jpeg import *

# convert logs to images every second
while 1:
    p2j(79) # 79 for camera images, 80 for test.jpeg
    time.sleep(1)