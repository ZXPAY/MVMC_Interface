function [formatString, size] = getFormatOfStruct(data)
% function: Short description
%
% Extended description

hmiTypeInfo = struct(...
'Name',{'i8' ,'i16' ,'i32' ,'i64' ,'ui8' ,'ui16' ,'ui32' ,'ui64' ,'f32'  ,'f64'  },...
'Type',{?int8,?int16,?int32,?int64,?uint8,?uint16,?uint32,?uint64,?single,?double},...
'Size',{1    , 2    ,4     ,8     ,1     ,2      ,4      ,8      ,4      ,8      });

if metaclass(data) ~= ?struct
    error('data must be a struct.');
end

names = fieldnames(data);
formatString = '';
size = 0;

for i=1:length(names)
    arr = getfield(data,char(names(i)));
    for j=1:length(hmiTypeInfo)
        if metaclass(arr) == hmiTypeInfo(j).Type
            typeStr = hmiTypeInfo(j).Name;
            typeSize = hmiTypeInfo(j).Size;
            break;
        end
    end
    formatString = [formatString, typeStr, 'x', int2str(length(arr))];
    size = size + typeSize*length(arr);
    if i~=length(names)
        formatString = [formatString ,','];
end

end  % function
