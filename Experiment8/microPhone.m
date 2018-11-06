clc, clear, close all

filenames = ['micro_1.mat'];
fileNumber = 1;
dimension = 4;
dataNumbers = 100;

data = zeros([101, dimension, fileNumber]);
TimeConsumeMS = zeros([1, dimension]);
for i=1:fileNumber
    temp = load(filenames(i, :));
    temp = struct2cell(temp);
    for n=1:dimension
        data(:, n, i) = temp{n}.f0;
        TimeConsumeMS(1, n) = data(end, n, i);
    end
end

for i=1:dimension
    fig = figure();
    plot([1:1:dataNumbers], data(1:dataNumbers, i), 'o--');
    grid on; hold on
    xlabel('dot');
    ylabel('value');
    title('micro phone data 100 µ§¼Æ¾Ú');
    saveas(fig, strcat('_microPhone_', num2str(500*i), 'Hz.jpg'));
    
    fig = figure();
    Y = fft(data(1:dataNumbers,i));
%     Fs = double(double(dataNumbers)/double(TimeConsumeMS(1, i))*1000)     % SamplingFrequency
    Fs = 12500;
    T = 1/Fs;
    L = double(dataNumbers);
    P2 = abs(Y/L);
    P1 = P2(1:L/2+1);
    P1(2:end-1) = 2*P1(2:end-1);
    f = Fs*(0:(L/2))/L;
    plot(f,P1) 
    title(strcat('micro phoneFFT ', num2str(500*i), 'Hz'));
    xlabel('f (Hz)');
    ylabel('|P1(f)|');
    [Hz, Amp] = ginput(1)   % use mouth click the picture
    saveas(fig, strcat('_microPhoneFFT_', num2str(500*i), 'Hz.jpg'));
end







