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
pn_seq = comm.BarkerCode;
pn_seq.Length = pn_length;
pn_seq.SamplesPerFrame = pn_length;
pn_seq = step(pn_seq)';
% stem(xcorr(pn_seq))

% fsk information
BRF = 300E3; % fsk bitrate, b/s
FDA = 300E3; % frequency deviation, Hz

% make sure frequency deviation is within spec
if (FDA + BRF/2) > 500E3
    disp('Error: FDA too high.')
end

%% creating the transmit frame with PN sequence

sample_pixel = [1, 0, 1, 0, 1, 1, 0, 0]; % simulated 8 bit pixel
tx_frame = zeros(1, length(sample_pixel)*pn_length);

for i = 1:length(sample_pixel)
    
end
    
    
    
    
    
    
    
    
    
