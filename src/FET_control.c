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
//#define CHGFETProtectionsB 0x9266
#define CHG_UTC BIT(0)
#define CHG_UTINT BIT(2)
#define CHG_OTC BIT(4)
#define CHG_OTINT BIT(6)
#define CHG_OTF BIT(7)
//#define CHGFETProtectionsC 0x9267
#define CHG_HWDF BIT(1)
#define CHG_PTO BIT(2)
#define CHG_COVL BIT(4)
#define CHG_SCDL BIT(6)
//#define DSGFETProtectionsA 0x9269
#define DSG_CUV BIT(2)
#define DSG_OCD1 BIT(5)
#define DSG_OCD2 BIT(6)
#define DSG_SCD BIT(7)
//#define DSGFETProtectionsB 0x926A
#define DSG_UTD BIT(1)
#define DSG_UTINT BIT(2)
#define DSG_OTD BIT(5)
#define DSG_OTINT BIT(6)
#define DSG_OTF BIT(7)
//#define DSGFETProtectionsC 0x926B
#define DSG_HWDF BIT(1)
#define DSG_OCDL BIT(5)
#define DSG_SCDL BIT(6)
#define DSG_OCD3 BIT(7)

uint8_t PDSG_timer = 0;
uint8_t CHG_ctrl=0;
uint8_t DSG_ctrl=0;
uint8_t PCHG_ctrl=0;
uint8_t PDSG_ctrl=0;

void FET_auto_control
(
				//input
				const uint16_t *CellVoltage,
				const uint8_t LD_TOS_delta,
				const uint8_t FET_ctrl_en,		//0x9308 FETOptions[FET_CTRL_EN]
				const uint8_t FET_init_off,		//0x9308 FETOptions[FET_INIT_OFF]
				const uint8_t FET_en,			//0x9343 MfgStatusInit[FET_EN]
				const uint8_t PDSG_en,			//0x9308 FETOptions[PDSG_EN]
				const uint8_t CHG_protectionA,
				const uint8_t CHG_protectionB,
				const uint8_t CHG_protectionC,
				const uint8_t DSG_protectionA,
				const uint8_t DSG_protectionB,
				const uint8_t DSG_protectionC,
				const int16_t PCHG_startvoltage,
				const int16_t PCHG_stopvoltage,
				const uint8_t PDSG_timeout,
				const uint8_t PDSG_stop_delta,
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
				const uint8_t PTOS_error,
				const uint8_t OTC_error,
				const uint8_t OTD_error,
				const uint8_t OTF_error,
				const uint8_t UTC_error,
				const uint8_t UTD_error,
				const uint8_t OTINT_error,
				const uint8_t UTINT_error,
				//output
				uint8_t *CHG_ON,
				uint8_t *DSG_ON,
				uint8_t *PCHG_ON,
				uint8_t *PDSG_ON
)
{

	
	if(!FET_ctrl_en)//FETs未使能
		return;
	if(FET_init_off && !(*CHG_ON | *DSG_ON))//FETs初始状态默认关闭，且主机未发送开启命令
	{
		return;
	}

	if(FET_en)//设备自动控制FETs已使能
	{
		//CHG_ctrl
		//Settings:Protection:CHG FET Protections A
		if((COV_error && (CHG_COV & CHG_protectionA)) | (OCC_error && (CHG_OCC & CHG_protectionA)) | (SCD_error && (CHG_SCD & CHG_protectionA)))
		{
			CHG_ctrl = 0;
		}
		//Settings:Protection:CHG FET Protections B
		else if((UTC_error && (CHG_UTC & CHG_protectionB)) | (OTC_error && (CHG_OTC & CHG_protectionB)) | (OTF_error && (CHG_OTF & CHG_protectionB)) | (UTINT_error && (CHG_UTINT & CHG_protectionB)) | (OTINT_error && (CHG_OTINT & CHG_protectionB)))
		{
			CHG_ctrl = 0;
		}
		//Settings:Protection:CHG FET Protections C
		else if((PTOS_error && (CHG_PTO & CHG_protectionC)) | (COVL_error && (CHG_COVL & CHG_protectionC)) | (SCDL_error && (CHG_SCDL & CHG_protectionC)))
		{
			CHG_ctrl = 0;
		}
		else
			CHG_ctrl = 1;
		
		//DSG_ctrl
		uint8_t DSG_ctrl_temp = DSG_ctrl;
		//Settings:Protection:DSG FET Protections A
		if((CUV_error && (DSG_CUV & DSG_protectionA)) | (OCD1_error && (DSG_OCD1 & DSG_protectionA)) | (OCD2_error && (DSG_OCD2 & DSG_protectionA)) | (SCD_error && (DSG_SCD & DSG_protectionA)))
			DSG_ctrl = 0;
		//Settings:Protection:DSG FET Protections B
		else if((OTD_error && (DSG_OTD & DSG_protectionB)) | (OTF_error && (DSG_OTF & DSG_protectionB)) | (UTD_error && (DSG_UTD & DSG_protectionB)) | (UTINT_error && (DSG_UTINT & DSG_protectionB)) | (OTINT_error && (DSG_OTINT & DSG_protectionB)))
			DSG_ctrl = 0;
		//Settings:Protection:DSG FET Protections C
		else if((OCDL_error && (DSG_OCDL & DSG_protectionC)) | (SCDL_error && (DSG_SCDL & DSG_protectionC)) | (OCD3_error && (DSG_OCD3 & DSG_protectionC)))
			DSG_ctrl = 0;
		else
			DSG_ctrl = 1;
		
		//PCHG_ctrl
		uint16_t min_Vcell = *CellVoltage;
	    for(int i=1;i<16;i++)
	    {
	        if(*CellVoltage < min_Vcell)
	            min_Vcell = *CellVoltage;
	        CellVoltage ++;
	    }
		if(PTOS_error && (CHG_PTO & CHG_protectionC))
			PCHG_ctrl = 0;
		else if(min_Vcell < PCHG_startvoltage)
			PCHG_ctrl = 1;
		else if(PCHG_ctrl == 1 && min_Vcell >= PCHG_stopvoltage)
			PCHG_ctrl = 0;

		//PDSG_ctrl
		if(PDSG_timer == PDSG_timeout)//基于超时关闭PDSG
			PDSG_ctrl = 0;
		else if(PDSG_en && PDSG_ctrl && (LD_TOS_delta >= PDSG_stop_delta))//基于电压关闭PDSG
			PDSG_ctrl = 0;
		else if(PDSG_en && DSG_ctrl == 1 && DSG_ctrl_temp == 0)//识别到DSG_ctrl上升沿时，置1
			PDSG_ctrl = 1;
		else if(!PDSG_en || DSG_ctrl == 0)
			PDSG_ctrl = 0;
		
		if(PDSG_timer == PDSG_timeout || PDSG_ctrl == 0)
			PDSG_timer = 0;
		else if(PDSG_ctrl == 1)
			PDSG_timer++;

	}
	else//完全手动控制FETs(FET_TEST模式)
		return;

	*CHG_ON = CHG_ctrl;
	*PCHG_ON = PCHG_ctrl;
	*PDSG_ON = PDSG_ctrl;
	if(PDSG_ctrl == 1)//预放电阶段
		*DSG_ON = 0;
	else if(PDSG_ctrl == 0)
		*DSG_ON = DSG_ctrl;
}

uint8_t PTO_counter=0;
uint8_t pto_error=0;

void PTO_protect(
			//input
			const int16_t CC1_current,
			const int16_t current,
			const uint8_t PCHG_on,
			const uint8_t PTO_en,
			const uint16_t PTO_delay,
			const int16_t PTO_charge_th,
			const int16_t dsg_current_th,
			const int16_t PTO_reset,
			//output
			uint8_t *PTOS_alert,
			uint8_t *PTOS_error
		   )
{
	uint8_t alert;
	if(!PTO_en)
	{
		alert = 0;
		pto_error = 0;
		PTO_counter = 0;
	}
	else
	{
		if(PCHG_on && (CC1_current < PTO_charge_th))
			alert = 1;
		else
			alert = 0;
		
		if(current < dsg_current_th && CC1_current >= PTO_reset)//这里用的CC1电流,实际应该换算成电荷量再比较
			PTO_counter = 0;
		else if(alert == 1)
			PTO_counter = PTO_counter;
		else if(PTO_counter == PTO_delay)//计满清零，触发PTO故障
		{
			pto_error = 1;
			PTO_counter = 0;
		}
		else if(PCHG_on && (CC1_current > PTO_charge_th))//预充电模式开启且CC1电流超过阈值
			PTO_counter ++;

		//pto_error只有主机命令可以清除
	}
	
	*PTOS_error = pto_error;
	*PTOS_alert = alert;
}

