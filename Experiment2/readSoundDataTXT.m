clc, clear, close all

filename = 'A.TXT';
%filename = 'HMITEST.TXT';

fileID  = fopen(filename, 'r');
formatSpec = '%f';
Fs = 2000;

data = fscanf(fileID,formatSpec)

norm_data = (data-min(data))/(max(data)-min(data));

sound(norm_data(1000:1400), Fs);




