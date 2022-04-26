#include "common.h"

int input_counter = 0;                          //用于输入向量的计数

void BQ76952_Vcell
(					FILE* file,
					uint16_t *CellVoltage,
					int16_t *current,
					uint16_t *charger,
					uint16_t *LD)
{
	uint16_t VC[16];
	int16_t current_temp;
	uint16_t charger_temp;
	uint16_t LD_temp;
	
    for(int i=0;i<16;i++)
    {
        fscanf(file,"%hu",&VC[i]);
		*CellVoltage = VC[i];
		CellVoltage ++;
		//writeDirectMemory(CellVoltage[i],0x14 + i*2);
		//readDirectMemory(0x14 + i*2);
		
    }
	
    fscanf(file,"%hd",&current_temp);
    fscanf(file,"%hu",&charger_temp);
    fscanf(file,"%hu",&LD_temp);

	*current = current_temp;
	*charger = charger_temp;
	*LD = LD_temp;
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
	uint8_t CHG_ON,DSG_ON,PCHG_ON,PDSG_ON;
    FILE *fp;
    //fp = fopen("../sim/COV_COVL_TEST.txt","r");    //testcase自定义Vcell
    //fp = fopen("../sim/CUV_TEST.txt","r");
    //fp = fopen("../sim/SUPPLY_TEST.txt","r");
    fp = fopen("../sim/TEST_case.txt","r");
    if(fp==NULL)
    {  
		printf("File cannot open! " );  
		//exit;  
		return 0;
	}
	
	//printf("\n*****  BQ76952 Funtion Model  *****\n\n");
    initMemory();

	int cycle_counter = 0;
     while (!feof(fp))
    //while(1)
    {
        /*
        input_counter++;
	    printf("\nNext input %d... ...\n",input_counter);*/
		cycle_counter++;
		printf("\n#%d\n",cycle_counter);
		
        BQ76952_Vcell(fp,&CellVoltage,&current,&charger,&LD);  //Supply to VC1-VC16
        BQ76952(&CellVoltage,current,charger,LD,&CHG_ON,&DSG_ON,&PCHG_ON,&PDSG_ON);//DUT_BQ76952

		printf("CHG_ON = %d\n",CHG_ON);
		printf("DSG_ON = %d\n",DSG_ON);
		printf("PCHG_ON = %d\n",PCHG_ON);
		printf("PDSG_ON = %d\n",PDSG_ON);
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
				uint8_t *CUV_REC_HYS,
				uint8_t *COV_REC_HYS,
				uint16_t *CUV_DLY,
				uint16_t *COV_DLY,
				uint8_t *RecoveryTime,
				uint8_t *COVL_Limit,
				uint8_t *COVL_DEC_DLY,
				uint8_t *COVL_RecoveryTime,
				
				uint8_t *FET_ctrl_en,
				uint8_t *FET_init_off,
				uint8_t *FET_en,
				uint8_t *PDSG_en,
				uint8_t *CHG_protectionA,
				uint8_t *CHG_protectionB,
				uint8_t *CHG_protectionC,
				uint8_t *DSG_protectionA,
				uint8_t *DSG_protectionB,
				uint8_t *DSG_protectionC,
				int16_t *PCHG_StartVoltage,
				int16_t *PCHG_StopVoltage,
				uint8_t *PDSG_Timeout,
				uint8_t *PDSG_StopDelta,
				
				uint8_t *SCD_TH,                 
				uint8_t *SCD_DLY,
				uint8_t *SCD_REC_DLY,
				uint8_t *SCDL_Limit,
				uint8_t *SCDL_DEC_DLY,
				uint8_t *SCDL_RecoveryTime,
				int16_t *SCDL_REC_TH,
				uint16_t *SCDL_CURR_RECOV,
				uint8_t *OCD1_TH,                           
				uint8_t *OCD1_DLY,                            
				uint8_t *OCD2_TH,                          
				uint8_t *OCD2_DLY,                          
				uint8_t *OCD3_TH,                          
				uint8_t *OCD3_DLY,                            
				int16_t *OCD_REC_TH,                       
				uint8_t *OCDL_Limit,                          
				uint8_t *OCDL_DEC_DLY,                    
				uint8_t *OCDL_RecoveryTime,                 
				int16_t *OCDL_REC_TH,
				uint16_t *OCDL_CURR_RECOV,
				uint8_t *OCC_TH,
				uint8_t *OCC_DLY,
				int16_t *OCC_CREC_TH,
				int16_t *OCC_VREC_Delta,

				uint16_t *PTO_DLY,
				int16_t *PTO_Charge_TH,
				int16_t *PTO_RESET,
				int16_t *DSG_Current_TH
)
{
	*CUV_TH = 3000;         
    *COV_TH = 5000; 
    *CUV_REC_HYS = 0;
    *COV_REC_HYS = 0;
    *CUV_DLY = 2;
    *COV_DLY = 2; 
    *RecoveryTime = 3;      				  //保护的恢复延时
    
    *COVL_Limit = 4;        
    *COVL_DEC_DLY = 100; 
    *COVL_RecoveryTime = 150;
	
	*FET_ctrl_en = 1;
	*FET_init_off = 0;
	*FET_en = 1;
	*PDSG_en = 1;
	*CHG_protectionA = 0x98;
	*CHG_protectionB = 0xD5;
	*CHG_protectionC = 0x56;
	*DSG_protectionA = 0xE4;
	*DSG_protectionB = 0xE6;
	*DSG_protectionC = 0xE2;
	*PCHG_StartVoltage = 0;
	*PCHG_StopVoltage = 0;
	*PDSG_Timeout = 5;
	*PDSG_StopDelta = 50;

	*SCD_TH = 100;                            //单位mv,原始寄存器为译码值
	*SCD_DLY = 2;                             //实际延时很小 us 级别
	*SCD_REC_DLY = 2;                         //实际 s 级别延时
	
	*SCDL_Limit = 3;                          
	*SCDL_DEC_DLY = 100;                      //实际 s 级别延时
	*SCDL_RecoveryTime = 150;                 //实际 s 级别延时 Settings:Protection:Protection Configuration[SCDL_CURR_RECOV] 有效时该恢复机制才会起作用
	*SCDL_REC_TH = -10;                       //mV单位  原始寄存器单位为mA注意连接时单位
	
	uint16_t Protection_Configuration = 0x0002;								//#define ProtectionConfiguration 0x925F 
	*SCDL_CURR_RECOV = Protection_Configuration & (1<<10);                	//Protection_Configuration & (1<<10)    是否使能SCDL 基于电流恢复
	*OCDL_CURR_RECOV = Protection_Configuration & (1<<9);                //Protection_Configuration & (1<<9)    是否使能OCDL 基于电流恢复
	
	*OCD1_TH = 100;                           //单位2mv
	*OCD1_DLY = 2;                            //实际延时 ms 级别
	*OCD2_TH = 200;                           //单位2mv
	*OCD2_DLY = 2;                            //实际延时 ms 级别
	*OCD3_TH = -100;                          //基于CC1读数结果,单位userA(可配置)
	*OCD3_DLY = 2;                            //实际延时 s 级别
	*OCD_REC_TH = -10;                        //mV单位  原始寄存器单位为mA注意连接时单位
	
	*OCDL_Limit = 3;                          
	*OCDL_DEC_DLY = 100;                      //实际 s 级别延时
	*OCDL_RecoveryTime = 150;                 //实际 s 级别延时 Settings:Protection:Protection Configuration[OCDL_CURR_RECOV] 有效时该恢复机制才会起作用
	*OCDL_REC_TH = -10;                       //mV单位  原始寄存器单位为mA注意连接时单位

	*OCC_TH = 4;                              //mV
	*OCC_DLY = 2;                             //实际延时 ms 级别
	*OCC_CREC_TH = 1;                         // mV单位  原始寄存器单位为mA注意连接时单位
	*OCC_VREC_Delta = 200;                    // 10mV单位

	*PTO_DLY = 1800;
	*PTO_Charge_TH = 250;
	*PTO_RESET = 2;

	*DSG_Current_TH = 100;
}
void update_register
(
			   //input
			   const uint16_t *CellVoltage,
			   const uint8_t CHG_FET,
			   const uint8_t DSG_FET,
			   const uint8_t PCHG_FET,
			   const uint8_t PDSG_FET,
			   const uint8_t CUV_alert,
			   const uint8_t CUV_error,
			   const uint8_t COV_alert,
			   const uint8_t COV_error,
			   const uint8_t COVL_alert,
			   const uint8_t COVL_error,
			   const uint8_t SCD_alert,
			   const uint8_t SCD_error,
			   const uint8_t SCDL_alert,
			   const uint8_t SCDL_error,
			   const uint8_t OCD1_alert,
			   const uint8_t OCD1_error,
			   const uint8_t OCD2_alert,
			   const uint8_t OCD2_error,
			   const uint8_t OCD3_alert,
			   const uint8_t OCD3_error,
			   const uint8_t OCDL_alert,
			   const uint8_t OCDL_error,
			   const uint8_t OCC_alert,
			   const uint8_t OCC_error,
			   const uint8_t PTOS_alert,
			   const uint8_t PTOS_error
)
{
	//电池电压
	for(int i=0;i<16;i++)
	{
		writeDirectMemory(*CellVoltage,Cell1Voltage + i*2);
		CellVoltage++;
	}

	uint8_t safetyalertA;
	uint8_t safetystatusA;
	uint8_t safetyalertB;
	uint8_t safetystatusB;
	uint8_t safetyalertC;
	uint8_t safetystatusC;
	uint8_t fetstatus;

	safetyalertA = CUV_alert<<2 | COV_alert<<3 | OCC_alert<<4 | OCD1_alert<<5 | OCD2_alert<<6 | SCD_alert<<7;
	writeDirectMemory(safetyalertA, SafetyAlertA);

	safetystatusA = CUV_error<<2 | COV_error<<3 | OCC_error<<4 | OCD1_error<<5 | OCD2_error<<6 | SCD_error<<7;
	writeDirectMemory(safetystatusA, SafetyStatusA);


//	safetyalertB = UTC_alert | UTD_alert<<1 | UTINT_alert<<2 | OTC_alert<<3 | OTD_alert<<4 | OTINT_alert<<5 | OTF_alert<<6;
//	writeDirectMemory(safetyalertB, SafetyAlertB);

//	safetystatusB = UTC_error | UTD_error<<1 | UTINT_error<<2 | OTC_error<<3 | OTD_error<<4 | OTINT_error<<5 | OTF_error<<6;
//	writeDirectMemory(safetystatusB, SafetyStatusB);


	safetyalertC = PTOS_alert<<3 | COVL_alert<<4 | OCDL_alert<<5 | SCDL_alert<<6 | OCD3_alert<<7;
	writeDirectMemory(safetyalertC, SafetyAlertC);

	safetystatusC = PTOS_error<<3 | COVL_error<<4 | OCDL_error<<5 | SCDL_error<<6 | OCD3_error<<7;
	writeDirectMemory(safetystatusC, SafetyStatusC);

	fetstatus = CHG_FET | PCHG_FET<<1 | DSG_FET<<2 | PDSG_FET<<3 ;
	//	fetstatus = CHG_FET | PCHG_FET<<1 | DSG_FET<<2 | PDSG_FET<<3 | DCHG_PIN<<4 | DDSG_PIN<<5 | ALRT_PIN<<6;	
	writeDirectMemory(fetstatus, FETStatus);

}

void BQ76952
(
				//input
                const uint16_t *CellVoltage,       
//                const uint16_t TS1,             
//                const uint16_t TS2,             
//                const uint16_t TS3,             
                const int16_t current,          
                const uint16_t charger,         
				const uint16_t LD,             

//                const uint8_t DCHG,           
//                const uint8_t DDSG,          
				//output
                uint8_t *CHG_on,                
                uint8_t *DSG_on,               
                uint8_t *PCHG_on,               
                uint8_t *PDSG_on              
//                uint8_t *Alert 
)
{
	uint16_t CUV_TH;         
    uint16_t COV_TH; 
    uint8_t CUV_REC_HYS;
    uint8_t COV_REC_HYS;
    uint16_t CUV_DLY;
    uint16_t COV_DLY; 
    uint8_t RecoveryTime;      
    uint8_t COVL_Limit;        
    uint8_t COVL_DEC_DLY; 
    uint8_t COVL_RecoveryTime;

	uint8_t SCD_TH;            
	uint8_t SCD_DLY;           
	uint8_t SCD_REC_DLY;       
	
	uint8_t SCDL_Limit;
	uint8_t SCDL_DEC_DLY;
	uint8_t SCDL_RecoveryTime;
	int16_t SCDL_REC_TH;
	uint16_t SCDL_CURR_RECOV;

	uint8_t OCC_TH;
	uint8_t OCC_DLY;
	int16_t OCC_CREC_TH;
	int16_t OCC_VREC_Delta;
	
	uint8_t OCD1_TH;                           //单位2mv
	uint8_t OCD1_DLY;                            //实际延时 ms 级别
	uint8_t OCD2_TH;                           //单位2mv
	uint8_t OCD2_DLY ;                            //实际延时 ms 级别
	uint8_t OCD3_TH;                          //基于CC1读数结果,单位userA(可配置)
	uint8_t OCD3_DLY;                            //实际延时 s 级别
	int16_t OCD_REC_TH;                        //mV单位  原始寄存器单位为mA注意连接时单位

	uint8_t OCDL_Limit;                          
	uint8_t OCDL_DEC_DLY;                     //实际 s 级别延时
	uint8_t OCDL_RecoveryTime;                 //实际 s 级别延时 Settings:Protection:Protection Configuration[OCDL_CURR_RECOV] 有效时该恢复机制才会起作用
	int16_t OCDL_REC_TH;                       //mV单位  原始寄存器单位为mA注意连接时单位
	uint16_t OCDL_CURR_RECOV;
	
	uint8_t FET_ctrl_en;
	uint8_t FET_init_off;
	uint8_t FET_en;
	uint8_t PDSG_en;

	uint8_t CHG_protectionA;
	uint8_t CHG_protectionB;
	uint8_t CHG_protectionC;
	uint8_t DSG_protectionA;
	uint8_t DSG_protectionB;
	uint8_t DSG_protectionC;
	int16_t PCHG_StartVoltage;
	int16_t PCHG_StopVoltage;
	uint8_t PDSG_Timeout;
	uint8_t PDSG_StopDelta;

	uint16_t PTO_DLY;
	int16_t PTO_Charge_TH;
	int16_t PTO_RESET;
	
	int16_t DSG_Current_TH;
	
    uint8_t CUV_alert;
    uint8_t CUV_error;
    uint8_t COV_alert;
    uint8_t COV_error;
    uint8_t COVL_alert;
    uint8_t COVL_error;
	
    uint8_t SCD_alert;
    uint8_t SCD_error;
	uint8_t SCDL_alert;
	uint8_t SCDL_error;
	
	uint8_t OCD1_alert;
	uint8_t OCD1_error;
	uint8_t OCD2_alert;
	uint8_t OCD2_error;
	uint8_t OCD3_alert;
	uint8_t OCD3_error;
	uint8_t OCDL_alert;
	uint8_t OCDL_error;
	uint8_t OCC_alert;
	uint8_t OCC_error;
	
	uint8_t PTOS_alert;
	uint8_t PTOS_error;
	
	uint8_t CHG_ON;
	uint8_t DSG_ON;
	uint8_t PCHG_ON;
	uint8_t PDSG_ON;


	update_config(
				//output
				&CUV_TH,
				&COV_TH,
				&CUV_REC_HYS,
				&COV_REC_HYS,
				&CUV_DLY,
				&COV_DLY,
				&RecoveryTime,
				&COVL_Limit,
				&COVL_DEC_DLY,
				&COVL_RecoveryTime,
				&FET_ctrl_en,
				&FET_init_off,
				&FET_en,
				&PDSG_en,
				&CHG_protectionA,
				&CHG_protectionB,
				&CHG_protectionC,
				&DSG_protectionA,
				&DSG_protectionB,
				&DSG_protectionC,
				&PCHG_StartVoltage,
				&PCHG_StopVoltage,
				&PDSG_Timeout,
				&PDSG_StopDelta,
				&SCD_TH,
				&SCD_DLY,
				&SCD_REC_DLY,
				&SCDL_Limit,
				&SCDL_DEC_DLY,
				&SCDL_RecoveryTime,
				&SCDL_REC_TH,
				&SCDL_CURR_RECOV,
				&OCD1_TH,                           
				&OCD1_DLY,                            
				&OCD2_TH,                           
				&OCD2_DLY,                            
				&OCD3_TH,                         
				&OCD3_DLY,                            
				&OCD_REC_TH,                        
				&OCDL_Limit,                         
				&OCDL_DEC_DLY,                     
				&OCDL_RecoveryTime,                 
				&OCDL_REC_TH,
				&OCDL_CURR_RECOV,
				&OCC_TH,
				&OCC_DLY,
				&OCC_CREC_TH,
				&OCC_VREC_Delta,
				&PTO_DLY,
				&PTO_Charge_TH,
				&PTO_RESET,
				&DSG_Current_TH
				);
	
    CUV_protect(
				//input
				CellVoltage,		//VC1-VC16
				CUV_TH,				//阈值
				CUV_DLY,			//延时
				CUV_REC_HYS,			//恢复滞后电压
				RecoveryTime,		//恢复延时
				//output
				&CUV_alert,			//alert信号
				&CUV_error);		//error信号

    COV_protect(
				//input
				CellVoltage,		//VC1-VC16
    			COV_TH,				//阈值
    			COV_DLY,			//延时
    			COV_REC_HYS,			//恢复滞后电压
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

	SCD_protect(
				//input
				current,
				SCD_TH,
				SCD_DLY,
				SCD_REC_DLY,
				//output
				&SCD_alert,
				&SCD_error);
	SCDL_protect(
				//input
				current,
				LD,
				SCDL_Limit,
				SCDL_DEC_DLY,
				SCDL_RecoveryTime,
				SCDL_REC_TH,
				SCDL_CURR_RECOV,
				//output
				&SCDL_alert,
				&SCDL_error
				);
	OCD1_protect(
				//input
				current,
				OCD1_TH,
				OCD1_DLY,
				OCD_REC_TH,
				RecoveryTime,
				//output
				&OCD1_alert,
				&OCD1_error
				);
	OCD2_protect(
				//input
				current,
				OCD2_TH,
				OCD2_DLY,
				OCD_REC_TH,
				RecoveryTime,
				//output
				&OCD2_alert,
				&OCD2_error
				);
	
	int16_t CC1_current = -10;		//CC1读数结果,用于OCD3判断
	OCD3_protect(
				//input
				CC1_current,
				OCD3_TH,
				OCD3_DLY,
				OCD_REC_TH,
				RecoveryTime,
				//output
				&OCD3_alert,
				&OCD3_error
				);		
	OCDL_protect(
				//input
				current,
				LD,
				OCDL_Limit,
				OCDL_DEC_DLY,
				OCDL_RecoveryTime,
				OCDL_REC_TH,
				OCDL_CURR_RECOV,
				//output
				&OCDL_alert,
				&OCDL_error
				);
	
	int16_t Stack_Pack_Delta = 10;	//用于OCC的电压恢复机制（阈值OCC_VREC_Delta）
	OCC_protect(
				//input
				current,
				OCC_TH,
				OCC_DLY,
				OCC_CREC_TH,
				Stack_Pack_Delta,
				OCC_VREC_Delta,
			    RecoveryTime,
				//output
				&OCC_alert,
				&OCC_error
				);

	PTO_protect(
				//input
				CC1_current,
				current,
				PCHG_on,
				PTO_DLY,
				PTO_Charge_TH,
				DSG_Current_TH,
				PTO_RESET,
				//output
				&PTOS_alert,
				&PTOS_error
			   );
	
	uint8_t LD_TOS_Delta = 10;	//用于PDSG基于电压关闭机制（阈值PDSG_StopDelta）
	FET_auto_control(
				//input
				CellVoltage,
				LD_TOS_Delta,
				FET_ctrl_en,
				FET_init_off,
				FET_en,
				PDSG_en,
				CHG_protectionA,
				CHG_protectionB,
				CHG_protectionC,
				DSG_protectionA,
				DSG_protectionB,
				DSG_protectionC,
				COV_error,
				COVL_error,
				CUV_error,
				SCD_error,
				SCDL_error,
				OCD1_error,
				OCD2_error,
				OCD3_error,
				OCDL_error,
				OCC_error,
				PTOS_error,
				PCHG_StartVoltage,
				PCHG_StopVoltage,
				PDSG_Timeout,
				PDSG_StopDelta,
				//output
				&CHG_ON,
				&DSG_ON,
				&PCHG_ON,
				&PDSG_ON
				);
	
    update_register(
				//input
				&CellVoltage,
				CHG_ON,
				DSG_ON,
				PCHG_ON,
				PDSG_ON,
				CUV_alert,
				CUV_error,
				COV_alert,
				COV_error,
				COVL_alert,
				COVL_error,
				SCD_alert,
				SCD_error,
				SCDL_alert,
				SCDL_error,
				OCD1_alert,
				OCD1_error,
				OCD2_alert,
				OCD2_error,
				OCD3_alert,
				OCD3_error,
				OCDL_alert,
				OCDL_error,
				OCC_alert,
				OCC_error,
				PTOS_alert,
				PTOS_error
				);
	
	*CHG_on = CHG_ON;
	*DSG_on = DSG_ON;
	*PCHG_on = PCHG_ON;
	*PDSG_on = PDSG_ON;
	
}
