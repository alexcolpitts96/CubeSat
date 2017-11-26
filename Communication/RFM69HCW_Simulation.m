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

tx_frame = reshape(tx_frame, [numel(tx_frame), 1]);
tx_frame = (tx_frame+1)./2;

%% fsk modulating the signal
M = 2; % binary communication 
FDA = 300E3; % frequency deviation, Hz
BRF = 300E3; % fsk bitrate, b/s

% make sure frequency deviation is within spec
if (FDA + BRF/2) > 500E3
    disp('Error: FDA too high.')
end

nsamp = 8; % spreading factor for fsk
tx_fsk = fskmod(tx_frame, M, FDA, nsamp, 2*BRF);

% for i = 1:pixel_size
%     tx_fsk(i, :) = fskmod((tx_frame(1, :)+1)/2, M, FDA, pn_length, BRF);
% end

%% adding noise to the channel
noise_power = 30;
tx_fsk = tx_fsk + wgn(numel(tx_fsk), 1, noise_power, 'dBm');


%% fsk demodulation

rx_fsk = fskdemod(tx_fsk, M, FDA, nsamp, 2*BRF);
[num_fsk,BER_fsk] = biterr(tx_frame, rx_fsk);
disp(sprintf('\nChannel Noise Power Level: %0.2f dBm', noise_power))
disp(sprintf('\nFSK BER: %0.2f %%', BER_fsk*100))

%% correlating the baseband signal

% strip out one symbol at a time
rx_frame = reshape(rx_fsk, [pixel_size, pn_length]);
rx_frame = (rx_frame*2)-1;

rx_data = zeros(1, pixel_size);

for i = 1:pixel_size
    if max(-xcorr(rx_frame(i, :), pn_1)) > (0.8*pn_length)
        rx_data(i) = 1;
    else
        rx_data(i) = 0;
    end
end

[num_pn,BER_pn] = biterr(sample_pixel, rx_data);
disp(sprintf('\nPN BER: %0.2f %%', BER_pn*100))







