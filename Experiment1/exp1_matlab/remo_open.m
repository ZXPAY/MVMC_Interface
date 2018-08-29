function [ Port ] = remo_open( COM_number )

delete(instrfindall);

% If you have Instrument Control Toolbox you can also try >>info =instrhwinfo('serial'), to get all the information about serial ports.
COM = strcat('COM',num2str(COM_number));
% set commuication
Port = serial(COM);
set(Port,'BaudRate',38400);
set(Port,'InputBufferSize',5000);
set(Port, 'FlowControl', 'none');
set(Port, 'Parity', 'none');
set(Port, 'DataBits', 8);
set(Port, 'StopBit', 1);
set(Port, 'Timeout',1000);
fopen(Port);

end
