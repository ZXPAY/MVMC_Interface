clc, clear, close all

timeOutMainLoop = 10;
setStateSpaceParameters = struct(...
                    'paraA', single([0 1 -2 2]),...
                    'paraB', single([0 1]),...
                    'paraC', single([-2 4]),...
                    'paraD', single([1]));

setStateSpaceInput = struct('paraU',[0]);   

setStateSpaceGet = struct('C', uint16([0]),...
                           'X0', single([0]),...
                           'X1', single([0]),...
                           'y', single([0]));  

dotGenerate = single(linspace(0, 10*2*pi, 100));
sinGenerate = single(sin(dotGenerate));

port = remo_open(4);

msg = remo_get_msg(port)

[setParaEr, setFormatString, Bytes] = remo_form_put(port, setStateSpaceParameters)

for n=1:5
    msg = remo_get_msg(port)
end

for step=1:100
    setStateSpaceInput.paraU = sinGenerate(step);
    [setInputEr, setFormatString, Bytes] = remo_form_put(port, setStateSpaceInput);
    msg = remo_get_msg(port)
end


[data, dataFormat, er] = remo_form_get(port);

X_state = zeros([100,2]);
X_state(:,1) = data.type1;
X_state(:,2) = data.type2;
y_state = data.type3;
counter = linspace(1,100,100);


figure();
plot(counter, sinGenerate);
title('�����J�ƾ�');
xlabel('��(�ƾ�)');
ylabel('�ƭ�');
grid on; hold on;

figure();
plot(counter, y_state);
title('�����X�ƾ�');
xlabel('��(�ƾ�)');
ylabel('�ƭ�');

figure();
plot(counter, X_state(:, 1));
title('����X���A�ƾ�-1');
xlabel('��(�ƾ�)');
ylabel('�ƭ�');

figure();
plot(counter, X_state(:, 2));
title('����X���A�ƾ�-2');
xlabel('��(�ƾ�)');
ylabel('�ƭ�');

remo_close(port);






