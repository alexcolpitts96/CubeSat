% Calculating Maximum Data Transfer Per Day
% Date: October 15, 2017
% Author: Alex Colpitts

clear
clc

% If a camera is used with the following properties
image_width = 780;
image_height = 440;
image_pixels = image_width * image_height;
pixel_size = 24; % 24 bit color images
image_bits = image_pixels * pixel_size;

% If a Heimiller Sequences is used...
p = 11;
processing_gain = 10*log10(p^2);

% Making the assumption that a RFM22B Tx/Rx Chip is used for this
% calculation.
data_rate = 256E3; % bit/s

orbit_time = 92; % minutes
overhead_time = [13 20]; % minutes
dark_time = orbit_time - overhead_time; % minutes
orbit_per_day = 15.5;

bit_per_day = (overhead_time*60) * data_rate * orbit_per_day;

% If we repeat the image 3 times
n = 3;
n_repeat_tx_BPD = bit_per_day/n;

% If we account for a PN sequence that is p^2 chips long
pn_BPD = n_repeat_tx_BPD/p^2;

% maximum image size is thus
max_image_size = pn_BPD;

% displaying results
disp(sprintf("\nImage Size: %d (Bits)", image_bits))
disp(sprintf("\nMax Image Size (13 Minutes): %d (Bits)", max_image_size(1)))
disp(sprintf("\nMax Image Size (20 Minutes): %d (Bits)", max_image_size(2)))
disp(sprintf("\n13 Minutes Works?: %d", (max_image_size(1)>image_bits)))
disp(sprintf("\n20 Minutes Works?: %d", (max_image_size(2)>image_bits)))
