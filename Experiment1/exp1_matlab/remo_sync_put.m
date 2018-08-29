function [error] = remo_sync_put(port,data)

fprintf(port, 'sync');
res = remo_getline(port);
[error] = remo_put(port,data);

end
