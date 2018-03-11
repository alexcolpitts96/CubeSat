import os
from shutil import copy2

def p2j():
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

    # create a directory called backup_logs if it doesn't exist ---------------------------
    backup_dir = cwd + "/backup_logs/"
    dir_exist = os.path.exists(backup_dir)

    if not dir_exist:
        os.mkdir(backup_dir)

    # check if any log files exist ---------------------------------------------------------
    for FILE in os.listdir(log_dir):

        # if the file is a log file
        if FILE.endswith(".log"):

            # copy log to backup folder
            copy2(log_dir + FILE, backup_dir)

            # find jpeg start and end
            log = open(log_dir + FILE, 'rb')
            bytes = log.read()
            start = bytes.find(b'\xFF\xD8')
            end = bytes.find(b'\xFF\xD9') + 1

            remainder = bytes[start:end]

            # store image in image_dir as .jpeg
            temp = FILE[:].replace('.log', '.jpeg')
            output = image_dir + temp
            image = open(output, 'wb')
            image.write(remainder)

            # delete log file
            os.remove(log_dir + FILE)

        else:
            # delete log file
            os.remove(log_dir + FILE)
