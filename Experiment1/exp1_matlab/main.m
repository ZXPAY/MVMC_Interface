clc, clear, close all

timeOutMainLoop = 10;
setStateSpaceParameters = struct(...
                    'paraA', single([1.35 0.55 -0.45 0.35]),...
                    'paraB', single([0.5 0.5]),...
                    'paraC', single([3 1]),...
                    'paraD', single([1]));

setStateSpaceInput = struct('paraU',[0]);   

setStateSpaceGet = struct('C', uint16([0]),...
                           'X0', single([0]),...
                           'X1', single([0]),...
                           'y', single([0]));  

% dotGenerate = single(linspace(0, 10*2*pi, 150));
dotGenerate = ones([150,1]);
dotGenerate(1,1) = 0;
sinGenerate = single(dotGenerate);

port = remo_open(18);

msg = remo_get_msg(port)

[setParaEr, setFormatString, Bytes] = remo_form_put(port, setStateSpaceParameters)

for n=1:5
    msg = remo_get_msg(port)
end

y_state = zeros([150,1]);
for step=1:150
    setStateSpaceInput.paraU = sinGenerate(step);
    [setInputEr, setFormatString, Bytes] = remo_form_put(port, setStateSpaceInput);
    msg = remo_get_msg(port)
    y_state(step,:) = str2num(msg);
end


[data, dataFormat, er] = remo_form_get(port);

% X_state = zeros([100,2]);
% X_state(:,1) = data.type1;
% X_state(:,2) = data.type2;
% y_state = data.type1;
counter = linspace(1,150,150);


% figure();
% plot(counter, sinGenerate);
% title('實驗輸入數據');
% xlabel('筆(數據)');
% ylabel('數值');
% grid on; hold on;

figure();
plot(counter, y_state);
title('實驗輸出數據');
xlabel('筆(數據)');
ylabel('數值');

% figure();
% plot(counter, X_state(:, 1));
% title('實驗X狀態數據-1');
% xlabel('筆(數據)');
% ylabel('數值');
% 
% figure();
% plot(counter, X_state(:, 2));
% title('實驗X狀態數據-2');
% xlabel('筆(數據)');
% ylabel('數值');

remo_close(port);






