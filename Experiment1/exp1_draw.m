clc, clear, close all

dataNumbers = 100;

raw_data = load('exp1_HMI_1.mat');
rawCellData = struct2cell(raw_data);

counter = zeros([dataNumbers]);
X_state = zeros([dataNumbers, 2]);
y_state = zeros([dataNumbers]);

for n=1:dataNumbers
     raw = rawCellData{n};
     counter(n) = raw.f0;
     X_state(n, 1) = raw.f1;
     X_state(n, 2) = raw.f2;
     y_state(n) = raw.f3;
end

figure();
plot(counter, y_state);
title('實驗輸出數據');
xlabel('筆(數據)');
ylabel('數值');
grid on; hold on;

figure();
plot(counter, X_state(:, 1));
title('實驗X狀態數據');
xlabel('筆(數據)');
ylabel('數值');

figure();
plot(counter, X_state(:, 2));
title('實驗X狀態數據');
xlabel('筆(數據)');
ylabel('數值');

