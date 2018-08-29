function [  ] = remo_close( Port )

fclose(Port)
delete(Port)

end
