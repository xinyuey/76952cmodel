/*OV UV*/

#include "common.h"

uint8_t COV_counter = 0;        //COV事件计数
uint8_t COV_recounter = 0;      //COV恢复事件计数
uint8_t CUV_counter = 0;        //CUV事件计数
uint8_t CUV_recounter = 0;      //COV恢复事件计数
uint8_t COVL_counter = 0;       //COV故障计数
uint8_t COVL_DEC_counter = 0;   //计满仍未再次触发COV，则递减COVL_counter
uint8_t COVL_REC_counter = 0;   //计满后，COVL自动恢复

StateMachine COV_State = NORMAL;
StateMachine CUV_State = NORMAL;
StateMachine_Lacth COVL_State = LATCH_RESET;

uint8_t COV_Comp(const uint16_t *CellVoltage,uint16_t threshold)
{
    uint16_t max_Vcell = 0;
    for(int i=0;i<16;i++)
    {
        if(*CellVoltage > max_Vcell)
            max_Vcell = *CellVoltage;
        CellVoltage ++;
    }

    if(max_Vcell >= threshold)
        return 1;
    else
        return 0;
}

uint8_t CUV_Comp(const uint16_t *CellVoltage,uint16_t threshold)
{
    uint16_t min_Vcell = *CellVoltage;
    for(int i=1;i<16;i++)
    {
        if(*CellVoltage < min_Vcell)
            min_Vcell = *CellVoltage;
        CellVoltage ++;
    }
    if(min_Vcell <= threshold)
        return 1;
    else
        return 0;
}

void COV_protect
(
				//input
                const uint16_t *CellVoltage,
                const uint8_t COV_en,
                const uint16_t COV_th,          
                const uint16_t COV_delay,      
                const uint8_t COV_rec_hys,         
                const uint8_t recoverytime,
                //output
                uint8_t *COV_alert,             
                uint8_t *COV_error              
)              
{
    uint8_t alert,error;
	if(!COV_en)
	{
		alert = 0;
		error = 0;
		COV_State = NORMAL;
		COV_counter = 0;
		COV_recounter = 0;
		COVL_DEC_counter = 0;
		
	}
	else
	{
		switch (COV_State)
	    {
	        case NORMAL:
	            alert = 0;
	            error = 0;
	            if(COV_Comp(CellVoltage,COV_th))
	            {
	                COV_counter ++;
	                COV_State = ALERT;
					printf("\nAlert: COV\n");
	            }
	            else if(COVL_counter > 0)
	                COVL_DEC_counter ++;
	            break;
	        case ALERT:
	            alert = 1;
				error = 0;
	            if(COV_counter == COV_delay-1 && COV_Comp(CellVoltage,COV_th))
	            {
	                COV_counter = 0;
	                COV_State = TRIP;
					printf("\nError: COV\n");
	                if(COVL_State != LATCH_TRIP)    //COVL触发后就不计数了
	                    COVL_counter ++;    //放在这是为识别COV_Fault上升沿，即COV_Fault上升沿计数值达到锁存阈值，会触发COVL
	            }    
	            else if (!COV_Comp(CellVoltage,COV_th))
	            {
	                COV_counter = 0;
	                COV_State = NORMAL;
	            }
	            else
	                COV_counter ++;
	            break;
	        case TRIP:
	            alert = 0;
	            error = 1;
	            if (COV_recounter == recoverytime-1 && !COV_Comp(CellVoltage,(COV_th - COV_rec_hys)))
	            {
	                COV_recounter = 0;
	                COVL_DEC_counter = 0;
	                COV_State = NORMAL;
					printf("\nRecovery: COV\n");
	            }   
	            else if(!COV_Comp(CellVoltage,(COV_th - COV_rec_hys)))
	            {
	                COV_recounter ++;
	            }
	            break;
	        default:
	            break;
	    }
	}
	
    *COV_alert = alert;
    *COV_error = error;

}

void CUV_protect
(
				//input
				const uint16_t *CellVoltage,
				const uint8_t CUV_en,
                const uint16_t CUV_th,          
                const uint16_t CUV_delay,       
                const uint8_t CUV_rec_hys,         
                const uint8_t recoverytime,     
                //output
                uint8_t *CUV_alert,             
                uint8_t *CUV_error              
)
{
    uint8_t alert,error;
	if(!CUV_en)
	{
		alert = 0;
		error = 0;
		CUV_State = NORMAL;
		CUV_counter = 0;
		CUV_recounter = 0;
	}
	else
	{
	    switch (CUV_State)
	    {
	        case NORMAL:   
	            alert = 0;
	            error = 0;
	            if(CUV_Comp(CellVoltage,CUV_th))
	            {
	                CUV_counter ++;
	                CUV_State = ALERT;
					printf("\nAlert: CUV\n");
	            }
	            break;
	        case ALERT:
	            alert = 1;
				error = 0;
	            if(CUV_counter == CUV_delay-1 && CUV_Comp(CellVoltage,CUV_th))
	            {
	                CUV_counter = 0;
	                CUV_State = TRIP;
					printf("\nError: CUV\n");
	            }    
	            else if (!CUV_Comp(CellVoltage,CUV_th))
	            {
	                CUV_counter = 0;
	                CUV_State = NORMAL;
	            }
	            else
	                CUV_counter ++;
	            break;
	        case TRIP:
	            alert = 0;
	            error = 1;
	            if (CUV_recounter == recoverytime-1 && !CUV_Comp(CellVoltage,(CUV_th + CUV_rec_hys)))
	            {
	                CUV_recounter = 0;
	                CUV_State = NORMAL;
					printf("\nRecovery: CUV\n");
	            }   
	            else if(!CUV_Comp(CellVoltage,(CUV_th + CUV_rec_hys)))
	            {
	                CUV_recounter ++;
	            }
	            break;
	        default:
	            break;
	    }
	}
	
    *CUV_alert = alert;
    *CUV_error = error;
}

void COVL_protect
(
				//input
				const uint8_t COVL_en,
                const uint8_t COVL_limit,           
                const uint8_t COVL_dec_delay,       
                const uint8_t COVL_recoverytime,
                //output
                uint8_t *COVL_alert,                
                uint8_t *COVL_error                 
)
{
    uint8_t alert,error;
	if(!COVL_en)
	{
		alert = 0;
		error = 0;
		COVL_State =  LATCH_RESET;
		COVL_counter = 0;
		COVL_REC_counter = 0;
	}
	else
	{
		switch (COVL_State)
	    {
	        case LATCH_RESET:   
	            alert = 0;
	            error = 0;
	            if(COVL_counter > 0)
	            {
					COVL_State = LATCH_ALERT;
					printf("\nAlert: COV Latch\n");
				}
	            break;
	        case LATCH_ALERT:
	            alert = 1;
				error = 0;
	            if(COVL_counter == COVL_limit)
	            {
	                COVL_counter = 0;
	                COVL_State = LATCH_TRIP;
					printf("\nError: COV Latch\n");
	            }
	            else if(COVL_counter == 0)
	                COVL_State = LATCH_RESET;
	            else if(COVL_DEC_counter >= COVL_dec_delay)    
	                COVL_counter --;
	            break;
	        case LATCH_TRIP:
	            alert = 0;
	            error = 1;
	            if(COVL_REC_counter == COVL_recoverytime-1)
	            {
	                COVL_REC_counter = 0;
	                COVL_State = LATCH_RESET;
					printf("\nRecovery: COV Latch\n");
	            } 
	            else
	                COVL_REC_counter ++;    //COVL故障定时自动恢复
	            break;
	        default:
	            break;
	    }
	}
	
    *COVL_alert = alert;
    *COVL_error = error;
}
