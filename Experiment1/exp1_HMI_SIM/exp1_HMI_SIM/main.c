/*
 * exp1_HMI_SIM.c
 *
 * Created: 2018/8/28 下午 05:02:06
 * Author : zxpay
 */ 
#define F_CPU 11059200
#include <avr/io.h>
#include "ASA_Lib.h"
#define ParaARowSize 2
#define ParaAColSize 2
#define ParaBRowSize 2
#define ParaBColSize 1
#define ParaCRowSize 1
#define ParaCColSize 2
#define ParaDRowSize 1
#define ParaDColSize 1
#define ParaXRowSize 2
#define ParaXColSize 1
#define ParaYRowSize 1
#define ParaYColSize 1
#define ParaURowSize 1
#define ParaUColSize 1
#define ExecuteTimes 150

void setMatrixTheSame(float *mat1, float *mat2, int row, int col);
void MatDotProduct(float* ptr_m1, float* ptr_m2, float* ptr_dot, int row1, int col1, int col2);
void testDotProduct();
void testPlusMat();

struct StateSpace{
	uint16_t SamplingCounter;
	float xState[ExecuteTimes+1][2];
	float yState[ExecuteTimes][1];
	float paraA[ParaARowSize][ParaAColSize];
	float paraB[ParaBRowSize][ParaBColSize];
	float paraC[ParaCRowSize][ParaCColSize];
	float paraD[ParaDRowSize][ParaDColSize];
	float paraU[1][ExecuteTimes]; // input
}StateSpace;

struct sendStruct{
	uint16_t C;
	float X0;
	float X1;
	float y;
}sendStruct;

void printParameter(){
	printf("===============================\n");
	for(int i=0;i<ParaARowSize;i++){
		for(int j=0;j<ParaAColSize;j++){
			printf("A[%d][%d]=%f\n", i, j, StateSpace.paraA[i][j]);
		}
	}
	printf("===============================\n");
	for(int i=0;i<ParaBRowSize;i++){
		for(int j=0;j<ParaBColSize;j++){
			printf("B[%d][%d]=%f\n", i, j, StateSpace.paraB[i][j]);
		}
	}
	printf("===============================\n");
	for(int i=0;i<ParaCRowSize;i++){
		for(int j=0;j<ParaCColSize;j++){
			printf("C[%d][%d]=%f\n", i, j, StateSpace.paraC[i][j]);
		}
	}
	printf("===============================\n");
	for(int i=0;i<ParaDRowSize;i++){
		for(int j=0;j<ParaDColSize;j++){
			printf("D[%d][%d]=%f\n", i, j, StateSpace.paraD[i][j]);
		}
	}
	printf("===============================\n");
}

void setMatrixTheSame(float *mat1, float *mat2, int row, int col){
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			*(mat1+j+i*col) = *(mat2+j+i*col);
		}
	}
}

void initStateStateInput(){
	for(int i=0;i<ExecuteTimes;i++){
		if(i==0){
			StateSpace.paraU[0][0] = 0;
		}
		else{
			StateSpace.paraU[0][i] = 1;
		}
	}
}

void MatDotProduct(float* ptr_m1, float* ptr_m2, float* ptr_dot, int row1, int col1, int col2){
	float dot_ans[row1][col2];
	float m1[row1][col1], m2[col1][col2];    // ROW2 = COL1
	//ptr_m = (float*)malloc(sizeof(float)*row1*col2);
	// Get value from matrix 1
	for(int i=0;i<row1;i++){
		for(int j=0;j<col1;j++){
			m1[i][j] = *(ptr_m1+i*col1+j);
			//printf("%d, %d, %.2f\n",i+1, j+1, m1[i][j]);
		}
	}
	// Get value from matrix 2
	for(int i=0;i<col1;i++){
		for(int j=0;j<col2;j++){
			m2[i][j] = *(ptr_m2+i*col2+j);
			//printf("%d, %d, %.2f\n",i+1, j+1, m2[i][j]);
		}
	}
	// Execute the dot product
	for(int i=0;i<row1;i++){
		for(int j=0;j<col2;j++){
			dot_ans[i][j] = 0;
			for(int k=0;k<col1;k++){    // summation
				dot_ans[i][j] += m1[i][k] * m2[k][j];
			}
			ptr_dot[i*col2+j] = dot_ans[i][j];
			//printf("%d,%d,%.2f,  %p\n",i+1,j+1,dot_ans[i][j], &dot_ans[i][j]);
		}
	}
}

void MatPlus(float* ptr_m1, float* ptr_m2, float* ptr_dot, int row, int col){
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			ptr_dot[j+i*col] = *(ptr_m1+j+i*col) + *(ptr_m2+j+i*col);
		}
	}
}

void updateStateSpace(int counter){
	float buffer1[ParaARowSize][ParaXColSize];   // A*X
	MatDotProduct(&StateSpace.paraA[0][0], &StateSpace.xState[counter][0],
		&buffer1[0][0], ParaARowSize, ParaAColSize, ParaXColSize);
		
	float buffer2[ParaBRowSize][ParaUColSize];  // B*U
	MatDotProduct(&StateSpace.paraB[0][0], &StateSpace.paraU[0][counter],
		&buffer2[0][0], ParaBRowSize, ParaBColSize, ParaUColSize);
		
	MatPlus(&buffer1[0][0], &buffer2[0][0], &StateSpace.xState[counter+1][0],
		ParaXRowSize, ParaXColSize);
	
	float buffer3[ParaCRowSize][ParaXColSize];  // C*X
	MatDotProduct(&StateSpace.paraC[0][0], &StateSpace.xState[counter][0],
		&buffer3[0][0], ParaCRowSize, ParaCColSize, ParaXColSize);
		
	float buffer4[ParaDRowSize][ParaUColSize];  // DXU
	MatDotProduct(&StateSpace.paraD[0][0], &StateSpace.xState[counter][0],
		&buffer4[0][0], ParaDRowSize, ParaDColSize, ParaUColSize);
		
	MatPlus(&buffer3[0][0], &buffer4[0][0], &StateSpace.yState[counter][0],
		ParaYRowSize, ParaYColSize);
}
int main(void){
	ASA_M128_set();
	printf("Main code ~\n");
	float ParameterA[ParaARowSize][ParaAColSize] = {{1.35, 0.55}, {-0.45, 0.35}};
	float ParameterB[ParaBRowSize][ParaBColSize] = {{0.5}, {0.5}};
	float ParameterC[ParaCRowSize][ParaCColSize] = {{3, 1}};
	float ParameterD[ParaDRowSize][ParaDColSize] = {{1}};
	StateSpace.xState[0][0] = 0;
	StateSpace.xState[0][1] = 0;
	testDotProduct();
	testPlusMat();
	printf("Initialize parameters ...\n");
	setMatrixTheSame(&StateSpace.paraA[0][0], &ParameterA[0][0], ParaARowSize, ParaAColSize);
	setMatrixTheSame(&StateSpace.paraB[0][0], &ParameterB[0][0], ParaBRowSize, ParaBColSize);
	setMatrixTheSame(&StateSpace.paraC[0][0], &ParameterC[0][0], ParaCRowSize, ParaCColSize);
	setMatrixTheSame(&StateSpace.paraD[0][0], &ParameterD[0][0], ParaDRowSize, ParaDColSize);
	printParameter();
	initStateStateInput();
    while (1) {
		for(int counter=0;counter<ExecuteTimes;counter++){
			updateStateSpace(counter);
			printf("%.2f, %.2f, %.2f\n", StateSpace.xState[counter][0], StateSpace.xState[counter][1],
									StateSpace.yState[counter][0]);
			sendStruct.C = counter;
			sendStruct.X0 = StateSpace.xState[counter][0];
			sendStruct.X1 = StateSpace.xState[counter][1];
			sendStruct.y = StateSpace.yState[counter][0];
			char check = M128_HMI_Form_put("ui16x1,f32x1,f32x1,f32x1", 14, &sendStruct);
			if(check) printf("Error -> %d\n", check);
		}
		_delay_ms(30000);
    }
}

void testDotProduct(){
	float mat1[3][2] = {{1,5}, {2.2,5.99}, {5.55, 8}};
	float mat2[2][2] = {{2,5}, {-1, -2}};
	float ans[3][2] = {{-3, -5}, {-1.59, -0.98}, {3.1, 11.75}};
	float buffer[3][2];
	MatDotProduct(&mat1[0][0], &mat2[0][0], &buffer[0][0], 3, 2, 2);
	int correctCNT = 0;
	for(int i=0;i<3;i++){
		for(int j=0;j<2;j++){
			float ratio = ans[i][j]/buffer[i][j];
			if(ratio > 0.999 && ratio < 1.001){
				correctCNT++;
			}
			//printf("=== %f, %f ===\n", ans[i][j], buffer[i][j]);
		}
	}
	if(correctCNT == 6){
		printf("Test Dot Product Success !!!\n");
	}
	else{
		printf("Test Dot Product Fail !!!\n");
	}
}
void testPlusMat(){
	float mat1[3][2] = {{1,5}, {2.2,5.99}, {5.55, 8}};
	float mat2[3][2] = {{2,5}, {-1, -2}, {2.23, 3.55}};
	float ans[3][2] = {{3, 10}, {1.2, 3.99}, {7.78, 11.55}};
	float buffer[3][2];
	MatPlus(&mat1[0][0], &mat2[0][0], &buffer[0][0], 3, 2);
	int correctCNT = 0;
	for(int i=0;i<3;i++){
		for(int j=0;j<2;j++){
			float ratio = ans[i][j]/buffer[i][j];
			if(ratio > 0.999 && ratio < 1.001){
				correctCNT++;
			}
			//printf("=== %f, %f ===\n", ans[i][j], buffer[i][j]);
		}
	}
	if(correctCNT == 6){
		printf("Test Plus Success !!!\n");
	}
	else{
		printf("Test Plus Fail !!!\n");
	}
}

