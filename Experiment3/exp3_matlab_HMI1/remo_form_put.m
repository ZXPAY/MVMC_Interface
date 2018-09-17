function [error, FormatString, Bytes] = remo_form_put(Port,data)

error = 0;
CheckSum = 0;

[FormatString, Bytes] = getFormatOfStruct(data)

fwrite(Port,hex2dec('AB'),'uint8');
fwrite(Port,hex2dec('AB'),'uint8');
fwrite(Port,hex2dec('AB'),'uint8');

FormatBytes = length(FormatString);
TotalBytes = 1+FormatBytes+Bytes;

TotalBytesH = rem(TotalBytes,256);
TotalBytesL = floor(TotalBytes/256);
fwrite(Port,TotalBytesH,'uint8');
fwrite(Port,TotalBytesL,'uint8');
fwrite(Port,FormatBytes,'uint8');

%  FormatString
[Format,TypeNum,StructSize,error] = decodeFormatStr(FormatString);
CheckSum = CheckSum + TotalBytes;
CheckSum = FormatBytes;

% put FormatString
for i = 1:FormatBytes
    fwrite(Port,uint8(FormatString(i)),'uint8');
    CheckSum = CheckSum + FormatString(i);
end

% put data
names = fieldnames(data);

for n = 1:Bytes/StructSize
    for i = 1:TypeNum
        putdata = cast(getfield(data(n),names{i}) , Format(i).Type);
        Cdata = typecast(putdata,'uint8');
        fwrite(Port,Cdata,'uint8');
        CheckSum = CheckSum +sum(Cdata);
    end
end

% put CheckSum
CheckSum = rem(CheckSum,256);
fwrite(Port,CheckSum,'uint8');


end
