function [Format,TypeNum,StructSize,error] = decodeFormatStr(FormatString)
% handle FormatString
error = 0;
StructSize = 0;
FormatBytes = length(FormatString);
TypeInfo = struct(...
'Name',{'i64'  ,'ui64'  ,'i8'  ,'ui8'  ,'i32'  ,'ui32'  ,'f64'   ,'i16'  ,'ui16'  ,'f32'   },...
'Type',{'int64','uint64','int8','uint8','int32','uint32','double','int16','uint16','single'},...
'Size',{ 8     , 8      , 1    , 1     , 4     , 4      , 8      ,2     , 2      , 4      });

TypeNum = 0;
i = 1;
while i <= FormatBytes-1
    for j = 1:10
        l = i+length(TypeInfo(j).Name)-1;
        if l > FormatBytes-1
            continue;
        end
        if strcmp( FormatString(i:l) , TypeInfo(j).Name )
            TypeNum = TypeNum + 1;
            Format(TypeNum).Name = strcat('type', num2str(TypeNum) );
            Format(TypeNum).Type = TypeInfo(j).Type;
            Format(TypeNum).TypeSize = TypeInfo(j).Size;
            Format(TypeNum).Num = 0;
            i = i +length(TypeInfo(j).Name);
            break;
        end
        if j==10
            error = 1;
            warning('FormatString error');
            return;
        end
    end

    if FormatString(i) == 'x'
        i = i + 1;
    else
        warning('FormatString error 3');
        return;
    end


    while i <= FormatBytes
        if FormatString(i)>='0' && FormatString(i)<='9'
            Format(TypeNum).Num = Format(TypeNum).Num *10 + FormatString(i) - 48;
            i = i+1;
        else
            break;
        end
    end

    if i <= FormatBytes
        if FormatString(i) == ','
            i = i + 1;
        else
            warning('FormatString error 3');
            return;
        end
    end
    StructSize = StructSize + Format(TypeNum).TypeSize*Format(TypeNum).Num;
    if Format(TypeNum).Num == 0
        error = 1;
        warning('FormatString¿ù»~');
        return;
    end

end

end
