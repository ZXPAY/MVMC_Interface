/*
 * exp1_HMI.c
 *
 * Created: 2018/8/2 上午 11:33:20
 * Author : zxpay
 */ 
#define F_CPU 11592000
#include <avr/io.h>
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
//#define num_float64 9

void initStateSpace();
void updateStateSpace();
void setStateSpaceParaA();
void setStateSpaceParaB();
void setStateSpaceParaC();
void setStateSpaceParaD();
void setStateSpaceParaU();
void setStateSpaceAllPara();
void sendStateSpaceDataToHMI();
void setStateSpaceParaUsingHMI();

/*
	Equation : 
	>> x(k+1) = A(k) * x(k) + B(k) * u(k)
	>> y(k) = C(k) * x(k) + D(k) * u(k)
************************************************
	SamplingCounter : 紀錄執行次數
	xState[20] : 儲存20筆 X state的資料
	yState[20] : 儲存20筆 y state的資料 (輸出)
	paraA : 可設定的參數 => A
	paraB : 可設定的參數 => B
	paraC : 可設定的參數 => C
	paraD : 可設定的參數 => D
	paraU : 可設定的輸入 => U (輸入)
*/
struct StateSpace{
	uint16_t SamplingCounter;
	float xState[20];
	float yState[20];
	float paraA;
	float paraB;
	float paraC;
	float paraD;
	float paraU; // input
}StateSpace;

void initStateSpace() {
	StateSpace.SamplingCounter = 0;
	for(int i=0;i<sizeof(StateSpace.xState)/4;i++) StateSpace.xState[i]=0;
	for(int i=0;i<sizeof(StateSpace.yState)/4;i++) StateSpace.yState[i]=0;
	StateSpace.paraA = 0;
	StateSpace.paraB = 0;
	StateSpace.paraC = 0;
	StateSpace.paraD = 0;
	StateSpace.paraU = 0;
	updateStateSpace();
}

void updateStateSpace() {
	uint16_t thisPosition = StateSpace.SamplingCounter;
	if (thisPosition == 0) {
		StateSpace.xState[thisPosition] = StateSpace.paraA*StateSpace.xState[thisPosition] +
		StateSpace.paraB*StateSpace.paraU;
		StateSpace.yState[thisPosition] = StateSpace.paraC*StateSpace.xState[thisPosition] +
		StateSpace.paraD*StateSpace.paraU;
	}
	else {
		StateSpace.xState[thisPosition] = StateSpace.paraA*StateSpace.xState[thisPosition-1] +
		StateSpace.paraB*StateSpace.paraU;
		StateSpace.yState[thisPosition] = StateSpace.paraC*StateSpace.xState[thisPosition-1] +
		StateSpace.paraD*StateSpace.paraU;
	}
	StateSpace.SamplingCounter++;
	if(StateSpace.SamplingCounter==sizeof(StateSpace.xState)/4){
		StateSpace.xState[0] = StateSpace.xState[sizeof(StateSpace.xState)/4-1];
		StateSpace.yState[0] = StateSpace.yState[sizeof(StateSpace.xState)/4-1];
		StateSpace.SamplingCounter = 1;
		printf("RESET!!\n");
	}
	printf("cnt:%d, %.2f\n", StateSpace.SamplingCounter, StateSpace.yState[StateSpace.SamplingCounter]);
}

void setStateSpaceParaA(){
	float enterData;
	printf("Enter parameter A\n");
	scanf("%f", &enterData);
	StateSpace.paraA = enterData;
}
void setStateSpaceParaB(){
	float enterData;
	printf("Enter parameter B\n");
	scanf("%f", &enterData);
	StateSpace.paraB = enterData;
}
void setStateSpaceParaC(){
	float enterData;
	printf("Enter parameter C\n");
	scanf("%f", &enterData);
	StateSpace.paraC = enterData;
}
void setStateSpaceParaD(){
	float enterData;
	printf("Enter parameter D\n");
	scanf("%f", &enterData);
	StateSpace.paraD = enterData;
}
void setStateSpaceParaU(){
	float enterData;
	printf("Enter parameter U\n");
	scanf("%f", &enterData);
	StateSpace.paraU = enterData;
}
void setStateSpaceAllPara(){
	setStateSpaceParaA();
	setStateSpaceParaB();
	setStateSpaceParaC();
	setStateSpaceParaD();
	setStateSpaceParaU();
	updateStateSpace();
}

void sendStateSpaceDataToHMI(){
	char check = M128_HMI_Form_put("ui16x1,f32x20,f32x20,f32x1,f32x1,f32x1,f32x1,f32x1",
						182, &StateSpace);
	if(check); //printf("Error -> %d\n", check);
}

void setStateSpaceParaUsingHMI(){
	char check = M128_HMI_Form_get("ui16x1,f32x20,f32x20,f32x1,f32x1,f32x1,f32x1,f32x1",
	182, &StateSpace);
	if(check) printf("Error -> %d\n", check);
}

void printParameters(){
	printf("print parameters ...\n");
	printf("Counter:%d\n", StateSpace.SamplingCounter);
	printf("A:%f\n", StateSpace.paraA);
	printf("B:%f\n", StateSpace.paraB);
	printf("C:%f\n", StateSpace.paraC);
	printf("D:%f\n", StateSpace.paraD);
	printf("U:%f\n", StateSpace.paraU);
	printf("Newest output is %f\n", StateSpace.yState[StateSpace.SamplingCounter]);
}

int main(void){
	//char check;
	ASA_M128_set();
	initStateSpace();
	setStateSpaceAllPara();
	printf("Main code start ~~~\n");

	while (1) {
		uint16_t LoopCounter = 0;
		printf("Please Enter Mode ...\n");
		int mode;
		scanf("%d", &mode);
		switch(mode){
			// 將目前設置的參數顯示出來
			case 0:
				printParameters();
			break;
			
			/*
				更新狀態空間資料，並將資料傳送至人機介面
			*/
			case 1:
				printf("Run Mode 0\n");
				while(LoopCounter < 10){
					updateStateSpace();
					sendStateSpaceDataToHMI();
					LoopCounter++;
					_delay_ms(500);
				}
			break;
			
			/*
				設定狀態空間參數
				格式範例如下:
ui16x1,f32x20,f32x20,f32x1,f32x1,f32x1,f32x1,f32x1:
ui16:
1
f32:
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
f32:
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
f32:
1
f32:
2
f32:
3
f32:
4
f32:
8
			*/
			case 2:
				printf("Run Mode 2\n");
				setStateSpaceParaUsingHMI();
				printParameters();
			break;
		}

    }
}
