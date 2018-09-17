function [data,FormatString,error] = remo_form_get(Port)

error = 0;
CheckSum = 0;

% get pre-check form ASA_PC
for i = 1:3
    check = fread(Port,1,'uint8');
    if check ~= hex2dec('BB');
        error = 2;
        warning('封包頭錯誤');
        return
    end
end

% get TotalBytes
GetTotalBytesH = fread(Port,1,'uint8');
GetTotalBytesL = fread(Port,1,'uint8');
GetTotalBytes = GetTotalBytesH*256 + GetTotalBytesL;
CheckSum = GetTotalBytesH + GetTotalBytesL;

% get FormatBytes
GetFormatBytes = fread(Port,1,'uint8');
CheckSum = CheckSum + GetFormatBytes;

% get data bytes
GetBytes = GetTotalBytes-GetFormatBytes;
Bytes = GetBytes-1;

% get FormatBytes
for i = 1:GetFormatBytes
    FormatString(i) = fread(Port,1,'uint8');
    CheckSum = CheckSum + FormatString(i);
end
FormatString = char(FormatString);
[Format,TypeNum,StructSize,error] = decodeFormatStr(FormatString);

% get binary data in uint8 form ASA_PC
for i = 1:Bytes
    Cdata(i) = fread(Port,1,'uint8');
    CheckSum = CheckSum + Cdata(i);
end

% get check value form ASA_PC
CheckSum = rem(CheckSum,256);
GetCheckSum = fread(Port,1,'uint8');

% trans binary data to struct
i = 1;
for n = 1 : Bytes/StructSize
    for j = 1:TypeNum
        l = i + Format(j).TypeSize*Format(j).Num - 1;
        arraydata = typecast( uint8(Cdata( i:l )) , Format(j).Type);
        i = l+1;
        celldata(j,n)= {arraydata};
    end
end
data = cell2struct(celldata,{Format.Name},1);

if CheckSum ~= GetCheckSum
    error = 4;
    warning('CheckSum錯誤，請重新傳輸');
    return;
end

end
