# Convert Putty Log to JPEG Image
# Author: Alex Colpitts
# Date: March 5, 2018

import os

# get current working directory (cwd)
cwd = os.getcwd()

# create log directory if not already in existence -------------------------------------
log_dir = cwd + "/logs/"
dir_exist = os.path.exists(log_dir)

if not dir_exist:
    os.mkdir(log_dir)

# create a directory called images is it doesn't already exist -------------------------
image_dir = cwd + "/images/"
dir_exist = os.path.exists(image_dir)

if not dir_exist:
    os.mkdir(image_dir)

# check if any log files exist ---------------------------------------------------------
for FILE in os.listdir(log_dir):
    if FILE.endswith(".log"):
        # call conversion function