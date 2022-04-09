#include "common.h"

uint8_t COV_protect();
uint8_t CUV_protect();
uint8_t COVL_protect();


uint8_t COV_protect();
uint8_t CUV_protect();
uint8_t COVL_protect();


uint16_t CellVoltage [16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint16_t charger,LD;                            //充电器检测结果与负载检测结果
int16_t  current;                               //电流结果输入

int input_counter = 0;                          //用于输入向量的计数

void BQ76952_Vcell(FILE* file)
{
    for(int i=0;i<16;i++)
    {
        fscanf(file,"%hu",&CellVoltage[i]);
		writeDirectMemory(CellVoltage[i],0x14 + i*2);
//		readDirectMemory(0X14 + i*2);
    }
<<<<<<< HEAD
    fscanf(file,"%hd",&current);
    fscanf(file,"%hu",&charger);
    fscanf(file,"%hu",&LD);
=======

    /*fscanf(file,"%hd",&current);
    fscanf(file,"%hu",&charger);
    fscanf(file,"%hu",&LD);*/
}

void Print_input()
{
    for(int i=0;i<16;i++)
    {
        printf("VC%-4d",i+1);
    }
    printf("Vic");
    printf("\n");
    for(int i=0;i<16;i++)
    {
        printf("%-6hu",CellVoltage[i]);
    }
    printf(" %-6hd",current);
    printf("\n");
>>>>>>> master
}

void BQ76952_Init()
{
    printf("\n*****  BQ76952 Funtion Model  *****\n\n");
    initMemory();
}

void Print_input()
{
    for(int i=0;i<16;i++)
    {
        printf("VC%-4d",i+1);
    }
    printf("Vic");
    printf("\n");
    for(int i=0;i<16;i++)
    {
        printf("%-6hu",CellVoltage[i]);
    }
    printf(" %-6hd",current);
    printf("\n");
}

int main()
{
    
    BQ76952_Init();

    FILE *fp;
    fp = fopen("../sim/TEST_case.txt","r");    //testcase自定义Vcell
    //fp = fopen("CUV_TEST.txt","r");
    //fp = fopen("SUPPLY_TEST.txt","r");
    if(fp==NULL)
    {  
		printf("File cannot open! " );  
		//exit;  
		return 0;
	}

		
    while (!feof(fp))
    //while(1)
    {
<<<<<<< HEAD
        input_counter++;
	    printf("\nNext input %d... ...\n",input_counter);

        BQ76952_Vcell(fp);  //Supply to VC1-VC16
        Print_input();
=======
        /*
        input_counter++;
	    printf("\nNext input %d... ...\n",input_counter);*/

        BQ76952_Vcell(fp);  //Supply to VC1-VC16

        //Print_input();
>>>>>>> master
        CUV_protect();
		//readDirectMemory(SafetyAlertA);
		//readDirectMemory(SafetyStatusA);

        COV_protect();
        COVL_protect();	
		//readDirectMemory(SafetyAlertA);
		//readDirectMemory(SafetyStatusA);
		//readDirectMemory(SafetyAlertC);
		//readDirectMemory(SafetyStatusC);
    }

	while(1)
	{
		Command_Sequence();
	}
	
	return 0;    
}
