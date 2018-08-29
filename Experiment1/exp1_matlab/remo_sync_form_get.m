function [data,FormatString,error] = remo_sync_form_get(port)

fprintf(port, 'sync');
res = remo_getline(port);
[data,FormatString,error] = remo_form_get(port);

end
