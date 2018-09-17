function [data,error] = remo_get(port)

error = 0;

% get pre-check form ASA_PC
for i = 1:3
    check = fread(port, 1,'uint8');
    if check ~= hex2dec('AA');
        error = 2;
        warning('«Ê¥]ÀY¿ù»~');
        return
    end
end

% get type form ASA_PC
Type = fread(port, 1,'uint8');

switch Type
    case 0  % int8
        type = 'int8';
        TypeSize = 1;
    case 1  % int16
        type = 'int16';
        TypeSize = 2;
    case 2  % int32
        type = 'int32';
        TypeSize = 4;
    case 3  % int64
        type = 'int64';
        TypeSize = 8;
    case 4  % uint8
        type = 'uint8';
        TypeSize = 1;
    case 5  % uint16
        type = 'uint16';
        TypeSize = 2;
    case 6  % uint32
        type = 'uint32';
        TypeSize = 4;
    case 7  % uint64
        type = 'uint64';
        TypeSize = 8;
    case 8  % float32
        type = 'single';
        TypeSize = 4;
    case 9  % float64
        type = 'double';
        TypeSize = 8;
    otherwise
        warning('error type');
        return
end

% get data bytes form ASA_PC
BytesH = fread(port, 1,'uint8');
BytesL = fread(port, 1,'uint8');
Bytes  = BytesH*256 + BytesL;
CheckSum = BytesH + BytesL;

% get binary data in uint8 form ASA_PC
for i = 1:Bytes
    Cdata(i) = fread(port, 1,'uint8');
    CheckSum = CheckSum + Cdata(i);
end

% get check value form ASA_PC
CheckSum = rem(CheckSum,256);
GetCheckSum = fread(port, 1,'uint8');

% trans binary data to type
data = typecast(0, type);
for i = 1 : Bytes/TypeSize
    data(i) = typecast(uint8(Cdata( (i-1)*TypeSize +1: (i)*TypeSize )), type);
end

if CheckSum ~= GetCheckSum
    error = 4;
    warning('ÀË¬d½X¿ù»~');
end

end
