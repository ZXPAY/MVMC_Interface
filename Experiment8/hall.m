clc, clear, close all

raw_data = load('hall_1.mat');
raw_data = struct2cell(raw_data);


data = zeros([100, 2]);

data(:,1) = raw_data{1}.f0;
data(:,2) = raw_data{2}.f0;

fig = figure();
plot([1:1:100], data(:,1), 'o--');
xlabel('個數');
ylabel('ADC Value');
title('Hall sensor test, No magnet');
grid on;
saveas(fig, strcat('hall_noMagnet.jpg'));

fig = figure();
plot([1:1:100], data(:,2), 'o--');
xlabel('個數');
ylabel('ADC Value');
title('Hall sensor test, use magnet');
grid on;
saveas(fig, strcat('hall_Magnet.jpg'));
