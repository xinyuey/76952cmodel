/* OTC OTD OTF OTINT UTC UTD UTINT */

//相关寄存器
//#define TS1Config 0x92FD                    // Settings:Configuration:TS1 Config
//#define TS2Config 0x92FE                    // Settings:Configuration:TS2 Config
//#define TS3Config 0x92FF                    // Settings:Configuration:TS3 Config
//#define OTCThreshold 0x929A                 // Protections:OTC:Threshold
//#define OTCDelay 0x920B                     // Protections:OTC:Delay
//#define OTCRecovery 0x929C                  // Protections:OTC:Recovery
//#define OTDThreshold 0x929D                 // Protections:OTD:Threshold
//#define OTDDelay 0x929E                     // Protections:OTD:Delay
//#define OTDRecovery 0x929F                  // Protections:OTD:Recovery
//#define OTFThreshold 0x92A0                 // Protections:OTF:Threshold
//#define OTFDelay 0x92A1                     // Protections:OTF:Delay
//#define OTFRecovery 0x92A2                  // Protections:OTF:Recovery
//#define OTINTThreshold 0x92A3               // Protections:OTINT:Threshold
//#define OTINTDelay 0x92A4                   // Protections:OTINT:Delay
//#define OTINTRecovery 0x92A5                // Protections:OTINT:Recovery
//#define UTCThreshold 0x92A6                 // Protections:UTC:Threshold
//#define UTCDelay 0x92A7                     // Protections:UTC:Delay
//#define UTCRecovery 0x92A8                  // Protections:UTC:Recovery
//#define UTDThreshold 0x92A9                 // Protections:UTD:Threshold
//#define UTDDelay 0x92AA                     // Protections:UTD:Delay
//#define UTDRecovery 0x92AB                  // Protections:UTD:Recovery
//#define UTINTThreshold 0x92AC               // Protections:UTINT:Threshold
//#define UTINTDelay 0x92AD                   // Protections:UTINT:Delay
//#define UTINTRecovery 0x92AE                // Protections:UTINT:Recovery
//#define ProtectionsRecoveryTime 0x92AF      // Protections:Recovery:Time

#include "common.h"

#define TS_OPT_PULLUP	(BIT(7) | BIT(6))
#define TS_OPT_POLY		(BIT(5) | BIT(4))
#define TS_OPT_TYPE 	(BIT(3) | BIT(2))
#define TS_OPT_FUNC 	(BIT(1) | BIT(0))

StateMachine OTC_State = NORMAL;
StateMachine OTD_State = NORMAL;
StateMachine OTF_State = NORMAL;
StateMachine UTC_State = NORMAL;
StateMachine UTD_State = NORMAL;
StateMachine OTINT_State = NORMAL;
StateMachine UTINT_State = NORMAL;

uint8_t OTC_counter = 0;
uint8_t OTC_recounter = 0;
uint8_t OTD_counter = 0;
uint8_t OTD_recounter = 0;
uint8_t OTF_counter = 0;
uint8_t OTF_recounter = 0;
uint8_t UTC_counter = 0;
uint8_t UTC_recounter = 0;
uint8_t UTD_counter = 0;
uint8_t UTD_recounter = 0;
uint8_t OTINT_counter = 0;
uint8_t OTINT_recounter = 0;
uint8_t UTINT_counter = 0;
uint8_t UTINT_recounter = 0;

void OTC_protect
(
				//input
				const int16_t max_cell_temp,
				const uint8_t OTC_en,
				const int8_t OTC_th,
				const uint8_t OTC_delay,
				const int8_t OTC_rec_th,
				const uint8_t recoverytime,
				//output
				uint8_t *OTC_alert,
				uint8_t *OTC_error
)
{
	uint8_t alert,error;
	if(!OTC_en)
	{
		alert = 0;
		error = 0;
		OTC_State = NORMAL;
		OTC_counter = 0;
		OTC_recounter = 0;
	}
	else
	{
		switch (OTC_State)
	    {
	        case NORMAL:   
	            alert = 0;
	            error = 0;
	            if(max_cell_temp > OTC_th)
	            {
	                OTC_counter ++;
	                OTC_State = ALERT;
					printf("\nAlert: OTC\n");
	            }
	            break;
	        case ALERT:
	            alert = 1;
				error = 0;
	            if(OTC_counter == OTC_delay-1 && (max_cell_temp > OTC_th))
	            {
	                OTC_counter = 0;
	                OTC_State = TRIP;
					printf("\nError: OTC\n");
	            }    
	            else if (max_cell_temp <= OTC_th)
	            {
	                OTC_counter = 0;
	                OTC_State = NORMAL;
	            }
	            else
	                OTC_counter ++;
	            break;
	        case TRIP:
	            alert = 0;
	            error = 1;
	            if (OTC_recounter == recoverytime-1 && (max_cell_temp <= OTC_rec_th))
	            {
	                OTC_recounter = 0;
	                OTC_State = NORMAL;
					printf("\nRecovery: OTC\n");
	            }   
	            else if(max_cell_temp <= OTC_rec_th)
	            {
	                OTC_recounter ++;
	            }
	            break;
	        default:
	            break;
	    }
	}
    
    *OTC_alert = alert;
    *OTC_error = error;
}
void OTD_protect
(
				//input
				const int16_t max_cell_temp,
				const uint8_t OTD_en,
				const int8_t OTD_th,
				const uint8_t OTD_delay,
				const int8_t OTD_rec_th,
				const uint8_t recoverytime,
				//output
				uint8_t *OTD_alert,
				uint8_t *OTD_error
)
{
	uint8_t alert,error;
	if(!OTD_en)
	{
		alert = 0;
		error = 0;
		OTD_State = NORMAL;
		OTD_counter = 0;
		OTD_recounter = 0;
	}
	else
	{
		switch (OTD_State)
	    {
	        case NORMAL:   
	            alert = 0;
	            error = 0;
	            if(max_cell_temp > OTD_th)
	            {
	                OTD_counter ++;
	                OTD_State = ALERT;
					printf("\nAlert: OTD\n");
	            }
	            break;
	        case ALERT:
	            alert = 1;
				error = 0;
	            if(OTD_counter == OTD_delay-1 && (max_cell_temp > OTD_th))
	            {
	                OTD_counter = 0;
	                OTD_State = TRIP;
					printf("\nError: OTD\n");
	            }    
	            else if (max_cell_temp <= OTD_th)
	            {
	                OTD_counter = 0;
	                OTD_State = NORMAL;
	            }
	            else
	                OTD_counter ++;
	            break;
	        case TRIP:
	            alert = 0;
	            error = 1;
	            if (OTD_recounter == recoverytime-1 && (max_cell_temp <= OTD_rec_th))
	            {
	                OTD_recounter = 0;
	                OTD_State = NORMAL;
					printf("\nRecovery: OTD\n");
	            }   
	            else if(max_cell_temp <= OTD_rec_th)
	            {
	                OTD_recounter ++;
	            }
	            break;
	        default:
	            break;
	    }
	}
	
    *OTD_alert = alert;
    *OTD_error = error;
}
void OTF_protect
(
				//input
				const int16_t max_fet_temp,
				const uint8_t OTF_en,
				const int8_t OTF_th,
				const uint8_t OTF_delay,
				const int8_t OTF_rec_th,
				const uint8_t recoverytime,
				//output
				uint8_t *OTF_alert,
				uint8_t *OTF_error
)
{
	uint8_t alert,error;
	if(!OTF_en)
	{
		alert = 0;
		error = 0;
		OTF_State = NORMAL;
		OTF_counter = 0;
		OTF_recounter = 0;
	}
	else
	{
	    switch (OTF_State)
	    {
	        case NORMAL:   
	            alert = 0;
	            error = 0;
	            if(max_fet_temp > OTF_th)
	            {
	                OTF_counter ++;
	                OTF_State = ALERT;
					printf("\nAlert: OTF\n");
	            }
	            break;
	        case ALERT:
	            alert = 1;
				error = 0;
	            if(OTF_counter == OTF_delay-1 && (max_fet_temp > OTF_th))
	            {
	                OTF_counter = 0;
	                OTF_State = TRIP;
					printf("\nError: OTF\n");
	            }    
	            else if (max_fet_temp <= OTF_th)
	            {
	                OTF_counter = 0;
	                OTF_State = NORMAL;
	            }
	            else
	                OTF_counter ++;
	            break;
	        case TRIP:
	            alert = 0;
	            error = 1;
	            if (OTF_recounter == recoverytime-1 && (max_fet_temp <= OTF_rec_th))
	            {
	                OTF_recounter = 0;
	                OTF_State = NORMAL;
					printf("\nRecovery: OTF\n");
	            }   
	            else if(max_fet_temp <= OTF_rec_th)
	            {
	                OTF_recounter ++;
	            }
	            break;
	        default:
	            break;
	    }
	}
	
    *OTF_alert = alert;
    *OTF_error = error;
}
void UTC_protect
(
				//input
				const int16_t min_cell_temp,
				const uint8_t UTC_en,
				const int8_t UTC_th,
				const uint8_t UTC_delay,
				const int8_t UTC_rec_th,
				const uint8_t recoverytime,
				//output
				uint8_t *UTC_alert,
				uint8_t *UTC_error
)
{
	uint8_t alert,error;
	if(!UTC_en)
	{
		alert = 0;
		error = 0;
		UTC_State = NORMAL;
		UTC_counter = 0;
		UTC_recounter = 0;
	}
	else
	{
	    switch (UTC_State)
	    {
	        case NORMAL:   
	            alert = 0;
	            error = 0;
	            if(min_cell_temp < UTC_th)
	            {
	                UTC_counter ++;
	                UTC_State = ALERT;
					printf("\nAlert: UTC\n");
	            }
	            break;
	        case ALERT:
	            alert = 1;
				error = 0;
	            if(UTC_counter == UTC_delay-1 && (min_cell_temp < UTC_th))
	            {
	                UTC_counter = 0;
	                UTC_State = TRIP;
					printf("\nError: UTC\n");
	            }    
	            else if (min_cell_temp >= UTC_th)
	            {
	                UTC_counter = 0;
	                UTC_State = NORMAL;
	            }
	            else
	                UTC_counter ++;
	            break;
	        case TRIP:
	            alert = 0;
	            error = 1;
	            if (UTC_recounter == recoverytime-1 && (min_cell_temp >= UTC_rec_th))
	            {
	                UTC_recounter = 0;
	                UTC_State = NORMAL;
					printf("\nRecovery: UTC\n");
	            }   
	            else if(min_cell_temp >= UTC_rec_th)
	            {
	                UTC_recounter ++;
	            }
	            break;
	        default:
	            break;
	    }
	}
	
    *UTC_alert = alert;
    *UTC_error = error;
}
void UTD_protect
(
				//input
				const int16_t min_cell_temp,
				const uint8_t UTD_en,
				const int8_t UTD_th,
				const uint8_t UTD_delay,
				const int8_t UTD_rec_th,
				const uint8_t recoverytime,
				//output
				uint8_t *UTD_alert,
				uint8_t *UTD_error
)
{
	uint8_t alert,error;
	if(!UTD_en)
	{
		alert = 0;
		error = 0;
		UTD_State = NORMAL;
		UTD_counter = 0;
		UTD_recounter = 0;
	}
	else
	{
	    switch (UTD_State)
	    {
	        case NORMAL:   
	            alert = 0;
	            error = 0;
	            if(min_cell_temp < UTD_th)
	            {
	                UTD_counter ++;
	                UTD_State = ALERT;
					printf("\nAlert: UTD\n");
	            }
	            break;
	        case ALERT:
	            alert = 1;
				error = 0;
	            if(UTD_counter == UTD_delay-1 && (min_cell_temp < UTD_th))
	            {
	                UTD_counter = 0;
	                UTD_State = TRIP;
					printf("\nError: UTD\n");
	            }    
	            else if (min_cell_temp >= UTD_th)
	            {
	                UTD_counter = 0;
	                UTD_State = NORMAL;
	            }
	            else
	                UTD_counter ++;
	            break;
	        case TRIP:
	            alert = 0;
	            error = 1;
	            if (UTD_recounter == recoverytime-1 && (min_cell_temp >= UTD_rec_th))
	            {
	                UTD_recounter = 0;
	                UTD_State = NORMAL;
					printf("\nRecovery: UTD\n");
	            }   
	            else if(min_cell_temp >= UTD_rec_th)
	            {
	                UTD_recounter ++;
	            }
	            break;
	        default:
	            break;
	    }
	}
	
    *UTD_alert = alert;
    *UTD_error = error;
}
void OTINT_protect
(
				//input
				const int16_t Internal_Temp,
				const uint8_t OTINT_en,
				const int8_t OTINT_th,
				const uint8_t OTINT_delay,
				const int8_t OTINT_rec_th,
				const uint8_t recoverytime,
				//output
				uint8_t *OTINT_alert,
				uint8_t *OTINT_error
)
{
	uint8_t alert,error;
	if(!OTINT_en)
	{
		alert = 0;
		error = 0;
		OTINT_State = NORMAL;
		OTINT_counter = 0;
		OTINT_recounter = 0;
	}
	else
	{
	    switch (OTINT_State)
	    {
	        case NORMAL:   
	            alert = 0;
	            error = 0;
	            if(Internal_Temp > OTINT_th)
	            {
	                OTINT_counter ++;
	                OTINT_State = ALERT;
					printf("\nAlert:  OTINT\n");
	            }
	            break;
	        case ALERT:
	            alert = 1;
				error = 0;
	            if(OTINT_counter == OTINT_delay-1 && (Internal_Temp > OTINT_th))
	            {
	                OTINT_counter = 0;
	                OTINT_State = TRIP;
					printf("\nError:  OTINT\n");
	            }    
	            else if (Internal_Temp <= OTINT_th)
	            {
	                OTINT_counter = 0;
	                OTINT_State = NORMAL;
	            }
	            else
	                OTINT_counter ++;
	            break;
	        case TRIP:
	            alert = 0;
	            error = 1;
	            if (OTINT_recounter == recoverytime-1 && (Internal_Temp <= OTINT_rec_th))
	            {
	                OTINT_recounter = 0;
	                OTINT_State = NORMAL;
					printf("\nRecovery:  OTINT\n");
	            }   
	            else if(Internal_Temp <= OTINT_rec_th)
	            {
	                OTINT_recounter ++;
	            }
	            break;
	        default:
	            break;
	    }
	}
	
    *OTINT_alert = alert;
    *OTINT_error = error;
}
void UTINT_protect
(
				//input
				const int16_t Internal_Temp,
				const uint8_t UTINT_en,
				const int8_t UTINT_th,
				const uint8_t UTINT_delay,
				const int8_t UTINT_rec_th,
				const uint8_t recoverytime,
				//output
				uint8_t *UTINT_alert,
				uint8_t *UTINT_error
)
{
	uint8_t alert,error;
	if(!UTINT_en)
	{
		alert = 0;
		error = 0;
		UTINT_State = NORMAL;
		UTINT_counter = 0;
		UTINT_recounter = 0;
	}
	else
	{
	    switch (UTINT_State)
	    {
	        case NORMAL:   
	            alert = 0;
	            error = 0;
	            if(Internal_Temp < UTINT_th)
	            {
	                UTINT_counter ++;
	                UTINT_State = ALERT;
					printf("\nAlert:  UTINT\n");
	            }
	            break;
	        case ALERT:
	            alert = 1;
				error = 0;
	            if(UTINT_counter == UTINT_delay-1 && (Internal_Temp < UTINT_th))
	            {
	                UTINT_counter = 0;
	                UTINT_State = TRIP;
					printf("\nError:  UTINT\n");
	            }    
	            else if (Internal_Temp >= UTINT_th)
	            {
	                UTINT_counter = 0;
	                UTINT_State = NORMAL;
	            }
	            else
	                UTINT_counter ++;
	            break;
	        case TRIP:
	            alert = 0;
	            error = 1;
	            if (UTINT_recounter == recoverytime-1 && (Internal_Temp >= UTINT_rec_th))
	            {
	                UTINT_recounter = 0;
	                UTINT_State = NORMAL;
					printf("\nRecovery:  UTINT\n");
	            }   
	            else if(Internal_Temp >= UTINT_rec_th)
	            {
	                UTINT_recounter ++;
	            }
	            break;
	        default:
	            break;
	    }
	}
	
    *UTINT_alert = alert;
    *UTINT_error = error;
}

void InternT_protect
(
				//input
				const int16_t Internal_Temp,
				const uint8_t TINT_en,
				const uint8_t TINT_fett,
				const uint8_t OTINT_en,
				const int8_t OTINT_th,
				const uint8_t OTINT_delay,
				const int8_t OTINT_rec_th,
				const uint8_t UTINT_en,
				const int8_t UTINT_th,
				const uint8_t UTINT_delay,
				const int8_t UTINT_rec_th,
				const uint8_t recoverytime,
				//output
				uint8_t *OTINT_alert,
				uint8_t *OTINT_error,
				uint8_t *UTINT_alert,
				uint8_t *UTINT_error
)
{
	uint8_t otint_alert,otint_error;
	uint8_t utint_alert,utint_error;
	if(TINT_en==1 || TINT_fett==1)//Internal_Temp被用作电池温度保护或FET温度保护，不用作内部温度保护
		return;
	else//内部温度保护
	{
		OTINT_protect(
					//input
					Internal_Temp,
					OTINT_en,
					OTINT_th,
					OTINT_delay,
					OTINT_rec_th,
					recoverytime,
					//output
					&otint_alert,
					&otint_error
					);
		UTINT_protect(
					//input
					Internal_Temp,
					UTINT_en,
					UTINT_th,
					UTINT_delay,
					UTINT_rec_th,
					recoverytime,
					//output
					&utint_alert,
					&utint_error
					);

		*OTINT_alert = otint_alert;
		*OTINT_error = otint_error;
		*UTINT_alert = utint_alert;
		*UTINT_error = utint_error;
	}
}

void TermistorT_protect
(
				//input
				const int16_t TS1,
				const int16_t TS2,
				const int16_t TS3,
				const uint8_t TS1_config,
				const uint8_t TS2_config,
				const uint8_t TS3_config,
				const int16_t Internal_Temp,
				const uint8_t TINT_en,
				const uint8_t TINT_fett,
				const uint8_t OTC_en,
				const int8_t OTC_th,
				const uint8_t OTC_delay,
				const int8_t OTC_rec_th,
				const uint8_t OTD_en,
				const int8_t OTD_th,
				const uint8_t OTD_delay,
				const int8_t OTD_rec_th,
				const uint8_t OTF_en,
				const int8_t OTF_th,
				const uint8_t OTF_delay,
				const uint8_t OTF_rec_th,
				const uint8_t UTC_en,
				const int8_t UTC_th,
				const uint8_t UTC_delay,
				const int8_t UTC_rec_th,
				const uint8_t UTD_en,
				const int8_t UTD_th,
				const uint8_t UTD_delay,
				const int8_t UTD_rec_th,
				const uint8_t recoverytime,
				//output
				uint8_t *OTC_alert,
				uint8_t *OTC_error,
				uint8_t *OTD_alert,
				uint8_t *OTD_error,
				uint8_t *OTF_alert,
				uint8_t *OTF_error,
				uint8_t *UTC_alert,
				uint8_t *UTC_error,
				uint8_t *UTD_alert,
				uint8_t *UTD_error
)
{
	uint8_t otc_alert=0,otc_error=0;
	uint8_t otd_alert=0,otd_error=0;
	uint8_t otf_alert=0,otf_error=0;
	uint8_t utc_alert=0,utc_error=0;
	uint8_t utd_alert=0,utd_error=0;
	
	int16_t cell_temp[4] = {0};						//TS1,TS2,TS3,Internal_Temp
	uint8_t cell_temp_f[4] = {0};
	int16_t fet_temp[4] = {0};						//TS1,TS2,TS3,Internal_Temp
	uint8_t fet_temp_f[4] = {0};
	uint8_t TS1_pullup,TS2_pullup,TS3_pullup;		//热敏电阻上拉类型（18k/180k）
	uint8_t TS1_poly,TS2_poly,TS3_poly;				//热敏电阻多项式类型（18k/180k）
	uint8_t TS1_type,TS2_type,TS3_type;				//热敏电阻温度复用
	uint8_t TS1_func,TS2_func,TS3_func;				//热敏电阻是否使能
	TS1_pullup = (TS_OPT_PULLUP & TS1_config) >> 6;	//Settings:Configuration:TS1 Config[7:6]
	TS1_poly = (TS_OPT_POLY & TS1_config) >> 4; 	//Settings:Configuration:TS1 Config[5:4]
	TS1_type = (TS_OPT_TYPE & TS1_config) >> 2; 	//Settings:Configuration:TS1 Config[3:2]
	TS1_func = TS_OPT_FUNC & TS1_config;	    	//Settings:Configuration:TS1 Config[1:0]
	TS2_pullup = (TS_OPT_PULLUP & TS2_config) >> 6;	//Settings:Configuration:TS2 Config[7:6]
	TS2_poly = (TS_OPT_POLY & TS2_config) >> 4;		//Settings:Configuration:TS2 Config[5:4]
	TS2_type = (TS_OPT_TYPE & TS2_config) >> 2;		//Settings:Configuration:TS2 Config[3:2]
	TS2_func = TS_OPT_FUNC & TS2_config;			//Settings:Configuration:TS2 Config[1:0]
	TS3_pullup = (TS_OPT_PULLUP & TS3_config) >> 6;	//Settings:Configuration:TS3 Config[7:6]
	TS3_poly = (TS_OPT_POLY & TS3_config) >> 4;		//Settings:Configuration:TS3 Config[5:4]
	TS3_type = (TS_OPT_TYPE & TS3_config) >> 2;		//Settings:Configuration:TS3 Config[3:2]
	TS3_func = TS_OPT_FUNC & TS3_config;			//Settings:Configuration:TS3 Config[1:0]
	
	if(TS1_func == 0x03 && TS1_type == 0x01)//电池温度
	{
		cell_temp[0] = TS1;//实际上TS1寄存器存储的是电阻值，此处应进行电阻值和温度值的转换
		cell_temp_f[0] = 1;
		fet_temp_f[0] = 0;
	}
	else if(TS1_func == 0x03 && TS1_type == 0x03)//FET温度
	{
		fet_temp[0] = TS1;
		fet_temp_f[0] = 1;
		cell_temp_f[0] = 0;
	}
	else
	{
		cell_temp[0] = 0;
		cell_temp_f[0] = 0;
		fet_temp[0] = 0;
		fet_temp_f[0] = 0;
	}

	if(TS2_func == 0x03 && TS2_type == 0x01)//电池温度
	{
		cell_temp[1] = TS2;
		cell_temp_f[1] = 1;
		fet_temp_f[1] = 0;
	}
	else if(TS2_func == 0x03 && TS2_type == 0x03)//FET温度
	{
		fet_temp[1] = TS2;
		fet_temp_f[1] = 1;
		cell_temp_f[1] = 0;
	}
	else
	{
		cell_temp[1] = 0;
		cell_temp_f[1] = 0;
		fet_temp[1] = 0;
		fet_temp_f[1] = 0;
	}

	if(TS3_func == 0x03 && TS3_type == 0x01)//电池温度
	{
		cell_temp[2] = TS3;
		cell_temp_f[2] = 1;
		fet_temp_f[2] = 0;
	}
	else if(TS3_func == 0x03 && TS3_type == 0x03)//FET温度
	{
		fet_temp[2] = TS3;
		fet_temp_f[2] = 1;
		cell_temp_f[2] = 0;
	}
	else
	{
		cell_temp[2] = 0;
		cell_temp_f[2] = 0;
		fet_temp[2] = 0;
		fet_temp_f[2] = 0;
	}

	if(TINT_en == 1 && TINT_fett != 1)//电池温度
	{
		cell_temp[3] = Internal_Temp;
		cell_temp_f[3] = 1;
		fet_temp_f[3] = 0;
	}
	else if(TINT_en != 1 && TINT_fett == 1)//FET温度
	{
		fet_temp[3] = Internal_Temp;
		fet_temp_f[3] = 1;
		cell_temp_f[3] = 0;
	}
	else
	{
		cell_temp[3] = 0;
		cell_temp_f[3] = 0;
		fet_temp[3] = 0;
		fet_temp_f[3] = 0;
	}

	//min_cell_temp
	int16_t min_cell_temp = 0x7fff;
	for(int i=0;i<4;i++)
	{
		if((cell_temp_f[i] == 1) && (cell_temp[i] < min_cell_temp))
			min_cell_temp = cell_temp[i];	
	}

	//max_cell_temp
	int16_t max_cell_temp = 0x8000;
	for(int i=0;i<4;i++)
	{
		if((cell_temp_f[i] == 1) && (cell_temp[i] > max_cell_temp))
			max_cell_temp = cell_temp[i];
	}
	
	//max_fet_temp
	int16_t max_fet_temp = 0x8000;
	for(int i=0;i<4;i++)
	{
		if((fet_temp_f[i] == 1) && (fet_temp[i] > max_fet_temp))
			max_fet_temp = fet_temp[i];		
	}

	if(cell_temp_f)
	{
		OTC_protect(
					//input
					max_cell_temp,
					OTC_en,
					OTC_th,
					OTC_delay,
					OTC_rec_th,
					recoverytime,
					//output
					&otc_alert,
					&otc_error);
		//OTD_protect
		OTD_protect(
					//input
					max_cell_temp,
					OTD_en,
					OTD_th,
					OTD_delay,
					OTD_rec_th,
					recoverytime,
					//output
					&otd_alert,
					&otd_error);
		//UTC_protect
		UTC_protect(
					//input
					min_cell_temp,
					UTC_en,
					UTC_th,
					UTC_delay,
					UTC_rec_th,
					recoverytime,
					//output
					&utc_alert,
					&utc_error);
		//UTD_protect
		UTD_protect(
					//input
					min_cell_temp,
					UTD_en,
					UTD_th,
					UTD_delay,
					UTD_rec_th,
					recoverytime,
					//output
					&utd_alert,
					&utd_error);
	}
		
	if(fet_temp_f)
	{
		//OTF_protect
		OTF_protect(
					//input
					max_fet_temp,
					OTF_en,
					OTF_th,
					OTF_delay,
					OTF_rec_th,
					recoverytime,
					//output
					&otf_alert,
					&otf_error);
	}

	*OTC_alert = otc_alert;
	*OTC_error = otc_error;
	*OTD_alert = otd_alert;
	*OTD_error = otd_error;
	*OTF_alert = otf_alert;
	*OTF_error = otf_error;
	*UTC_alert = utc_alert;
	*UTC_error = utc_error;
	*UTD_alert = utd_alert;
	*UTD_error = utd_error;
}
