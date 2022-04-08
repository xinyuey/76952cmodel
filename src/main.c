#include "common.h"


uint16_t CellVoltage [16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

int input_counter = 0;

void BQ76952_Vcell(FILE* file)
{
    for(int i=0;i<16;i++)
    {
        fscanf(file,"%d",&CellVoltage[i]);
		writeDirectMemory(CellVoltage[i],0x14 + i*2);
//		readDirectMemory(0X14 + i*2);
    }
    
//	printf("\n Next input %d... ...\n",input_counter);
}

void BQ76952_Init()
{
    printf("\n*****  BQ76952 Funtion Model  *****\n\n");
    initMemory();
}

void PrintVcell()
{
    for(int i=0;i<16;i++)
    {
        printf("VC%-6d",i+1);
    }
    printf("\n");
    for(int i=0;i<16;i++)
    {
        printf("%-8d",CellVoltage[i]);
    }
    printf("\n");
}

uint8_t main(int argc,char* argv)
{
    
    BQ76952_Init();

    FILE *fp;
    fp = fopen("../sim/COV_COVL_TEST.txt","r");    //testcase自定义Vcell
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
        BQ76952_Vcell(fp);  //Supply to VC1-VC16
        //PrintVcell();
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

	uint8_t command;
	while(1)
	{
			printf("waiting for command......\n");
			scanf("%x",&command);
			readDirectMemory(command);
			
	}
    
}