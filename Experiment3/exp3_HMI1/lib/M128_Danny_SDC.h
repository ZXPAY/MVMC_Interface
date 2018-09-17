#ifndef M128_DANNY_SDC_H
#define M128_DANNY_SDC_H

#define SDC_ID 4
#define MAX_WORDS_SIZE 32

/*
    Usage :
    1. 設定文件名稱和副檔名  setSDCName(..., ...);
    2. 開啟模式, append, replace or read mode
    3. 寫入或讀取資料
    4. 關閉文件  closeSDC();    --> important
*/

void createFile(char fileName[], char fileExtension[]);
void testSDC();
void initBuffer(char dataString[]);
void setSDCContinuous(); // Append mode
void setSDCReplace();    // Replace mode 
void setSDCRead();       // Read mode 
void setSDCWords(char dataString[], char datalength);
void closeSDC();
void setSDCName(char name[], char secondName[]);
int getBufferSize(char dataString[]);
int getBufferSizeUseCRLF(char dataString[]);

// SDC get
char getSDCData(char dataString[], char dataSize);
void getSDCDate(unsigned int dataDate);
void getSDCTime(unsigned int dataTime);
void getSDCSize(unsigned int dataSize);
void getSDCType(char isProhibited);
void getSDCResponse(char GoodOrNot);


#endif

