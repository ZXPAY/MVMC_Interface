/*
 * testHMI.c
 *
 * Created: 2018/8/7 下午 02:03:53
 *  Author: zxpay
 */ 

#include <inttypes.h>
#include "ASA_Lib.h"

#define num_int8 0
#define num_int16 1
#define num_int32 2
#define num_int64 3
#define num_uint8 4
#define num_uint16 5
#define num_uint32 6
#define num_uint64 7
#define num_float32 8

char typeName[9][10] = {{"int8"}, {"int16"}, {"int32"}, {"int64"},
{"uint8"}, {"uint16"}, {"uint32"}, {"uint64"},
{"float32"}};

void testM128_HMI_put(){
	char check;
	char testChar = 1;
	printf("Send char data ==> 1...");
	check = M128_HMI_put(sizeof(char), num_int8, &testChar);
	if(check!=0) printf("Error:%d\n", check);
	
	int testInt16 = 2;
	printf("Send int16 data ==> 2...");
	check = M128_HMI_put(sizeof(int), num_int16, &testInt16);
	if(check!=0) printf("Error:%d\n", check);
	
	int32_t testInt32 = 3;
	printf("Send int32 data ==> 3...");
	check = M128_HMI_put(sizeof(int32_t), num_int32, &testInt32);
	if(check!=0) printf("Error:%d\n", check);
	
	int64_t testInt64 = 4;
	printf("Send int64 data ==> 4...");
	check = M128_HMI_put(sizeof(int64_t), num_int64, &testInt64);
	if(check!=0) printf("Error:%d\n", check);
	
	uint8_t testUInt8 = 5;
	printf("Send int8 data ==> 5...");
	check = M128_HMI_put(sizeof(uint8_t), num_uint8, &testUInt8);
	if(check!=0) printf("Error:%d\n", check);
	
	uint16_t testUInt16 = 6;
	printf("Send uint16 data ==> 6...");
	check = M128_HMI_put(sizeof(uint16_t), num_uint16, &testUInt16);
	if(check!=0) printf("Error:%d\n", check);
	
	uint32_t testUInt32 = 7;
	printf("Send uint32 data ==> 7...");
	check = M128_HMI_put(sizeof(uint32_t), num_uint32, &testUInt32);
	if(check!=0) printf("Error:%d\n", check);
	
	uint64_t testUInt64 = 8;
	printf("Send uint64 data ==> 8...");
	check = M128_HMI_put(sizeof(uint64_t), num_uint64, &testUInt64);
	if(check!=0) printf("Error:%d\n", check);
	
	float testFloat = 9.99;
	printf("Send float data ==> 9.99...");
	check = M128_HMI_put(sizeof(float), num_float32, &testFloat);
	if(check!=0) printf("Error:%d\n", check);
	
	double testDouble = 10.1010;
	printf("Send double data ==> 10.1010...");
	check = M128_HMI_put(sizeof(double), num_float32, &testDouble);
	if(check!=0) printf("Error:%d\n", check);
	
}

void testM128_HMI_get(){
	char check = 0;
	printf("Please enter number which data shape want to get ...\n");
	int dataMode;
	scanf("%d", &dataMode);
	printf("type data to get ==>%s\n", typeName[dataMode]);
	switch(dataMode){
		case 0:
		printf("Getting...\n");
		int8_t data_int8 = 0;
		check = M128_HMI_get(sizeof(int8_t), &data_int8);
		printf("Data -> %d\n", data_int8);
		break;
		case 1:
		printf("Getting...\n");
		int16_t data_int16 = 0;
		check = M128_HMI_get(sizeof(int16_t), &data_int16);
		printf("Data -> %d\n", data_int16);
		break;
		case 2:
		printf("Getting...\n");
		int32_t data_int32 = 0;
		check = M128_HMI_get(sizeof(int32_t), &data_int32);
		printf("Data -> %d\n", data_int32);
		break;
		case 3:
		printf("Getting...\n");
		int64_t data_int64 = 0;
		check = M128_HMI_get(sizeof(int64_t), &data_int64);
		printf("Data -> %d\n", data_int64);
		break;
		case 4:
		printf("Getting...\n");
		uint8_t data_uint8 = 0;
		check = M128_HMI_get(sizeof(uint8_t), &data_uint8);
		printf("Data -> %d\n", data_uint8);
		break;
		case 5:
		printf("Getting...\n");
		uint16_t data_uint16 = 0;
		check = M128_HMI_get(sizeof(uint16_t), &data_uint16);
		printf("Data -> %u\n", data_uint16);
		break;
		case 6:
		printf("Getting...\n");
		uint32_t data_uint32 = 0;
		check = M128_HMI_get(sizeof(uint32_t), &data_uint32);
		printf("Data -> %lu\n", data_uint32);
		break;
		case 7:
		printf("Getting...\n");
		uint64_t data_uint64 = 0;
		check = M128_HMI_get(sizeof(uint64_t), &data_uint64);
		printf("Data -> %lu\n", data_uint64);
		break;
		case 8:
		printf("Getting...\n");
		float data_float32 = 0;
		check = M128_HMI_get(sizeof(float), &data_float32);
		printf("Data -> %.2f\n", data_float32);
		break;
		case 9:
		printf("Getting...\n");
		double data_double = 0;
		check = M128_HMI_get(sizeof(double), &data_double);
		printf("Data -> %.2f\n", data_double);
		break;
	}
	if(check != 0){
		//printf("error => %d\n", check);
	}
}

