function [data] = getBinDataArray(file_name)
f = fopen(file_name);
raw_data = fread(f);
data = zeros([uint16(size(raw_data)/2) ,1]);

for n=1:(size(raw_data)/2)
   if mod(n, 2) == 0
       data(n, 1) = raw_data(n-1) + raw_data(n)*256;
   else
       data(n, 1) = raw_data(n) + raw_data(n+1)*256;
   end
end
end

