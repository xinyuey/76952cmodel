/*CHG PCHG DSG PDSG*/

//相关寄存器
//#define FETOptions 0x9308                   // Settings:FET:FET Options
//#define ChgPumpControl 0x9309               // Settings:FET:Chg Pump Control
//#define PrechargeStartVoltage 0x930A        // Settings:FET:Precharge Start Voltage
//#define PrechargeStopVoltage 0x930C         // Settings:FET:Precharge Stop Voltage
//#define PredischargeTimeout 0x930E          // Settings:FET:Predischarge Timeout
//#define PredischargeStopDelta 0x930F        // Settings:FET:Predischarge Stop Delta

//#define CHGFETProtectionsA 0x9265           // Settings:Protection:CHG FET Protections A
//#define CHGFETProtectionsB 0x9266           // Settings:Protection:CHG FET Protections B
//#define CHGFETProtectionsC 0x9267           // Settings:Protection:CHG FET Protections C
//#define DSGFETProtectionsA 0x9269           // Settings:Protection:DSG FET Protections A
//#define DSGFETProtectionsB 0x926A           // Settings:Protection:DSG FET Protections B
//#define DSGFETProtectionsC 0x926B           // Settings:Protection:DSG FET Protections C
//#define BodyDiodeThreshold 0x9273           // Settings:Protection:Body Diode Threshold

#include "common.h"

//#define CHGFETProtectionsA 0x9265
#define CHG_COV BIT(3)
#define CHG_OCC BIT(4)
#define CHG_SCD BIT(7)
//#define CHGFETProtectionsC 0x9267
#define CHG_COVL BIT(4)
#define CHG_SCDL BIT(6)
//#define DSGFETProtectionsA 0x9269
#define DSG_CUV BIT(2)
#define DSG_OCD1 BIT(5)
#define DSG_OCD2 BIT(6)
#define DSG_SCD BIT(7)
//#define DSGFETProtectionsC 0x926B
#define DSG_OCDL BIT(5)
#define DSG_SCDL BIT(6)
#define DSG_OCD3 BIT(7)

void FET_auto_control
(
				//input
				const uint8_t FET_ctrl_en,		//0x9308 FETOptions[FET_CTRL_EN]
				const uint8_t FET_init_off,		//0x9308 FETOptions[FET_INIT_OFF]
				const uint8_t FET_en,			//0x9343 MfgStatusInit[FET_EN]
				const uint8_t CHG_protectionA,
				const uint8_t CHG_protectionB,
				const uint8_t CHG_protectionC,
				const uint8_t DSG_protectionA,
				const uint8_t DSG_protectionB,
				const uint8_t DSG_protectionC,
				const uint8_t COV_error,
				const uint8_t COVL_error,
				const uint8_t CUV_error,
				const uint8_t SCD_error,
				const uint8_t SCDL_error,
				const uint8_t OCD1_error,
				const uint8_t OCD2_error,
				const uint8_t OCD3_error,
				const uint8_t OCDL_error,
				const uint8_t OCC_error,
				//output
				uint8_t *CHG_ON,
				uint8_t *DSG_ON
)
{
	uint8_t CHG_ctrl,DSG_ctrl;
	
	if(!FET_ctrl_en)//FETs未使能
		return;
	if(FET_init_off && !(*CHG_ON | *DSG_ON))//FETs初始状态默认关闭，且主机未发送开启命令
	{
		return;
	}

	if(FET_en)//设备自动控制FETs已使能
	{
		//Settings:Protection:CHG FET Protections A
		if((COV_error && (CHG_COV & CHG_protectionA)) | (OCC_error && (CHG_OCC & CHG_protectionA)) | (SCD_error && (CHG_SCD & CHG_protectionA)) | (COVL_error && (CHG_COVL & CHG_protectionC)) | (SCDL_error && (CHG_SCDL & CHG_protectionC)))
			CHG_ctrl = 0;
		else
			CHG_ctrl = 1;

		//Settings:Protection:CHG FET Protections B
		//Settings:Protection:CHG FET Protections C

		//Settings:Protection:DSG FET Protections A
		if((CUV_error && (DSG_CUV & DSG_protectionA)) | (OCD1_error && (DSG_OCD1 & DSG_protectionA)) | (OCD2_error && (DSG_OCD2 & DSG_protectionA)) | (SCD_error && (DSG_SCD & DSG_protectionA)) | (OCDL_error && (DSG_OCDL & DSG_protectionC)) | (SCDL_error && (DSG_SCDL & DSG_protectionC)) | (OCD3_error && (DSG_OCD3 & DSG_protectionC)))
			DSG_ctrl = 0;
		else
			DSG_ctrl = 1;

	}
	else//完全手动控制FETs(FET_TEST模式)
		return;
	
	*CHG_ON = CHG_ctrl;
	*DSG_ON = DSG_ctrl;
}