% Calculating Transmission Power Levels
% Date: October 19, 2017
% Author: Alex Colpitts

clear
clc

figure_count = 1;
%% system parameters
BW = 2E6; % approximate bandwidth, Hz, BW = 2*(max_data_rate), ideally 1 Mbps
r = 400E3; % meters, altitude
f = [146E6 440E6 915E6 2450E6]; % f_c, amateur, amateur ISM ISM 
lambda = 3E8./f;

den = 1.2922; % density of air at 273 K 
P = 101300.0; % pressure in pascal
T = 300; % temperature in kelvin, assume pretty warm

% antennas gains
G_tx = 1.5; % dB, gain from dipole antenna on CubeSat
G_rx = 15; % dB, gain from cross-polarized yagi for ground station

% chip currently with best weighted means score...
P_tx = 13; % dBm

% contants
r_earth = 6371E3; %m
k_b = 1.38064852E-23; % boltzmann's constant

%% noise floor calculations

Noise_Floor = k_b * T * BW;
NF_dB = 10*log10(Noise_Floor);


%% path loss
view_angles = [10 170]; % Popescu Design Paper

% distances were changed to km from meters 
max_distance = sqrt(((r_earth + r)./1000).^2. - (r_earth ./ 1000 .* cosd(view_angles)).^2) - r_earth./1000.*sind(view_angles); 
max_distance = mean(max_distance)*1000; % converting distance back to meters

sat_distances = max_distance:-1:r; 
path_loss = -1 .* fspl(sat_distances,lambda); % free space path loss in dB

max_vs_min_loss = max(path_loss) - min(path_loss);

figure(figure_count)
plot(sat_distances, path_loss)
xlabel('distance to satellite (m)')
ylabel('free space path loss (dB)')
title('Free Space Path Loss For Various Frequencies and Distances')
legend 146MHz 440MHz 915MHz 2.45GHz

%% atmospheric attenuation approximation
% gaspl uses ITU model for gas attenuation, only works from 1-1000 GHz so
% the model will be plotted and then extrapolated down to 915 MHz
f_test = (1:0.01:2)*1E9;
t_celcius = T - 273;

atmos_attn = gaspl(r, f_test, t_celcius, P, den);

figure_count = figure_count +1;
figure(figure_count)
figure_count = figure_count + 1;

semilogy(f_test/1e9,atmos_attn)
xlabel('Frequency (GHz)')
ylabel('Specific Attenuation (dB)')
title('Atmospheric Gas Loss For Various Frequencies')

% Using Matlab curve fitting the atmospheric attenuation curve for lower
% frequencies was observed to have the following equation based on a cubic
% fit:
% powers for the cubic functions
p1 = 0.32056;
p2 = -1.9152;
p3 = 4.1511;
p4 = -0.11293;
% function must be handed the frequency as f/1e9
attn_gas_dB = @(x) (p1*x^3 + p2*x^2 + p3*x + p4);

attn_915 = attn_gas_dB(915E6/1E9); % 2.33 dB attenuation at 915 MHz
% very small attenuation change between having water vapor density at 0
% (dry air) and 15 (lots of moisture)

%% Total attenuation and Antenna Gains
% atmospheric gas losses and free space path losses
total_attn = attn_915 + [-max(path_loss(:,3)), -min(path_loss(:,3))]; 

% transmit dipole on cubesat, cross polarized yagi on earth 
total_gain = G_rx + G_tx;

link_total = total_gain - total_attn; % best and worse case attenuation

above_NF = link_total - NF_dB + P_tx; % amount rx signal is above noise floor

disp(sprintf('Best and Worse Case Scenarios'))
disp(sprintf('Total Attenuation:    %0.2f (dB)\n', total_attn))
disp(sprintf('\nTotal Gain:           %0.2f (dB)', total_gain))
disp(sprintf('Transmit Power:       %0.2f (dBm)\n\n', P_tx))
disp(sprintf('Best and Worse Case Scenarios'))
disp(sprintf('Above Noise Floor:    %0.2f (dB)\n', above_NF))



















