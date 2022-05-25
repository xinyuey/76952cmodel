/* SOV SUV 2LVL */

//相关寄存器
//#define MfgStatusInit 0x9343                 	// Settings:Manufacturing:Mfg Status Init[PF_EN]
//#define ProtectionConfiguration 0x925F      	// Settings:Protection:Protection Configuration[PF_FETS],[PF_FUSE]
//#define EnabledPFA 0x92C0                   	// Settings:Permanent Failure:Enabled PF A
//#define EnabledPFB 0x92C1                   	// Settings:Permanent Failure:Enabled PF B
//#define EnabledPFC 0x92C2                   	// Settings:Permanent Failure:Enabled PF C
//#define EnabledPFD 0x92C3                   	// Settings:Permanent Failure:Enabled PF D
//#define SUVThreshold 0x92CB                 	// Permanent Fail:SUV:Threshold
//#define SUVDelay 0x92CD                     	// Permanent Fail:SUV:Delay
//#define SOVThreshold 0x92CE                 	// Permanent Fail:SOV:Threshold
//#define SOVDelay 0x92D0                     	// Permanent Fail:SOV:Delay
//#define PF2LVLDelay 0x92F7                  	// Permanent Fail:2LVL:Delay

#include "common.h"

uint8_t SOV_counter = 0;       	//SOV故障计数
uint8_t SUV_counter = 0;       	//SUV故障计数
uint8_t LVL2_counter = 0;		//LVL2故障计数

StateMachine SOV_State = NORMAL;
StateMachine SUV_State = NORMAL;
StateMachine LVL2_State = NORMAL;

void SOV_protect
(
				//input
                const uint16_t *CellVoltage, 
                const uint8_t PF_en,
                const uint8_t SOV_en,
                const int16_t SOV_th,          
                const uint16_t SOV_delay,      
                //output
                uint8_t *SOV_alert,             
                uint8_t *SOV_error   
)
{
	uint8_t alert,error;
	if(!PF_en || !SOV_en)//二次保护不使能，或SOV保护不使能
	{
		alert = 0;
		error = 0;
		SOV_State = NORMAL;
		SOV_counter = 0;
	}
	else
	{
	    switch (SOV_State)
	    {
	        case NORMAL:   	     
	            alert = 0;
	            error = 0;
	            if(COV_Comp(CellVoltage,SOV_th))
	            {
	                SOV_counter ++;
	                SOV_State = ALERT;
					printf("\nAlert: SOV\n");
	            }
	            break;
	        case ALERT:
	            alert = 1;
				error = 0;
	            if(SOV_counter == SOV_delay-1 && COV_Comp(CellVoltage,SOV_th))
	            {
	                SOV_counter = 0;
	                SOV_State = TRIP;
					printf("\nError: SOV\n");
	            }    
	            else if (!COV_Comp(CellVoltage,SOV_th))
	            {
	                SOV_counter = 0;
	                SOV_State = NORMAL;
	            }
	            else
	                SOV_counter ++;
	            break;
	        case TRIP:
	            alert = 0;
	            error = 1;
	            break;
	        default:
	            break;
	    }
	}

	*SOV_alert = alert;
	*SOV_error = error;
}
void SUV_protect
(
				//input
                const uint16_t *CellVoltage, 
                const uint8_t PF_en,
                const uint8_t SUV_en,
                const int16_t SUV_th,          
                const uint16_t SUV_delay,      
                //output
                uint8_t *SUV_alert,             
                uint8_t *SUV_error   
)
{
	uint8_t alert,error;
	if(!PF_en || !SUV_en)//二次保护不使能，或SOV保护不使能
	{
		alert = 0;
		error = 0;
		SUV_State = NORMAL;
		SUV_counter = 0;
	}
	else
	{
	    switch (SUV_State)
	    {
	        case NORMAL:   	     
	            alert = 0;
	            error = 0;
	            if(CUV_Comp(CellVoltage,SUV_th))
	            {
	                SUV_counter ++;
	                SUV_State = ALERT;
					printf("\nAlert: SUV\n");
	            }
	            break;
	        case ALERT:
	            alert = 1;
				error = 0;
	            if(SUV_counter == SUV_delay-1 && CUV_Comp(CellVoltage,SUV_th))
	            {
	                SUV_counter = 0;
	                SUV_State = TRIP;
					printf("\nError: SUV\n");
	            }    
	            else if (!CUV_Comp(CellVoltage,SUV_th))
	            {
	                SUV_counter = 0;
	                SUV_State = NORMAL;
	            }
	            else
	                SUV_counter ++;
	            break;
	        case TRIP:
	            alert = 0;
	            error = 1;
	            break;
	        default:
	            break;
	    }
	}
	
	*SUV_alert = alert;
	*SUV_error = error;
}

void LVL2_protect
(
				//input
                const uint8_t fuse_in,
                const uint8_t PF_en,
                const uint8_t LVL2_en,
                const uint8_t LVL2_delay,
                //output
				uint8_t *LVL2_alert,
				uint8_t *LVL2_error
)
{
	uint8_t alert,error;
	if(!PF_en || !LVL2_en)//二次保护不使能，或SOV保护不使能
	{
		alert = 0;
		error = 0;
		LVL2_State = 0;
		LVL2_counter = 0;
	}
	else
	{
	    switch (LVL2_State)
	    {
	        case NORMAL:   	     
	            alert = 0;
	            error = 0;
	            if(fuse_in)
	            {
	                LVL2_counter ++;
	                LVL2_State = ALERT;
					printf("\nAlert: LVL2\n");
	            }
	            break;
	        case ALERT:
	            alert = 1;
				error = 0;
	            if(LVL2_counter == LVL2_delay-1 && fuse_in)
	            {
	                LVL2_counter = 0;
	                LVL2_State = TRIP;
					printf("\nError: LVL2\n");
	            }    
	            else if (!fuse_in)
	            {
	                LVL2_counter = 0;
	                LVL2_State = NORMAL;
	            }
	            else
	                LVL2_counter ++;
	            break;
	        case TRIP:
	            alert = 0;
	            error = 1;
	            break;
	        default:
	            break;
	    }
	}
	
	*LVL2_alert = alert;
	*LVL2_error = error;
}
