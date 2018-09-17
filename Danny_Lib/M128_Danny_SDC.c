#include "M128_Danny_SDC.h"
#include <stdio.h>
#include <inttypes.h>
#include "SDC00/ASA_lib_SDC00.h"

void createFile(char fileName[], char fileExtension[]){
    char check;
    check = ASA_SDC00_put(SDC_ID, 64, 8, fileName);
    if(check != 0) {printf("1 fail ...%d\n", check);}

    check = ASA_SDC00_put(SDC_ID, 72, 3, fileExtension);
    if(check != 0) {printf("2 fail ...%d\n", check);}

    check = ASA_SDC00_set(SDC_ID, 200, 0x07, 0, 0x05); 
    if(check != 0) {printf("Open fail ...%d\n", check);}

    check = ASA_SDC00_set(SDC_ID, 200, 0x01, 0, 0x00); // close
    if(check != 0) {printf("Close fail ...%d\n", check);}
}

void testSDC(){
    createFile("Hello", "txt");
    setSDCReplace();
    setSDCWords("Hello World !!!\r\n", 17);
    closeSDC();
}


void initBuffer(char dataString[]){
    memset(dataString, 0, sizeof(dataString));
}

// 續寫模式
void setSDCContinuous(){
    char check;
    check = ASA_SDC00_set(SDC_ID, 200, 0x07, 0, 0x07); // open 續寫
    if(check != 0) {printf("Open fail ...%d\n", check);}
}

// 蓋寫模式
void setSDCReplace(){
    char check;
    check = ASA_SDC00_set(SDC_ID, 200, 0x07, 0, 0x05); // open 覆蓋
    if(check != 0) {printf("Open fail ...%d\n", check);}
}

// 讀取模式
void setSDCRead(){
    char check;
    check = ASA_SDC00_set(SDC_ID, 200, 0x03, 0, 0x01); // Read 允讀
    if(check != 0) {printf("Open fail ...%d\n", check);}
}

char getSDCData(char dataString[], char dataSize){
    char check;
    check = ASA_SDC00_get(SDC_ID, 0, dataSize, dataString); // 取裡面的資料
    //if(check != 0) {printf("Read fail ...%d\n", check);}
    
    return check;
}
void getSDCDate(unsigned int dataDate){
    char check;
    check = ASA_SDC00_get(SDC_ID, 78, 2, &dataDate); // 取裡面的日期
    if(check != 0) {printf("Read fail ...%d\n", check);}
}
void getSDCTime(unsigned int dataTime){
    char check;
    check = ASA_SDC00_get(SDC_ID, 76, 2, &dataTime); // 取裡面的時間
    if(check != 0) {printf("Read fail ...%d\n", check);}
}
void getSDCSize(unsigned int dataSize){
    char check;
    check = ASA_SDC00_get(SDC_ID, 82, 4, &dataSize); // 取裡面的大小
    if(check != 0) {printf("Read fail ...%d\n", check);}
}
void getSDCType(char isProhibited){
    char check;
    check = ASA_SDC00_get(SDC_ID, 75, 1, &isProhibited); 
    if(check != 0) {printf("Read fail ...%d\n", check);}
}
void getSDCResponse(char GoodOrNot){
    char check;
    check = ASA_SDC00_get(SDC_ID, 34, 1, &GoodOrNot);
    if(check != 0) {printf("Read fail ...%d\n", check);}
}

void setSDCWords(char dataString[], uint16_t dataSize){
    char check;
    // check = ASA_SDC00_put(SDC_ID, 0, getBufferSize(dataString), dataString);
    check = ASA_SDC00_put(SDC_ID, 0, dataSize, dataString);
    if(check != 0) {printf("Write fail ...%d\n", check);}
}

void setSDCBinUint16(uint16_t data){
    char check;
    check = ASA_SDC00_put(SDC_ID, 0, 2, &data);
    if(check != 0) {printf("Write fail ...%d\n", check);}
}

void closeSDC(){
    char check;
    check = ASA_SDC00_set(SDC_ID, 200, 0x01, 0, 0x00); // close
    if(check != 0) {printf("Close fail ...%d\n", check);}
}

void setSDCName(char name[], char secondName[]){
    char check;
    check = ASA_SDC00_put(SDC_ID, 64, 8, name);
    if(check != 0) {printf("SD name fail ...%d\n", check);}

    check = ASA_SDC00_put(SDC_ID, 72, 3, secondName);
    if(check != 0) {printf("SD second name fail ...%d\n", check);}
}

int getBufferSize(char dataString[]){
    int counter = 0;
    for(int i=0;i<MAX_WORDS_SIZE;i++){
        if((dataString[i] != '\0')){
            counter++;
        }
        else{
            return counter;
        }
    }
}






