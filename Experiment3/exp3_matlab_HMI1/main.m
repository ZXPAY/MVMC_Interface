clc, clear, close all

portNumber = 18;

DataAcquisitionSystem = struct('TimeConsumeMS', uint32([0]));

port = remo_open(portNumber);

msg = remo_get_msg(port)   % Enter ADC data number you want to get.

prompt = '';
ADCNumber = input(prompt)
ADCNumber = uint16(ADCNumber);
remo_put(port, ADCNumber);
msg = remo_get_msg(port)   % DataNumbers is %d 

msg = remo_get_msg(port)   % Run Main Code

msg = remo_get_msg(port) 
msg = remo_get_msg(port)   % OK, Read SDC Data and send to matlab ...

getDataArray = zeros([ADCNumber, 1]);

TimeConsumeMS = str2num(remo_get_msg(port))

for i=1:ADCNumber
    data = str2num(remo_get_msg(port));
    getDataArray(i,:) = data;
end
remo_close(port);
getDataArray
dotGenerate = linspace(0, double(0.5*ADCNumber), ADCNumber);

norm_data = (getDataArray - min(getDataArray)) / (max(getDataArray) - min(getDataArray));

plot(dotGenerate, getDataArray);
xlim([0, 0.5*ADCNumber]);
ylim([0,1023]);
xlabel('ms');
ylabel('ADC Value');
grid on; hold on;

figure();
Y = fft(getDataArray);
% Fs = double(double(ADCNumber)/double(TimeConsumeMS)*1000)     % SamplingFrequency
Fs = 2000;
T = 1/Fs;
L = double(ADCNumber);
P2 = abs(Y/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = 2*P1(2:end-1);
f = Fs*(0:(L/2))/L;
plot(f,P1) 
title('Single-Sided Amplitude Spectrum of X(t)');
xlabel('f (Hz)');
ylabel('|P1(f)|');
[Hz, Amp] = ginput(1)   % use mouth click the picture


sound(norm_data, Fs);




