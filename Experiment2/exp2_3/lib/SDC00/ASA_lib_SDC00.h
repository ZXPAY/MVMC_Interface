#ifndef __ASA_SDC00_H__
#define __ASA_SDC00_H__

char ASA_SDC00_set(char ASA_ID, char LSByte, char Mask, char shift, char Data);
char ASA_SDC00_put(char ASA_ID, char LSByte, char Bytes, void *Data_p);
char ASA_SDC00_get(char ASA_ID, char LSByte, char Bytes, void *Data_p);

#endif
