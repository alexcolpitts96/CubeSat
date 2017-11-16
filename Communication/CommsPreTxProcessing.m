% Look at storing images prepped for transmission
% Author: Alex Colpitts
% Date: November 15, 2017

clear
clc

% parameters
image_res = [640 480];
bit_per_pixel = 24;
pn_length = 11; % bits -> barker sequence?

disp(sprintf('Processing Gain: %0.2f dB', 10*log10(pn_length)));

total_pixels = image_res(1) * image_res(2);
data_bits = bit_per_pixel * total_pixels;
data_bytes = data_bits/8;

pn_bits = data_bits * 11;   % roughly 81.1 Mb
pn_bytes = pn_bits/8;       % roughly 10.1 MB

% there will most likely be 2 images so data stored will need to be double
% (1 BW and 1 RGB both with 24 bit depth)







