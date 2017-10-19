% Calculating Transmission Power Levels
% Date: October 19, 2017
% Author: Alex Colpitts

clear
clc

% system must be able to transmit 400 km to the ground station
D_r = 400E3; % meters
f = 915E6; % transmit frequency
lambda = 3E8/f;

% antennas gains
G_tx = 1.5; % dB, gain from dipole antenna on CubeSat
G_rx = 15; % dB, gain from cross-polarized yagi for ground station



% calculated values
P_tx_dB = 1.5 + 5 + 20; % gain from dipole, trx chip and amplifier
P_rx_dB = P_tx_dB + G_tx + G_rx + 20*log10(lambda / (4 * pi * D_r));

disp(sprintf("Transmit Power from Satellite:    %0.2f(dBm)", P_tx_dB));
disp(sprintf("Power Received at Ground Station: %0.2f(dBm)", P_rx_dB));
disp(sprintf("Attenuation of RF Link:           %0.2f(dB)", P_rx_dB - P_tx_dB));