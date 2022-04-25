#include "common.h"

void COV_protect();
void CUV_protect();
void COVL_protect();

void SCD_protect();
void SCDL_protect();
void OCD1_protect();
void OCD2_protect();
void OCD3_protect();
void SCDL_protect();
void OCC_protect();

void FET_auto_control();

void Command_Sequence();
void BQ76952();



int input_counter = 0;                          //用于输入向量的计数

void BQ76952_Vcell(FILE* file,uint16_t *CellVoltage)
{
	uint16_t VC[16];
    for(int i=0;i<16;i++)
    {
        fscanf(file,"%hu",&VC[i]);
		*CellVoltage = VC[i];
		CellVoltage ++;
		//writeDirectMemory(CellVoltage[i],0x14 + i*2);
		//readDirectMemory(0x14 + i*2);
		
    }
	
    /*fscanf(file,"%hd",&current);
    fscanf(file,"%hu",&charger);
    fscanf(file,"%hu",&LD);*/

}

//void BQ76952_Init()
//{
//    printf("\n*****  BQ76952 Funtion Model  *****\n\n");
//    initMemory();
//	//initSettings();
//}

//void Print_input()
//{
//    for(int i=0;i<16;i++)
//    {
//        printf("VC%-4d",i+1);
//    }
//    printf("Vic");
//    printf("\n");
//    for(int i=0;i<16;i++)
//    {
//        printf("%-6hu",CellVoltage[i]);
//    }
//    printf(" %-6hd",current);
//    printf("\n");
//}

int main()
{
    uint16_t CellVoltage [16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	uint16_t charger,LD;                            //充电器检测结果与负载检测结果
	int16_t  current;                               //电流结果输入

    FILE *fp;
    fp = fopen("../sim/COV_COVL_TEST.txt","r");    //testcase自定义Vcell
    //fp = fopen("../sim/CUV_TEST.txt","r");
    //fp = fopen("../sim/SUPPLY_TEST.txt","r");
    //fp = fopen("../sim/TEST_case.txt","r");
    if(fp==NULL)
    {  
		printf("File cannot open! " );  
		//exit;  
		return 0;
	}
	
	//printf("\n*****  BQ76952 Funtion Model  *****\n\n");
    initMemory();
	
     while (!feof(fp))
    //while(1)
    {
        /*
        input_counter++;
	    printf("\nNext input %d... ...\n",input_counter);*/

        BQ76952_Vcell(fp,&CellVoltage);  //Supply to VC1-VC16
        BQ76952(&CellVoltage);
    }
//等待外部输入命令序列使用示例
//	while(1)
//	{
//		Command_Sequence();
//	}

////内部读写二级寄存器函数使用示例
//	uint8_t wdata[4] ={0x4a,0x71,0x31,0x90};
//	uint8_t rdata[4];
//	writeSubMemory(0x0071, 0, wdata, 4);
//	uint8_t *pdata = readSubMemory(0x0071, 0, 4);
//	for(int i=0;i<4;i++)
//	{
//		rdata[i] = *pdata;
//		printf("rdata[%d] = %#x\n",i,rdata[i]);
//		pdata ++;
//	}
}
void update_config
(
				uint16_t *CUV_TH,
				uint16_t *COV_TH,
				uint16_t *CUV_REC,
				uint16_t *COV_REC,
				uint16_t *CUV_DLY,
				uint16_t *COV_DLY,
				uint8_t *RecoveryTime,
				
				uint8_t *COVL_Limit,
				uint8_t *COVL_DEC_DLY,
				uint8_t *COVL_RecoveryTime,
				uint8_t *FET_ctrl_en,
				uint8_t *FET_init_off,
				uint8_t *FET_en,
				uint8_t *CHG_protectionA,
				uint8_t *CHG_protectionB,
				uint8_t *CHG_protectionC,
				uint8_t *DSG_protectionA,
				uint8_t *DSG_protectionB,
				uint8_t *DSG_protectionC
)
{
	*CUV_TH = 3000;         
    *COV_TH = 5000; 
    *CUV_REC = 0;
    *COV_REC = 0;
    *CUV_DLY = 2;
    *COV_DLY = 2; 
    *RecoveryTime = 30;      //保护的恢复延时
    
    *COVL_Limit = 4;        
    *COVL_DEC_DLY = 100; 
    *COVL_RecoveryTime = 150;
	
	*FET_ctrl_en = 1;
	*FET_init_off = 0;
	*FET_en = 1;

	*CHG_protectionA = 0x98;
	*CHG_protectionB = 0xD5;
	*CHG_protectionC = 0x56;
	*DSG_protectionA = 0xE4;
	*DSG_protectionB = 0xE6;
	*DSG_protectionC = 0xE2;
}
void update_register
(
			   //input
			   const uint16_t *CellVoltage,
			   const uint8_t CUV_alert,
			   const uint8_t CUV_error,
			   const uint8_t COV_alert,
			   const uint8_t COV_error,
			   const uint8_t COVL_alert,
			   const uint8_t COVL_error,
			   const uint8_t CHG_ON,
			   const uint8_t DSG_ON
)
{
}
void BQ76952
(
                const uint16_t *CellVoltage       //input
//                const uint16_t TS1,             //input
//                const uint16_t TS2,             //input
//                const uint16_t TS3,             //input
//                const uint16_t LD,              //input
//                const uint16_t charger,         //input
//                const int16_t current,          //input
//                const uint8_t DCHG,             //input  
//                const uint8_t DDSG,             //input
//
//                uint8_t *CHG_on,                //output
//                uint8_t *DSG_on                 //output
//                uint8_t *PCHG_on,               //output
//                uint8_t *PDSG_on,               //output
//                uint8_t *Alert                  //output
)
{
	uint16_t CUV_TH;         
    uint16_t COV_TH; 
    uint16_t CUV_REC;
    uint16_t COV_REC;
    uint16_t CUV_DLY;
    uint16_t COV_DLY; 
    uint8_t RecoveryTime;      //保护的恢复延时
    
    uint8_t COVL_Limit;        
    uint8_t COVL_DEC_DLY; 
    uint8_t COVL_RecoveryTime;
	
	uint8_t FET_ctrl_en;
	uint8_t FET_init_off;
	uint8_t FET_en;

	uint8_t CHG_protectionA;
	uint8_t CHG_protectionB;
	uint8_t CHG_protectionC;
	uint8_t DSG_protectionA;
	uint8_t DSG_protectionB;
	uint8_t DSG_protectionC;
	
    uint8_t CUV_alert;
    uint8_t CUV_error;
    uint8_t COV_alert;
    uint8_t COV_error;
    uint8_t COVL_alert;
    uint8_t COVL_error;

	uint8_t CHG_ON;
	uint8_t DSG_ON;


	update_config(
				//output
				&CUV_TH,
				&COV_TH,
				&CUV_REC,
				&COV_REC,
				&CUV_DLY,
				&COV_DLY,
				&RecoveryTime,
				&COVL_Limit,
				&COVL_DEC_DLY,
				&COVL_RecoveryTime,
				&FET_ctrl_en,
				&FET_init_off,
				&FET_en,
				&CHG_protectionA,
				&CHG_protectionB,
				&CHG_protectionC,
				&DSG_protectionA,
				&DSG_protectionB,
				&DSG_protectionC
				);
	
    CUV_protect(
				//input
				CellVoltage,		//VC1-VC16
				CUV_TH,				//阈值
				CUV_DLY,			//延时
				CUV_REC,			//恢复滞后电压
				RecoveryTime,		//恢复延时
				//output
				&CUV_alert,			//alert信号
				&CUV_error);		//error信号

    COV_protect(
				//input
				CellVoltage,		//VC1-VC16
    			COV_TH,				//阈值
    			COV_DLY,			//延时
    			COV_REC,			//恢复滞后电压
    			RecoveryTime,		//恢复延时
    			//output
    			&COV_alert,			//alert信号
    			&COV_error);		//alert信号

    COVL_protect(
				//input
				COVL_Limit,			//触发COVL错误的计数上限
    			COVL_DEC_DLY,		//COV错误计数递减的时间间隔
    			COVL_RecoveryTime,	//自动恢复延时
    			//output
    			&COVL_alert,		//alert信号
    			&COVL_error);		//error信号
    			
    /*SCD_protect();
    SCDL_protect();
    OCD1_protect();
    OCD2_protect();
    OCD3_protect();
    SCDL_protect();
    OCC_protect();*/			

	FET_auto_control(
				//input
				FET_ctrl_en,
				FET_init_off,
				FET_en,
				CHG_protectionA,
				CHG_protectionB,
				CHG_protectionC,
				DSG_protectionA,
				DSG_protectionB,
				DSG_protectionC,
				COV_error,
				COVL_error,
				CUV_error,
				//output
				&CHG_ON,
				&DSG_ON);


    
//    update_register(
//				//input
//				&CellVoltage,
//				CUV_alert,
//				CUV_error,
//				COV_alert,
//				COV_error,
//				COVL_alert,
//				COVL_error,
//				CHG_ON,
//				DSG_ON
//				);
	
//	*CHG_on = CHG_ON;
//	*DSG_on = DSG_ON;

}
