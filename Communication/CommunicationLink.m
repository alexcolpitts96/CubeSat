% Communication Link Simulation
% Date:     October 19, 2017
% Updated:  November 8, 2017
% Author: Alex Colpitts

clear
clc

figure_count = 1;
%% system parameters
r = 400E3; % meters, altitude
f = [146E6 440E6 915E6 2450E6]; % f_c, amateur, amateur ISM ISM 
lambda = 3E8./f;

den = 1.2922; % density of air at 273 K 
P = 101300.0; % pressure in pascal
T = 300; % temperature in kelvin, assume pretty warm

% antennas gains
G_tx = 2; % dB, gain from dipole antenna on CubeSat
G_rx = 15; % dB, gain from cross-polarized yagi for ground station

% % assume chip is ZETA-915-ND (13) or RFM69HCW (20)
P_tx = 20; % dBm

% assume chip is ZETA-915-ND (500E3) or RFM69HCW (300E3)
data_rate = 300E3;

BW = 2*data_rate;

% contants
r_earth = 6371E3; %m
k_b = 1.38064852E-23; % boltzmann's constant
ITU_power = 10*log10(4/1E-3); % W (EIRP)

%% noise floor calculations

Noise_Floor = k_b * T * BW;
NF_dB = 10*log10(Noise_Floor);

disp(sprintf('Noise Floor Estimation:    %0.2f (dB)\n', NF_dB))

%% free space path loss
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

atmos_attn_400 = gaspl(r, f_test, t_celcius, P, den);
atmos_attn_1400 = gaspl(max_distance, f_test, t_celcius, P, den);

figure_count = figure_count +1;
figure(figure_count)
figure_count = figure_count + 1;

subplot(2, 1, 1)
semilogy(f_test/1e9,atmos_attn_400)
xlabel('Frequency (GHz)')
ylabel('Specific Attenuation (dB)')
title('Atmospheric Gas Loss For Various Frequencies r = 400km')

subplot(2, 1, 2)
semilogy(f_test/1e9,atmos_attn_1400)
xlabel('Frequency (GHz)')
ylabel('Specific Attenuation (dB)')
title('Atmospheric Gas Loss For Various Frequencies r = 1400km')

% make approximation of attenuation to be same as 1 GHz 
attn_915 = gaspl([r max_distance], 1E9, t_celcius, P, den);

%% Total attenuation and Antenna Gains
% atmospheric gas losses and free space path losses
total_attn = attn_915 + [-min(path_loss(:,3)), -max(path_loss(:,3))]'; 

% transmit dipole on cubesat, cross polarized yagi on earth 
total_gain = G_rx + G_tx;

link_total = total_gain - total_attn; % best and worse case attenuation

above_NF = link_total - NF_dB + P_tx; % amount rx signal is above noise floor

disp(sprintf('Best and Worse Case Scenarios'))
disp(sprintf('Total Attenuation:    %0.2f (dB)\n', total_attn))
disp(sprintf('Transmit Power:       %0.2f (dBm)\n', P_tx))

%% amplifier stage 
% an amplifier stage is most likely not desirable due to
% power and complexity of adding it to the project 
P_tx_na = P_tx + G_tx;
amp_gain = (ITU_power - P_tx_na)*0.95; % stay under maximum power level

disp(sprintf('Best and Worse Case Scenarios, No Amplifier or PG'))
disp(sprintf('Receive Power:    %0.2f (dBm)\n', P_tx_na - total_attn + G_rx))

% disp(sprintf('Maximum Amplifier Gain:   %0.2f (dB)\n', amp_gain));
% disp(sprintf('Best and Worse Case Scenarios, With Amplifier'))
% disp(sprintf('Receive Power:    %0.2f (dB)\n', P_tx_na + amp_gain - total_attn))

%% data transfer 

% if a camera is used with the following properties
image_width = [640 1920];
image_height = [480 1080];
image_pixels = image_width .* image_height;
pixel_size = 24; % 24 bit color images
image_bits = image_pixels * pixel_size;

% it was determined that that roughly 7-8 minutes overhead at a time
overhead_time = 7.5 * 60; % seconds

raw_data = overhead_time * data_rate; % bits

chip_length = [1, 2, 5, 7, 9, 11, 13, 25, 49];
chip_length = 11;
chip_gain = 10*log10(chip_length);

% row1 = low res image, row2 = high res image
sequence_repetition = zeros(2, length(chip_length));

% determine what lengths could work
for i = 1:length(chip_length)
    sequence_repetition(:,i) = (raw_data./chip_length(i))./image_bits;
end

disp(sprintf('Best and Worse Case Scenarios, No Amplifier, N_seq = 11'))
disp(sprintf('Receive Power:    %0.2f (dBm)\n', P_tx_na - total_attn + G_rx + chip_gain))