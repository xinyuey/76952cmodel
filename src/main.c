#include "common.h"

#define RSPN 200	//敏感电阻阻值 单位毫欧
#define RST_SHUT_1S 10

int input_counter = 0;                          //用于输入向量的计数

int RS_counter_h = 0;

void BQ76952_Vcell
(					FILE* file,
					uint16_t *CellVoltage,
					int16_t *current,
					uint16_t *charger,
					uint16_t *LD,
					int32_t *BAT_Pin,
					int32_t *PACK_Pin,
					int32_t *LD_Pin,
					int16_t *TS1,
					int16_t *TS2,
					int16_t *TS3)
{
	uint16_t VC[16];
	int16_t current_temp;
	uint16_t charger_temp,LD_temp;
	int32_t BAT_temp,PACK_temp,LD_p_temp;
	int16_t TS1_temp,TS2_temp,TS3_temp;
	
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
    fscanf(file,"%hd",&BAT_temp);
    fscanf(file,"%hd",&PACK_temp);
    fscanf(file,"%hd",&LD_temp);
	fscanf(file,"%hd",&TS1_temp);
	fscanf(file,"%hd",&TS2_temp);
	fscanf(file,"%hd",&TS3_temp);
	*current = current_temp;
	*charger = charger_temp;
	*LD = LD_temp;
	*BAT_Pin = BAT_temp;
	*PACK_Pin = PACK_temp;
	*LD_Pin = LD_p_temp;
	*TS1 = TS1_temp;
	*TS2 = TS2_temp;
	*TS3 = TS3_temp;
	

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
	int16_t TS1,TS2,TS3;
	int32_t BAT_Pin,PACK_Pin,LD_Pin;
	uint8_t CHG_ON,DSG_ON,PCHG_ON,PDSG_ON;
	uint8_t FUSE = 0;
	uint8_t ALERT;
    FILE *fp;
    //fp = fopen("../sim/COV_COVL_TEST.txt","r");    //testcase自定义Vcell
    //fp = fopen("../sim/CUV_TEST.txt","r");
    //fp = fopen("../sim/SUPPLY_TEST.txt","r");
    //fp = fopen("../sim/TEST_case.txt","r");
	fp = fopen("../sim/testcase.txt","r");
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
		cycle_counter++;
		printf("\n#%d\n",cycle_counter);
		
		BQ76952_Vcell(fp,&CellVoltage,&current,&charger,&LD,&BAT_Pin,&PACK_Pin,&LD_Pin,&TS1,&TS2,&TS3);  //Supply to VC1-VC16
        BQ76952(&CellVoltage,current,charger,LD,BAT_Pin,PACK_Pin,LD_Pin,TS1,TS2,TS3,&CHG_ON,&DSG_ON,&PCHG_ON,&PDSG_ON,&FUSE,&ALERT);//DUT_BQ76952
		printf("\nCHG_ON = %d\n",CHG_ON);
		printf("DSG_ON = %d\n",DSG_ON);
		printf("PCHG_ON = %d\n",PCHG_ON);
		printf("PDSG_ON = %d\n",PDSG_ON);
		printf("FUSE = %d\n",FUSE);
		printf("ALERT = %d\n",ALERT);
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
				//output
				//FET控制
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
				//电压保护
				uint8_t *CUV_EN,
				uint8_t *COV_EN,
				uint8_t *COVL_EN,
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
				//电流保护
				uint8_t *SCD_EN,
				uint8_t *SCD_TH,                 
				uint8_t *SCD_DLY,
				uint8_t *SCD_REC_DLY,
				uint8_t *SCDL_EN,
				uint8_t *SCDL_Limit,
				uint8_t *SCDL_DEC_DLY,
				uint8_t *SCDL_RecoveryTime,
				int16_t *SCDL_REC_TH,
				uint16_t *SCDL_CURR_RECOV,
				uint8_t *OCD1_EN,
				uint8_t *OCD1_TH,                           
				uint8_t *OCD1_DLY, 
				uint8_t *OCD2_EN,
				uint8_t *OCD2_TH,                          
				uint8_t *OCD2_DLY, 
				uint8_t *OCD3_EN,
				int16_t *OCD3_TH,                          
				uint8_t *OCD3_DLY,                            
				int16_t *OCD_REC_TH, 
				uint8_t *OCDL_EN,
				uint8_t *OCDL_Limit,                          
				uint8_t *OCDL_DEC_DLY,                    
				uint8_t *OCDL_RecoveryTime,                 
				int16_t *OCDL_REC_TH,
				uint16_t *OCDL_CURR_RECOV,
				uint8_t *OCC_EN,
				uint8_t *OCC_TH,
				uint8_t *OCC_DLY,
				int16_t *OCC_CREC_TH,
				int16_t *OCC_VREC_Delta,
				//温度保护
				uint8_t *TS1_Config,
				uint8_t *TS2_Config,
				uint8_t *TS3_Config,
				uint8_t	*TINT_EN,							
				uint8_t *TINT_FETT,
				uint8_t *OTC_EN,
				int8_t *OTC_TH,
				uint8_t *OTC_DLY,
				int8_t *OTC_REC_TH,
				uint8_t *OTD_EN,
				int8_t *OTD_TH,
				uint8_t *OTD_DLY,
				int8_t *OTD_REC_TH,
				uint8_t *OTF_EN,
				int8_t *OTF_TH,
				uint8_t *OTF_DLY,
				int8_t *OTF_REC_TH,
				uint8_t *UTC_EN,
				int8_t *UTC_TH,
				uint8_t *UTC_DLY,
				int8_t *UTC_REC_TH,
				uint8_t *UTD_EN,
				int8_t *UTD_TH,
				uint8_t *UTD_DLY,
				int8_t *UTD_REC_TH,
				uint8_t *OTINT_EN,
				int8_t *OTINT_TH,
				uint8_t *OTINT_DLY,
				int8_t *OTINT_REC_TH,
				uint8_t *UTINT_EN,
				int8_t *UTINT_TH,
				uint8_t *UTINT_DLY,
				int8_t *UTINT_REC_TH,
				//预充电超时保护
				uint8_t *PTO_EN,
				uint16_t *PTO_DLY,
				int16_t *PTO_Charge_TH,
				int16_t *PTO_RESET,
				int16_t *DSG_Current_TH,
				//二次保护
				uint8_t *PF_EN,
				uint8_t *PF_FETS,
				uint8_t *PF_FUSE,
				uint8_t *PF_DPSLP,
				uint8_t *PF_REGS,
				//二次过压保护
				uint8_t *SOV_EN,
				int16_t *SOV_TH,
				uint8_t *SOV_DLY,
				//二次欠压保护
				uint8_t *SUV_EN,
				int16_t *SUV_TH,
				uint8_t *SUV_DLY,
				//二次保护器故障
				uint8_t *LVL2_EN,
				uint8_t *LVL2_DLY,
				//警报源使能掩码
			   	uint8_t *SF_ALERT_MASKA,
			    uint8_t *SF_ALERT_MASKB,
			   	uint8_t *SF_ALERT_MASKC,
			   	uint8_t *PF_ALERT_MASKA
)
{
	*CUV_EN = 1;
	*COV_EN = 1;
	*CUV_TH = 3000;         
    *COV_TH = 5000; 
    *CUV_REC_HYS = 0;
    *COV_REC_HYS = 0;
    *CUV_DLY = 2;
    *COV_DLY = 2; 
    *RecoveryTime = 3;      				  //保护的恢复延时
    
    *COVL_EN = 1;
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

	*SCD_EN = 1;
	*SCD_TH = 100;                            //单位mv,原始寄存器为译码值
	*SCD_DLY = 2;                             //实际延时很小 us 级别
	*SCD_REC_DLY = 2;                         //实际 s 级别延时

	*SCDL_EN = 1;
	*SCDL_Limit = 3;                          
	*SCDL_DEC_DLY = 100;                      //实际 s 级别延时
	*SCDL_RecoveryTime = 150;                 //实际 s 级别延时 Settings:Protection:Protection Configuration[SCDL_CURR_RECOV] 有效时该恢复机制才会起作用
	*SCDL_REC_TH = -10;                       //mV单位  原始寄存器单位为mA注意连接时单位
	
	uint16_t Protection_Configuration = 0x0002;								//#define ProtectionConfiguration 0x925F 
	*SCDL_CURR_RECOV = Protection_Configuration & (1<<10);                	//Protection_Configuration & (1<<10)    是否使能SCDL 基于电流恢复
	*OCDL_CURR_RECOV = Protection_Configuration & (1<<9);                //Protection_Configuration & (1<<9)    是否使能OCDL 基于电流恢复

	*OCD1_EN = 1;
	*OCD1_TH = 100;                           //单位2mv
	*OCD1_DLY = 2;                            //实际延时 ms 级别
	*OCD2_EN = 1;
	*OCD2_TH = 200;                           //单位2mv
	*OCD2_DLY = 2;                            //实际延时 ms 级别
	*OCD3_EN = 1;
	*OCD3_TH = -100;                          //基于CC1读数结果,单位userA(可配置)
	*OCD3_DLY = 2;                            //实际延时 s 级别
	*OCD_REC_TH = -10;                        //mV单位  原始寄存器单位为mA注意连接时单位

	*OCDL_EN = 1;
	*OCDL_Limit = 3;                          
	*OCDL_DEC_DLY = 100;                      //实际 s 级别延时
	*OCDL_RecoveryTime = 150;                 //实际 s 级别延时 Settings:Protection:Protection Configuration[OCDL_CURR_RECOV] 有效时该恢复机制才会起作用
	*OCDL_REC_TH = -10;                       //mV单位  原始寄存器单位为mA注意连接时单位

	*OCC_EN = 1;
	*OCC_TH = 100;                              //mV
	*OCC_DLY = 2;                             //实际延时 ms 级别
	*OCC_CREC_TH = 1;                         // mV单位  原始寄存器单位为mA注意连接时单位
	*OCC_VREC_Delta = 200;                    // 10mV单位

	*PTO_EN = 1;
	*PTO_DLY = 1800;
	*PTO_Charge_TH = 250;
	*PTO_RESET = 2;

	*DSG_Current_TH = 100;
	
	*TS1_Config = 0x07; 
	*TS2_Config = 0x00;
	*TS3_Config = 0x00;
	*TINT_EN = 0;							
	*TINT_FETT = 0;	
	*OTC_EN = 1;
	*OTC_TH = 55;
	*OTC_DLY = 2;
	*OTC_REC_TH = 50;
	*OTD_EN = 1;
	*OTD_TH = 60;
	*OTD_DLY = 2;
	*OTD_REC_TH = 55; 
	*OTF_EN = 1;
	*OTF_TH = 80;
	*OTF_DLY = 2;
	*OTF_REC_TH = 65; 
	*UTC_EN = 1;
	*UTC_TH = 0;
	*UTC_DLY = 2;
	*UTC_REC_TH = 5; 
	*UTD_EN = 1;
	*UTD_TH = 0;
	*UTD_DLY = 2;
	*UTD_REC_TH = 5; 
	*OTINT_EN = 1;
	*OTINT_TH = 85;
	*OTINT_DLY = 2;
	*OTINT_REC_TH = 80;
	*UTINT_EN = 1;
	*UTINT_TH = -20;
	*UTINT_DLY = 2;
	*UTINT_REC_TH = -15;

	*PF_EN = 1;
	*PF_FETS = 0;
	*PF_FUSE = 1;
	*PF_DPSLP = 0;
	*PF_REGS = 1;
	*SOV_EN = 1;
	*SOV_TH = 4500;
	*SOV_DLY = 5;
	*SUV_EN = 1;
	*SUV_TH = 2200;
	*SUV_DLY = 5;
	*LVL2_EN = 1;
	*LVL2_DLY = 5;

   	*SF_ALERT_MASKA = 0xFC;
   	*SF_ALERT_MASKB = 0xF7;
   	*SF_ALERT_MASKC = 0xF4;
   	*PF_ALERT_MASKA = 0x5F;
}
void update_register
(
			   	//input
			   	//电池电压
			   	const uint16_t *CellVoltage,
			   	//FET控制
			   	const uint8_t CHG_FET,
			   	const uint8_t DSG_FET,
			   	const uint8_t PCHG_FET,
			   	const uint8_t PDSG_FET,
			   	//电压保护
			   	const uint8_t CUV_alert,
			   	const uint8_t CUV_error,
			   	const uint8_t COV_alert,
			   	const uint8_t COV_error,
			   	const uint8_t COVL_alert,
			   	const uint8_t COVL_error,
			   	//电流保护
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
			   	//温度保护
			   	const uint8_t OTC_alert,
				const uint8_t OTC_error,
				const uint8_t OTD_alert,
				const uint8_t OTD_error,
				const uint8_t OTF_alert,
				const uint8_t OTF_error,
				const uint8_t UTC_alert,
				const uint8_t UTC_error,
				const uint8_t UTD_alert,
				const uint8_t UTD_error,
				const uint8_t OTINT_alert,
				const uint8_t OTINT_error,
				const uint8_t UTINT_alert,
				const uint8_t UTINT_error,
			   	//预充电超时保护
			   	const uint8_t PTOS_alert,
			   	const uint8_t PTOS_error,
			   	//二次过压保护
			   	const uint8_t SOV_alert,
			   	const uint8_t SOV_error,
			   	const uint8_t SUV_alert,
			   	const uint8_t SUV_error,
			   	const uint8_t LVL2_alert,
			   	const uint8_t LVL2_error,
			   	const uint8_t FUSE,
			   	//警报源使能掩码
			   	const uint8_t SF_ALERT_MASKA,
			   	const uint8_t SF_ALERT_MASKB,
			   	const uint8_t SF_ALERT_MASKC,
			   	const uint8_t PF_ALERT_MASKA,
			   	//output
			   	uint8_t *Alert_Latch
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
	uint8_t pfalertA;
	uint8_t pfstatusA;
	uint8_t fetstatus;
	uint8_t batterystatus;
	uint16_t alarmstatus;
	uint16_t alarmrawstatus;
	uint16_t alarmenable=0xFFFF;
	
	uint8_t SS,PF,PCHG_MODE;
	uint8_t SSBC,SSA,MSK_SFALERT,MSK_PFALERT,XCHG,XDSG;
	
	safetyalertA = CUV_alert<<2 | COV_alert<<3 | OCC_alert<<4 | OCD1_alert<<5 | OCD2_alert<<6 | SCD_alert<<7;
	writeDirectMemory(safetyalertA, SafetyAlertA);

	safetystatusA = CUV_error<<2 | COV_error<<3 | OCC_error<<4 | OCD1_error<<5 | OCD2_error<<6 | SCD_error<<7;
	writeDirectMemory(safetystatusA, SafetyStatusA);

	safetyalertB = UTC_alert | UTD_alert<<1 | UTINT_alert<<2 | OTC_alert<<3 | OTD_alert<<4 | OTINT_alert<<5 | OTF_alert<<6;
	writeDirectMemory(safetyalertB, SafetyAlertB);

	safetystatusB = UTC_error | UTD_error<<1 | UTINT_error<<2 | OTC_error<<3 | OTD_error<<4 | OTINT_error<<5 | OTF_error<<6;
	writeDirectMemory(safetystatusB, SafetyStatusB);

	safetyalertC = PTOS_alert<<3 | COVL_alert<<4 | OCDL_alert<<5 | SCDL_alert<<6 | OCD3_alert<<7;
	writeDirectMemory(safetyalertC, SafetyAlertC);

	safetystatusC = PTOS_error<<3 | COVL_error<<4 | OCDL_error<<5 | SCDL_error<<6 | OCD3_error<<7;
	writeDirectMemory(safetystatusC, SafetyStatusC);

	pfalertA = LVL2_alert << 2 | SOV_alert << 1 | SUV_alert;
	writeDirectMemory(pfalertA,PFAlertA);

	pfstatusA = LVL2_error << 2 | SOV_error << 1 | SUV_error;
	writeDirectMemory(pfstatusA,PFStatusA);
	
	fetstatus = CHG_FET | PCHG_FET<<1 | DSG_FET<<2 | PDSG_FET<<3 ;
	//	fetstatus = CHG_FET | PCHG_FET<<1 | DSG_FET<<2 | PDSG_FET<<3 | DCHG_PIN<<4 | DDSG_PIN<<5 | ALRT_PIN<<6;	
	writeDirectMemory(fetstatus, FETStatus);

	if(safetystatusA | safetystatusB | safetystatusC)
		SS = 1;
	else
		SS = 0;
	if(pfstatusA)
		PF = 1;
	else
		PF = 0;
	if(PCHG_FET)
		PCHG_MODE = 1;
	else
		PCHG_MODE = 0;
	batterystatus = SS << 11 | PF << 10 | FUSE << 9 | PCHG_MODE << 1;
	writeDirectMemory(batterystatus, BatteryStatus);

	if(safetyalertB | safetyalertC)
		SSBC = 1;
	else
		SSBC = 0;
	if(safetyalertA)
		SSA = 1;
	else
		SSA = 0;
	if((safetyalertA & SF_ALERT_MASKA) | (safetyalertB & SF_ALERT_MASKB) | (safetyalertC & SF_ALERT_MASKC))
		MSK_SFALERT = 1;
	else
		MSK_SFALERT = 0;
	if(pfalertA & PF_ALERT_MASKA)
		MSK_PFALERT = 1;
	else
		MSK_PFALERT = 0;
	if(!CHG_FET)
		XCHG = 1;
	else
		XCHG = 0;
	if(!DSG_FET)
		XDSG = 1;
	else
		XDSG = 0;
	
	alarmrawstatus = SSBC << 15 | SSA << 14 | PF << 13 | MSK_SFALERT << 12 | MSK_PFALERT << 11 | XCHG << 6 | XDSG << 5;
	alarmstatus = alarmrawstatus & alarmenable;
	writeDirectMemory(alarmrawstatus, AlarmRawStatus);
	writeDirectMemory(alarmstatus, AlarmStatus);
	
	if(alarmstatus)
		*Alert_Latch = 1;
	else
		*Alert_Latch = 0;
}

void BQ76952
(
				//input
                const uint16_t *CellVoltage,  	//16节电池差分电压                 
                const int16_t current,        	//敏感电阻差分电压
                const uint16_t charger,       	//充电器检测结果
				const uint16_t LD,				//负载检测结果
                const int32_t BAT_Pin,
                const int32_t PACK_Pin,
                const int32_t LD_Pin,
                const int16_t TS1,             	//TS1热敏电阻端温度(CELL)
                const int16_t TS2,             	//TS2引脚电压(WAKEON)
                const int16_t TS3,				//TS3热敏电阻温度(FET)
                const uint8_t RST_SHUT,
                //RST_SHUT Pin电压
                //DCHG Pin电压    
                //DDSG Pin电压   
                
				//output
                uint8_t *CHG_on,
                uint8_t *DSG_on,               
                uint8_t *PCHG_on,               
                uint8_t *PDSG_on,
                uint8_t *FUSE,
                uint8_t *Alert 
)
{
	uint8_t CUV_EN;
	uint16_t CUV_TH;
	uint16_t CUV_DLY;
	uint8_t CUV_REC_HYS;
	uint8_t COV_EN;
    uint16_t COV_TH; 
    uint16_t COV_DLY; 
    uint8_t COV_REC_HYS;
	uint8_t COVL_EN;
    uint8_t COVL_Limit;        
    uint8_t COVL_DEC_DLY; 
    uint8_t COVL_RecoveryTime;
	uint8_t RecoveryTime;

	uint8_t SCD_EN;
	uint8_t SCD_TH;            
	uint8_t SCD_DLY;           
	uint8_t SCD_REC_DLY;       

	uint8_t SCDL_EN;
	uint8_t SCDL_Limit;
	uint8_t SCDL_DEC_DLY;
	uint8_t SCDL_RecoveryTime;
	int16_t SCDL_REC_TH;
	uint16_t SCDL_CURR_RECOV;

	uint8_t OCD1_EN;
	uint8_t OCD1_TH;                           //单位2mv
	uint8_t OCD1_DLY;                            //实际延时 ms 级别
	uint8_t OCD2_EN;
	uint8_t OCD2_TH;                           //单位2mv
	uint8_t OCD2_DLY ;                            //实际延时 ms 级别
	uint8_t OCD3_EN;
	int16_t OCD3_TH;                          //基于CC1读数结果,单位userA(可配置)
	uint8_t OCD3_DLY;                            //实际延时 s 级别
	int16_t OCD_REC_TH;                        //mV单位  原始寄存器单位为mA注意连接时单位

	uint8_t OCDL_EN;
	uint8_t OCDL_Limit;                          
	uint8_t OCDL_DEC_DLY;                     //实际 s 级别延时
	uint8_t OCDL_RecoveryTime;                 //实际 s 级别延时 Settings:Protection:Protection Configuration[OCDL_CURR_RECOV] 有效时该恢复机制才会起作用
	int16_t OCDL_REC_TH;                       //mV单位  原始寄存器单位为mA注意连接时单位
	uint16_t OCDL_CURR_RECOV;

	uint8_t OCC_EN;
	uint8_t OCC_TH;
	uint8_t OCC_DLY;
	int16_t OCC_CREC_TH;
	int16_t OCC_VREC_Delta;
	
	uint8_t TS1_Config;
	uint8_t TS2_Config;
	uint8_t TS3_Config;
	uint8_t	TINT_EN;							
	uint8_t TINT_FETT;
	uint8_t OTC_EN;
	int8_t OTC_TH;
	uint8_t OTC_DLY;
	int8_t OTC_REC_TH;
	uint8_t OTD_EN;
	int8_t OTD_TH;
	uint8_t OTD_DLY;
	int8_t OTD_REC_TH;
	uint8_t OTF_EN;
	int8_t OTF_TH;
	uint8_t OTF_DLY;
	int8_t OTF_REC_TH;
	uint8_t UTC_EN;
	int8_t UTC_TH;
	uint8_t UTC_DLY;
	int8_t UTC_REC_TH;
	uint8_t UTD_EN;
	int8_t UTD_TH;
	uint8_t UTD_DLY;
	int8_t UTD_REC_TH;

	uint8_t OTINT_EN;
	int8_t OTINT_TH;
	uint8_t OTINT_DLY;
	int8_t OTINT_REC_TH;
	uint8_t UTINT_EN;
	int8_t UTINT_TH;
	uint8_t UTINT_DLY;
	int8_t UTINT_REC_TH;

	uint8_t PTO_EN;
	uint16_t PTO_DLY;
	int16_t PTO_Charge_TH;
	int16_t PTO_RESET;
	int16_t DSG_Current_TH;

	uint8_t PF_EN;
	uint8_t PF_FETS;
	uint8_t PF_FUSE;
	uint8_t PF_DPSLP;
	uint8_t PF_REGS;
	
	uint8_t SOV_EN;
	int16_t SOV_TH;
	uint8_t SOV_DLY;
	uint8_t SUV_EN;
	int16_t SUV_TH;
	uint8_t SUV_DLY;
	uint8_t LVL2_EN;
	uint8_t LVL2_DLY;
	
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

   	uint8_t SF_ALERT_MASKA;
    uint8_t SF_ALERT_MASKB;
   	uint8_t SF_ALERT_MASKC;
   	uint8_t PF_ALERT_MASKA;
	
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

	uint8_t OTC_alert;
	uint8_t OTC_error;
	uint8_t OTD_alert;
	uint8_t OTD_error;
	uint8_t OTF_alert;
	uint8_t OTF_error;
	uint8_t UTC_alert;
	uint8_t UTC_error;
	uint8_t UTD_alert;
	uint8_t UTD_error;
	
	uint8_t OTINT_alert;
	uint8_t OTINT_error;
	uint8_t UTINT_alert;
	uint8_t UTINT_error;
	
	uint8_t PTOS_alert;
	uint8_t PTOS_error;

	uint8_t SOV_alert;
	uint8_t SOV_error;
	uint8_t SUV_alert;
	uint8_t SUV_error;
	uint8_t LVL2_alert;
	uint8_t LVL2_error;
	
	uint8_t fuse_ctrl;
	uint8_t fuse_in;
	uint8_t alert_ctrl;
	
	uint8_t CHG_ON;
	uint8_t DSG_ON;
	uint8_t PCHG_ON;
	uint8_t PDSG_ON;

	update_config(
				//output
				//FET控制
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
				//电压保护
				&COV_EN,
				&COV_EN,
				&COVL_EN,
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
				//电流保护
				&SCD_EN,
				&SCD_TH,
				&SCD_DLY,
				&SCD_REC_DLY,
				&SCDL_EN,
				&SCDL_Limit,
				&SCDL_DEC_DLY,
				&SCDL_RecoveryTime,
				&SCDL_REC_TH,
				&SCDL_CURR_RECOV,
				&OCD1_EN,
				&OCD1_TH,                           
				&OCD1_DLY, 
				&OCD2_EN,
				&OCD2_TH,                           
				&OCD2_DLY, 
				&OCD3_EN,
				&OCD3_TH,                         
				&OCD3_DLY,                            
				&OCD_REC_TH, 
				&OCDL_EN,
				&OCDL_Limit,                         
				&OCDL_DEC_DLY,                     
				&OCDL_RecoveryTime,                 
				&OCDL_REC_TH,
				&OCDL_CURR_RECOV,
				&OCC_EN,
				&OCC_TH,
				&OCC_DLY,
				&OCC_CREC_TH,
				&OCC_VREC_Delta,
				//温度保护
				&TS1_Config,
				&TS2_Config,
				&TS3_Config,
				&TINT_EN,							
				&TINT_FETT,
				&OTC_EN,
				&OTC_TH,
				&OTC_DLY,
				&OTC_REC_TH,
				&OTD_EN,
				&OTD_TH,
				&OTD_DLY,
				&OTD_REC_TH,
				&OTF_EN,
				&OTF_TH,
				&OTF_DLY,
				&OTF_REC_TH,
				&UTC_EN,
				&UTC_TH,
				&UTC_DLY,
				&UTC_REC_TH,
				&UTD_EN,
				&UTD_TH,
				&UTD_DLY,
				&UTD_REC_TH,
				&OTINT_EN,
				&OTINT_TH,
				&OTINT_DLY,
				&OTINT_REC_TH,
				&UTINT_EN,
				&UTINT_TH,
				&UTINT_DLY,
				&UTINT_REC_TH,
				//预充电超时保护
				&PTO_EN,
				&PTO_DLY,
				&PTO_Charge_TH,
				&PTO_RESET,
				&DSG_Current_TH,
				//二次保护
				&PF_EN,
				&PF_FETS,
				&PF_FUSE,
				&PF_DPSLP,
				&PF_REGS,
				//二次过压保护
				&SOV_EN,
				&SOV_TH,
				&SOV_DLY,
				//二次欠压保护
				&SUV_EN,
				&SUV_TH,
				&SUV_DLY,
				&LVL2_EN,
				&LVL2_DLY,
				//警报源使能掩码
			    &SF_ALERT_MASKA,
			   	&SF_ALERT_MASKB,
			   	&SF_ALERT_MASKC,
			   	&PF_ALERT_MASKA
				);


	int16_t CC1_current = (current*1000)/RSPN;//CC1电流mA
	int16_t Internal_Temp = 40;	//内部温度°C
	uint8_t RST_SHUT_h_1s;
	uint8_t work_mode;
	
	if(!RST_SHUT)
		RS_counter_h = 0;
	else if(RST_SHUT == 1)
		RS_counter_h ++;
	if(RS_counter_h == RST_SHUT_1S)
		RST_SHUT_h_1s = 1;
	else
		RST_SHUT_h_1s = 0;

	BQ76852_work_mode(
				//input signal
				CellVoltage,		//VC1-16 mv	
				BAT_Pin,
				PACK_Pin,				
				TS2,				
				LD_Pin,						
				current,			//RSP-RSN mv					
				CC1_current,				
				Internal_Temp,
				RST_SHUT,
				RST_SHUT_h_1s,		//RST_SHUT拉高1s
				//output
				&work_mode
				);
	printf("work_mode = %d\n",work_mode);

	if(work_mode == 1 || work_mode == 2)
	{
    CUV_protect(
				//input
				CellVoltage,		//VC1-VC16
				CUV_EN,
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
				COV_EN,
    			COV_TH,				//阈值
    			COV_DLY,			//延时
    			COV_REC_HYS,			//恢复滞后电压
    			RecoveryTime,		//恢复延时
    			//output
    			&COV_alert,			//alert信号
    			&COV_error);		//alert信号

    COVL_protect(
				//input
				COVL_EN,
				COVL_Limit,			//触发COVL错误的计数上限
    			COVL_DEC_DLY,		//COV错误计数递减的时间间隔
    			COVL_RecoveryTime,	//自动恢复延时
    			//output
    			&COVL_alert,		//alert信号
    			&COVL_error);		//error信号

	SCD_protect(
				//input
				current,
				SCD_EN,
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
				SCDL_EN,
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
				OCD1_EN,
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
				OCD2_EN,
				OCD2_TH,
				OCD2_DLY,
				OCD_REC_TH,
				RecoveryTime,
				//output
				&OCD2_alert,
				&OCD2_error
				);
	OCD3_protect(
				//input
				CC1_current,
				OCD3_EN,
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
				OCDL_EN,
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
				OCC_EN,
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
	InternT_protect(
				//input
				Internal_Temp,
				TINT_EN,
				TINT_FETT,
				OTINT_EN,
				OTINT_TH,
				OTINT_DLY,
				OTINT_REC_TH,
				UTINT_EN,
				UTINT_TH,
				UTINT_DLY,
				UTINT_REC_TH,
				RecoveryTime,
				//output
				&OTINT_alert,
				&OTINT_error,
				&UTINT_alert,
				&UTINT_error
				);
	TermistorT_protect(
				//input
				TS1,				//TS1温度值（实际应输入电阻值，再转换为温度值）
				TS2,				//TS2温度值（实际应输入电阻值，再转换为温度值）
				TS3,				//TS3温度值（实际应输入电阻值，再转换为温度值）
				TS1_Config,			//热敏电阻TS1引脚复用配置	
				TS2_Config,			//热敏电阻TS2引脚复用配置
				TS3_Config,			//热敏电阻TS3引脚复用配置
				Internal_Temp,		//内部温度值
				TINT_EN,			//内部温度作为电池温度使能位
				TINT_FETT,			//内部温度作为FET温度使能位
				OTC_EN,
				OTC_TH,
				OTC_DLY,
				OTC_REC_TH,
				OTD_EN,
				OTD_TH,
				OTD_DLY,
				OTD_REC_TH,
				OTF_EN,
				OTF_TH,
				OTF_DLY,
				OTF_REC_TH,
				UTC_EN,
				UTC_TH,
				UTC_DLY,
				UTC_REC_TH,
				UTD_EN,
				UTD_TH,
				UTD_DLY,
				UTD_REC_TH,
				RecoveryTime,
				//output
				&OTC_alert,
				&OTC_error,
				&OTD_alert,
				&OTD_error,
				&OTF_alert,
				&OTF_error,
				&UTC_alert,
				&UTC_error,
				&UTD_alert,
				&UTD_error);
	PTO_protect(
				//input
				CC1_current,		//PTO触发电流值、PTO恢复电荷值
				current,			//PTO恢复时应处于放电状态
				*PCHG_on,
				PTO_EN,
				PTO_DLY,
				PTO_Charge_TH,		//PTO触发电流阈值
				DSG_Current_TH,		//放电状态电流阈值
				PTO_RESET,			//PTO恢复电荷阈值
				//output
				&PTOS_alert,
				&PTOS_error
			   );

	SOV_protect(
				//input
                CellVoltage, 
                PF_EN,
                SOV_EN,
                SOV_TH,          
                SOV_DLY,      
                //output
                &SOV_alert,             
                &SOV_error   
				);
	SUV_protect(
				//input
                CellVoltage, 
                PF_EN,
                SUV_EN,
                SUV_TH,          
                SUV_DLY,      
                //output
                &SUV_alert,             
                &SUV_error   
				);
	fuse_ctrl = PF_FUSE & (SOV_error | SUV_error);
	fuse_in = (*FUSE) & (!fuse_ctrl);
	LVL2_protect(
				//input
				fuse_in,
				PF_EN,
				LVL2_EN,
				LVL2_DLY,
				//output
				&LVL2_alert,
				&LVL2_error
				);

	uint8_t LD_Stack_Delta = LD_Pin - BAT_Pin;	//用于PDSG基于电压关闭机制（阈值PDSG_StopDelta）
	FET_auto_control(
				//input
				CellVoltage,
				LD_Stack_Delta,
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
				PCHG_StartVoltage,
				PCHG_StopVoltage,
				PDSG_Timeout,
				PDSG_StopDelta,
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
				OTC_error,
				OTD_error,
				OTF_error,
				UTC_error,
				UTD_error,
				OTINT_error,
				UTINT_error,
				//output
				&CHG_ON,
				&DSG_ON,
				&PCHG_ON,
				&PDSG_ON
				);
	}
	else if(work_mode == 3 || work_mode == 4)
	{
		//禁用FETs
		CHG_ON = 0;
		DSG_ON = 0;
		PCHG_ON = 0;
		PDSG_ON = 0;
		//关机模式下，寄存器会清零
		if(work_mode == 4)
		{
			for(int i=0;i<SIZE_OF_DIRECT;i++)
			{
				mm.DirectMemory[i].data = 0;
			}
		}
	}
    update_register(
				//input
				//电池电压
				&CellVoltage,
				//FET控制
				CHG_ON,
				DSG_ON,
				PCHG_ON,
				PDSG_ON,
				//电压保护
				CUV_alert,
				CUV_error,
				COV_alert,
				COV_error,
				COVL_alert,
				COVL_error,
				//电流保护
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
				//温度保护
				OTC_alert,
				OTC_error,
				OTD_alert,
				OTD_error,
				OTF_alert,
				OTF_error,
				UTC_alert,
				UTC_error,
				UTD_alert,
				UTD_error,
				OTINT_alert,
				OTINT_error,
				UTINT_alert,
				UTINT_error,
				//预充电超时保护
				PTOS_alert,
				PTOS_error,
				//二次欠压保护
				SOV_alert,
				SOV_error,
				//二次欠压保护
				SUV_alert,
				SUV_error,
				LVL2_alert,
				LVL2_error,
				fuse_ctrl,
				//警报源使能掩码
			    SF_ALERT_MASKA,
			   	SF_ALERT_MASKB,
			   	SF_ALERT_MASKC,
			   	PF_ALERT_MASKA,
			   	//output
			   	&alert_ctrl
				);
	
	*CHG_on = CHG_ON;
	*DSG_on = DSG_ON;
	*PCHG_on = PCHG_ON;
	*PDSG_on = PDSG_ON;
	*FUSE = fuse_ctrl | fuse_in;
	*Alert = alert_ctrl;
}
