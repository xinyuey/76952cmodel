/*CHG PCHG DSG PDSG*/

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

//0x9308 FETOptions
#define FET_INIT_OFF BIT(5)
#define PDSG_EN BIT(4)
#define FET_CTRL_EN BIT(3)
#define HOST_FET_EN BIT(2)
#define SLEEPCHG BIT(1)
#define SFET BIT(0)

//0x9309 ChgPumpControl  
#define SFMODE_SLEEP BIT(2)
#define LVEN BIT(1)
#define CPEN BIT(0)

//0x9343 MfgStatusInit
#define FET_EN BIT(4)

//0x7F FETStatus
#define PDSG_FET BIT(3)
#define DSG_FET BIT(2)
#define PCHG_FET BIT(1)
#define CHG_FET BIT(0)

//0x64 AlarmRawStatus 
#define XCHG BIT(6)
#define XDSG BIT(5)

uint8_t CHG_ON;
uint8_t DSG_ON;

typedef enum{
        RESET=1,
        ENABLE,
        DISABLE,
		NORMA,
		TEST
        }StateMachine_FET;

StateMachine_FET FET_State = RESET;

// uint8_t FET_ON[2];
uint8_t* FET_control()
{
	

	switch(FET_State)
	{
		case RESET:
			if(FET_CTRL_EN & readDirectMemory(FETOptions))
				FET_State =  RESET;
			else
				FET_State = ENABLE;//上电默认开启FETs
			break;
		case ENABLE:
			if(FET_INIT_OFF & readDirectMemory(FETOptions))
				FET_State = DISABLE;//上电默认关闭FETs
			else if(FET_EN & readDirectMemory(MfgStatusInit))
				FET_State = NORMA;//工作模式
			else if(!(FET_EN & readDirectMemory(MfgStatusInit)))
				FET_State = TEST;//测试模式
			break;
		case DISABLE:
			// if(;)//主机发送唤醒命令，开启FETs
			// 	FET_State = ENABLE;
			break;
		case NORMA:
			if(XCHG & readDirectMemory(AlarmRawStatus))//CHG相关的保护已触发,关闭CHG
			{
				CHG_ON = 0;
				clearFlags(CHG_FET,FETStatus);
				// clearFlags(PCHG_FET,FETStatus);
			}
			else
			{
				CHG_ON = 1;
				setFlags(CHG_FET,FETStatus);
				// setFlags(PCHG_FET,FETStatus);
			}
				
			if(XDSG & readDirectMemory(AlarmRawStatus))//DSG相关的保护已触发，关闭DSG
			{
				DSG_ON = 0;
				clearFlags(DSG_FET,FETStatus);
				// if(PDSG_EN & readDirectMemory(FETOptions))
				// 	clearFlags(PDSG_FET,FETStatus);
			}
			else
			{
				DSG_ON = 1;
				setFlags(DSG_FET,FETStatus);
				// if(PDSG_EN & readDirectMemory(FETOptions))
				// 	setFlags(PDSG_FET,FETStatus);
			}
			// FET_ON[2] = {CHG_ON,DSG_ON};
			break;
		case TEST:
			;//根据主机发送的命令，开启或关闭CHG/PCHG/DSG/PDSG
			break;
	}

}