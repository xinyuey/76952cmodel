#include "common.h"
#include <stdlib.h>
//相关寄存器
//#define ShutdownCellVoltage 0x923F          // Power:Shutdown:Shutdown Cell Voltage
//#define ShutdownStackVoltage 0x9241         // Power:Shutdown:Shutdown Stack Voltage
//#define LowVShutdownDelay 0x9243            // Power:Shutdown:Low V Shutdown Delay
//#define ShutdownTemperature 0x9244          // Power:Shutdown:Shutdown Temperature
//#define ShutdownTemperatureDelay 0x9245     // Power:Shutdown:Shutdown Temperature Delay
//#define FETOffDelay 0x9252                  // Power:Shutdown:FET Off Delay
//#define ShutdownCommandDelay 0x9253         // Power:Shutdown:Shutdown Command Delay
//#define AutoShutdownTime 0x9254             // Power:Shutdown:Auto Shutdown Time
//#define RAMFailShutdownTime 0x9255          // Power:Shutdown:RAM Fail Shutdown Time
//#define SleepCurrent 0x9248                 // Power:Sleep:Sleep Current
//#define VoltageTime 0x924A                  // Power:Sleep:Voltage Time
//#define WakeComparatorCurrent 0x924B        // Power:Sleep:Wake Comparator Current
//#define SleepHysteresisTime 0x924D          // Power:Sleep:Sleep Hysteresis Time
//#define SleepChargerVoltageThreshold 0x924E // Power:Sleep:Sleep Charger Voltage Threshold
//#define SleepChargerPACKTOSDelta 0x9250     // Power:Sleep:Sleep Charger PACK-TOS Delta

//#define BatteryStatus 0x12			      // BatteryStatus[SLEEP_EN],[SLEEP]

#define WAKEONTS2_TH 1000
#define WAKEONLD_TH 1500

const uint8_t SLEEP_en = 1; 					//睡眠模式使能
const int16_t SLEEP_current_th = 20; 			//进入或退出睡眠模式的CC1电流阈值，20mA
const int16_t SLEEP_wake_current_th = 500;		//退出睡眠模式的电流阈值,500mA
const uint8_t SLEEP_hysteresis_time = 10;		//退出再进入睡眠模式的滞后时间,10s
const int16_t SLEEP_charger_th = 20000;			//退出睡眠模式的stack端电压阈值，20V
const int16_t SLEEP_pack_stack_delta = 2000;	//退出睡眠模式的pack-stack电压阈值，2V
const int16_t SHUTDOWN_cell_th = 0;				//进入关机模式的电池电压阈值,0V
const int16_t SHUTDOWN_stack_th = 6000;			//进入关机模式的电池组电压阈值,6V
const uint8_t SHUTDOWN_lv_delay = 1;			//进入关机模式的低电压延时，1s
const uint8_t SHUTDOWN_temp_th = 85;			//进入关机模式的内部温度阈值,85°C
const uint8_t SHUTDOWN_temp_delay = 5;			//进入关机模式的内部温度延时,5s
const uint8_t DEEPSLEEP_PF_en = 0;				//使能通过永久性故障进入深度睡眠,0


typedef enum{
        normal=1,
        sleep,
        deepsleep,
        shutdown,
        soft_shutdown
        }WORKMODE;
WORKMODE mode_state = NORMAL;
int N2SD_flag = 0;
int S2N_flag = 0;
int S2SD_flag = 0;
int sleep_hys_counter = 0;
int shutdown_lv_counter = 0;
int shutdown_temp_counter = 0;
void BQ76852_work_mode
(
				//input signal
				const uint16_t *CellVoltage,				//电池电压
//				const int32_t stack_voltage,				//电池组电压
//				const int16_t pack_stack_Delta,				//PACK-STACK电压
				const int16_t PACK_voltage,
				const int16_t TS2_voltage,					//TS2电压
				const int16_t LD_voltage,							//LD电压
				const int16_t current,						//电流
				const int16_t CC1_current,					//CC1电流
				const int16_t internal_temp,				//内部温度
				const uint8_t RST_SHUT_L_1s,				//RST_SHUT拉低1s
				const uint8_t RST_SHUT_H_1s,				//RST_SHUT拉高1s
				//input config
//				const uint8_t SLEEP_en,						//睡眠模式使能
//				const int16_t SLEEP_current_th,				//进入或退出睡眠模式的CC1电流阈值，20mA
//				const int16_t SLEEP_wake_current_th,		//退出睡眠模式的电流阈值,500mA
//				const uint8_t SLEEP_hysteresis_time,		//退出再进入睡眠模式的滞后时间,10s
//				const int16_t SLEEP_charger_th,				//退出睡眠模式的stack端电压阈值，20V
//				const int16_t SLEEP_pack_stack_delta,		//退出睡眠模式的pack-stack电压阈值，2V
//				const int16_t SHUTDOWN_cell_th,				//进入关机模式的电池电压阈值,0V
//				const int16_t SHUTDOWN_stack_th,			//进入关机模式的电池组电压阈值,6V
//				const uint8_t SHUTDOWN_lv_delay,			//进入关机模式的低电压延时，1s
//				const uint8_t SHUTDOWN_temp_th,				//进入关机模式的内部温度阈值,85°C
//				const uint8_t SHUTDOWN_temp_delay,			//进入关机模式的内部温度延时,5s
//				const uint8_t DEEPSLEEP_PF_en,				//使能通过永久性故障进入深度睡眠,0
				//output
				uint8_t *work_mode
)
{
	uint16_t min_Vcell = *CellVoltage;
	int16_t stack_voltage = 0;
	int16_t pack_stack_Delta = 0;
    for(int i=1;i<16;i++)
    {
    	STACK_voltage = STACK_voltage + *CellVoltage;
        if(*CellVoltage < min_Vcell)
            min_Vcell = *CellVoltage;
        CellVoltage ++;
    }
	pack_stack_Delta = PACK_voltage - stack_voltage;
	switch (mode_state)
		{
		case normal:
			//进入关机模式
			if(shutdown_lv_counter == SHUTDOWN_lv_delay)//基于电池电压或电池组电压进入关机模式
			{
				shutdown_lv_counter = 0;
				N2SD_flag = 1;
			}
			else if((stack_voltage < SHUTDOWN_stack_th) || (min_Vcell < SHUTDOWN_cell_th))
				shutdown_lv_counter ++;
			else
				shutdown_lv_counter = 0;
			
			if(shutdown_temp_counter == SHUTDOWN_temp_delay)//基于内部温度进入关机模式
			{
				shutdown_temp_counter = 0;
				N2SD_flag = 1;
			}	
			else if(internal_temp > SHUTDOWN_temp_th)
				shutdown_temp_counter ++;
			else
				shutdown_temp_counter = 0;
			
			if(RST_SHUT_H_1s)//基于RST_SHUT引脚拉高1s进入关机模式(也可以通过主机子命令进入)
				N2SD_flag = 1;
			
			if(N2SD_flag && (TS2_voltage > WAKEONTS2_TH && LD_voltage < WAKEONLD_TH))//进入关机
			{
				mode_state = shutdown;
				N2SD_flag = 0;
				break;
			}
			else if(N2SD_flag && (TS2_voltage <=  WAKEONTS2_TH || LD_voltage >= WAKEONLD_TH))//进入软关机
			{
				mode_state = soft_shutdown;
				N2SD_flag = 0;
				break;
			}
			
			//进入睡眠模式
			if(S2N_flag && sleep_hys_counter == SLEEP_hysteresis_time)//滞后：退出睡眠模式后的一段时间内不能进入睡眠模式
			{
				sleep_hys_counter = 0;
				S2N_flag = 0;
			}
			else if(S2N_flag == 1)
			{
				sleep_hys_counter ++;
			}
			if(SLEEP_en && (abs(CC1_current) < SLEEP_current_th) && sleep_hys_counter == 0)
			{
				mode_state = sleep;
				break;
			}

			//进入DEEPSLEEP模式
			if(DEEPSLEEP_PF_en && readDirectMemory(PFStatusA))//基于永久性故障进入深度睡眠（也可以通过主机子命令）
			{
				mode_state = deepsleep;
				break;
			}
			break;
		case sleep:
			//进入关机模式
			if((stack_voltage < SHUTDOWN_stack_th) || (min_Vcell < SHUTDOWN_cell_th))//基于电池电压或电池组电压进入关机模式
				S2SD_flag = 1;
			
			if(shutdown_temp_counter == SHUTDOWN_temp_delay)//基于内部温度进入关机模式
			{
				shutdown_temp_counter = 0;
				S2SD_flag = 1;
			}	
			else if(internal_temp > SHUTDOWN_temp_th)
				shutdown_temp_counter ++;
			else
				shutdown_temp_counter = 0;
			
			if(RST_SHUT_H_1s)//基于RST_SHUT引脚拉高1s进入关机模式(主机命令)
				S2SD_flag = 1;
			
			if(S2SD_flag && (TS2_voltage > WAKEONTS2_TH && LD_voltage < WAKEONLD_TH))
			{
				mode_state = shutdown;
				S2SD_flag = 0;
				break;
			}
			else if(S2SD_flag && (TS2_voltage <=  WAKEONTS2_TH || LD_voltage >= WAKEONLD_TH))//软关机状态
			{
				mode_state = soft_shutdown;
				S2SD_flag = 0;
				break;
			}
			
			//进入正常模式
			if(readDirectMemory(SafetyStatusA) || readDirectMemory(SafetyStatusB) || readDirectMemory(SafetyStatusC))//基于保护故障退出SLEEP模式
			{
				mode_state = normal;
				S2N_flag = 1;
				break;
			}
			else if((5*current >= SLEEP_wake_current_th) || (CC1_current >= SLEEP_current_th))//基于显著电流退出SLEEP模式
			{
				mode_state = normal;
				S2N_flag = 1;
				break;
			}
			else if((pack_stack_Delta > SLEEP_pack_stack_delta) && (stack_voltage < SLEEP_charger_th))//基于充电器检测退出SLEEP模式
			{
				mode_state = normal;
				S2N_flag = 1;
				break;
			}
			else if(RST_SHUT_L_1s)//基于RST_SHUT引脚拉低1s退出SLEEP模式(也可以通过主机子命令)
			{
				mode_state = normal;
				S2N_flag = 1;
				break;
			}
			
			//进入DEEPSLEEP模式（仅主机子命令，内部无转换逻辑）

			break;
		case deepsleep:
			//进入关机模式
			if((stack_voltage < SHUTDOWN_stack_th) || (min_Vcell < SHUTDOWN_cell_th))//基于电池电压或电池组电压进入关机模式
				S2SD_flag = 1;
			if(S2SD_flag && (TS2_voltage > WAKEONTS2_TH && LD_voltage < WAKEONLD_TH))
			{
				mode_state = shutdown;
				S2SD_flag = 0;
				break;
			}
			else if(S2SD_flag && (TS2_voltage <=  WAKEONTS2_TH || LD_voltage >= WAKEONLD_TH))//软关机状态
			{
				mode_state = soft_shutdown;
				S2SD_flag = 0;
				break;
			}

			//进入正常模式
			if(RST_SHUT_L_1s)//基于RST_SHUT引脚拉低1s退出DEEPSLEEP模式(也可以通过主机子命令)
			{
				mode_state = normal;
				break;
			}
			if(LD_voltage> WAKEONLD_TH)//基于充电器检测退出DEEPSLEEP模式
			{
				mode_state = normal;
				break;
			}	
			break;
		case shutdown:
			//进入正常模式
			if((pack_stack_Delta > SLEEP_pack_stack_delta) || (TS2_voltage <= WAKEONTS2_TH))
				mode_state = normal;
			break;
		case soft_shutdown:
			//进入关机模式
			if(TS2_voltage > WAKEONTS2_TH && LD_voltage < WAKEONLD_TH)
			{
				mode_state = shutdown;
				break;
			}
			break;
		default:
			break;
		}
	*work_mode = mode_state;//1:NORMAL 2:SLEEP 3:DEEPSLEEP 4:SHUTDOWN 5:SOFT_SHUTDOWN
}
