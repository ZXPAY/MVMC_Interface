function [error] = remo_put(port,data)

error = 0;
typeStr = class(data);

if (strcmp(typeStr, 'int8'))
    type = 0;
    typeSize = 1;
    Cdata = typecast(int8(data),'uint8');
elseif (strcmp(typeStr, 'int16'))
    type = 1;
    typeSize = 2;
    Cdata = typecast(int16(data),'uint8');
elseif (strcmp(typeStr, 'int32'))
    type = 2;
    typeSize = 4;
    Cdata = typecast(int32(data),'uint8');
elseif (strcmp(typeStr, 'int64'))
    type = 3;
    typeSize = 8;
    Cdata = typecast(int64(data),'uint8');
elseif (strcmp(typeStr, 'uint8'))
    type = 4;
    typeSize = 1;
    Cdata = typecast(uint8(data),'uint8');
elseif (strcmp(typeStr, 'uint16'))
    type = 5;
    typeSize = 2;
    Cdata = typecast(uint16(data),'uint8');
elseif (strcmp(typeStr, 'uint32'))
    type = 6;
    typeSize = 4;
    Cdata = typecast(uint32(data),'uint8');
elseif (strcmp(typeStr, 'uint64'))
    type = 7;
    typeSize = 8;
    Cdata = typecast(uint64(data),'uint8');
elseif (strcmp(typeStr, 'single'))
    type = 8;
    typeSize = 4;
    Cdata = typecast(single(data),'uint8');
elseif (strcmp(typeStr, 'double'))
    type = 9;
    typeSize = 8;
    Cdata = typecast(double(data),'uint8');
else
    warning('不支援的資料格式');
    return;
end

bytes = length(Cdata);

fwrite(port,hex2dec('AB'),'uint8');
fwrite(port,hex2dec('AB'),'uint8');
fwrite(port,hex2dec('AB'),'uint8');
fwrite(port,type,'uint8');
bytesH = rem(bytes,256);
bytesL = floor(bytes/256);
fwrite(port,bytesH,'uint8');
fwrite(port,bytesL,'uint8');

% get binary data in uint8 form ASA_PC
checkSum = bytesH + bytesL;
for i = 1:bytes
    fwrite(port,Cdata(i),'uint8');
    checkSum = checkSum + Cdata(i);
end

% get check value form ASA_PC
checkSum = rem(checkSum, 256);
fwrite(port, checkSum, 'uint8');

end
