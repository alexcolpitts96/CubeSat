import os

# get current directory
cwd = os.getcwd()

packet_size = 16

# find the log file
log_dir = cwd + '/comp_logs/'

# see the available log files
logs = os.listdir(log_dir);

#
sat = open(log_dir + logs[0], 'rb')
gnd = open(log_dir + logs[1], 'rb')

# read in the files
sat_file = sat.read()
gnd_file = gnd.read()

# compare lengths, may be longer due to having to send empty bytes in packet
sat_len = len(sat_file)
gnd_len = len(gnd_file)
print('\nDifference in Length: ' + str(abs(sat_len - gnd_len)))
expected = abs((sat_len % packet_size) - packet_size)
print('Expected Difference: ' + str(expected))
print(expected == abs(sat_len - gnd_len))

# compare bytes
match = 0

for i in range(0, sat_len-1):
    print('next')
    print(sat_file[i])
    print(gnd_file[i])
    if sat_file[i] == gnd_file[i]:
        match = match + 1

print('\nMatched Bytes: ' + str(match))
print('Unmatched Bytes: ' + str(sat_len - match))