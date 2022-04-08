#include "common.h"


uint16_t CellVoltage [16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

void BQ76952_Vcell(FILE* file)
{
    for(int i=0;i<16;i++)
    {
        fscanf(file,"%d",&CellVoltage[i]);
		writeDirectMemory(CellVoltage[i],0x14 + i*2);
//		readDirectMemory(0X14 + i*2);
    }
	
}

void BQ76952_Init()
{
    printf("\n*****  BQ76952 Funtion Model  *****\n\n");
    initMemory();
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

	while(1)
	{
		Command_Sequence();
	}
	
	return 0;    
}
