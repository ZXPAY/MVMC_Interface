function [error, FormatString, Bytes] = remo_sync_form_put(port,data)

fprintf(port, 'sync');
res = remo_getline(port);
[error, FormatString, Bytes] = remo_form_put(port,data);

end
