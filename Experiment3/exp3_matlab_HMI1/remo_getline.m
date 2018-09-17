function [s] = remo_getline(port)

s = '';
isPrevSpace = 0;
while 1
    ch = fread(port, 1, 'char');
    if (ch==10) | (ch==0)
        break
    end
    if ch == 32
        isPrevSpace = 1;
    elseif isPrevSpace == 1
        isPrevSpace = 0;
        s = [s, ' ', char(ch)];
    else
        s = [s, char(ch)];
    end
end

end
