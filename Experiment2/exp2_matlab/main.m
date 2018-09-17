clc, clear, close all

portNumber = 3; 
sinGenerate = 0;

for n=0:1000
    dotGenerate = linspace(1,2*pi*n,200);
    sinGenerate = sinGenerate + sin(dotGenerate);
%     if mod(n, 100) == 0
%         figure();
%         plot(dotGenerate, sinGenerate);
%         xlabel('dot number');
%         ylabel('sin value');
%         grid on;
%     end
end
sinGenerate = sinGenerate / 1000;

plot(dotGenerate, sinGenerate);
xlabel('dot number');
ylabel('sin value');
grid on;

% matlabStruct = struct('rawData', uint16(sinGenerate));
% 
% port = remo_open(portNumber);
% 
% msg = remo_get_msg(port)   % Get data from matlab ...
% 
% [error, FormatString, Bytes] = remo_form_put(port, matlabStruct)
% 
% 
% for n=1:200
%    msg = remo_get_msg(port)  
% end
% 
% for n=1:200
%    msg2 = remo_get_msg(port)  
% end
% 
% remo_close(port);


