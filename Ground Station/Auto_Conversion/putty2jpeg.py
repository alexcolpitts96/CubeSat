import os

def p2j(ignore):
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

        # if the file is a log file
        if FILE.endswith(".log"):

            # skip first 'ignore' bytes
            log = open(log_dir + FILE, 'rb')
            log.seek(ignore)
            remainder = log.read()

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
