function [data,error] = remo_sync_get(port)

fprintf(port, 'sync');
res = remo_getline(port);
[data, error] = remo_get(port);

end
