% RFM69HCW_Simulation
% Alex Colpitts
% November 20, 2017

clear
clc

%% parameter setup

% channel information
f_c = 915E6; % carrier frequency, Hz

% sequence information 
pn_length = 11;
pn_1 = comm.BarkerCode('Length', pn_length, 'SamplesPerFrame', pn_length);
pn_1 = step(pn_1)';
pn_0 = pn_1 * -1';
% stem(xcorr(pn_seq))

% fsk information
BRF = 300E3; % fsk bitrate, b/s
FDA = 300E3; % frequency deviation, Hz

% make sure frequency deviation is within spec
if (FDA + BRF/2) > 500E3
    disp('Error: FDA too high.')
end

%% creating the transmit frame with PN sequence, CDMA type solution

sample_pixel = [1, 0, 1, 0, 1, 1, 0, 0]; % simulated 8 bit pixel
pixel_size = 8;

tx_frame = zeros(pixel_size, pn_length);

for i = 1:pixel_size
    if sample_pixel(i) == 1
        tx_frame(i, :) = pn_0;    
    else
        tx_frame(i, :) = pn_1;
    end
end


%% adding noise to the channel
noise_power = 20;
tx_frame = tx_frame + wgn(1, length(tx_frame), noise_power, 'dBm');



%% demodulating the signal 
rx_demod = [];
for i = 1:pixel_size
    if max(xcorr(tx_frame(i, :), pn_0)) > (pn_length*0.8) % needs to be at least 80% of the max peak
        rx_demod = [rx_demod 1];
    else
        rx_demod = [rx_demod 0];        
    end    
end
    
% compare to original transmitted message
if (rx_demod == sample_pixel)
    disp('Correct Byte Received')
else
    disp('Incorrect Byte Received')
    
end

disp(sprintf('\nNoise Power Level: %0.2f dBm', noise_power))
    
