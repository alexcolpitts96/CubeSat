% Calculating Maximum Data Transfer Per Day
% Date: October 19, 2017
% Author: Alex Colpitts

clear
clc

% If a camera is used with the following properties
image_width = [640 1920];
image_height = [480 1080];
image_pixels = image_width .* image_height;
pixel_size = 24; % 24 bit color images
image_bits = image_pixels * pixel_size;

% If a Heimiller Sequences is used...
p = 11;
processing_gain = 10*log10(p^2);

% Making the assumption that a RFM22B Tx/Rx Chip is used for this
% calculation.
% data_rate = 1E6; % bit/s
data_rate = 300E3;

orbit_time = 92; % minutes
overhead_time = 10; % minutes
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
disp(sprintf("\nMax Image Size (10 Minutes): %d (Bits)", max_image_size))
disp(sprintf("\nMinimum Works?: %d", (max_image_size>image_bits(1))))
disp(sprintf("Ideal Works?: %d", (max_image_size>image_bits(2))))
