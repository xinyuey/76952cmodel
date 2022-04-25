/*OV UV*/

#include "common.h"

// #define COV_ALERT BIT(3)
// #define COV_FAULT BIT(3)
// #define CUV_ALERT BIT(2)
// #define CUV_FAULT BIT(2)
// #define COVL_ALERT BIT(4)
// #define COVL_FAULT BIT(4)

// #define COV_CHG BIT(3)
// #define COVL_CHG BIT(4)
// #define CUV_DSG BIT(2)
// #define XCHG BIT(6)
// #define XDSG BIT(5)

//extern uint16_t CellVoltage[];

// uint16_t CUV_TH = 3000;         
// uint16_t COV_TH = 5000; 
// uint16_t CUV_REC = 0;
// uint16_t COV_REC = 0;
// uint16_t CUV_DLY = 2;
// uint16_t COV_DLY = 2;           

// uint8_t COVL_Limit = 4;        
// uint8_t COVL_DEC_DLY = 100; 
// uint8_t COVL_RecoveryTime = 150;

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

uint8_t COV_Comp(uint16_t threshold,const uint16_t *CellVoltage)
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

uint8_t CUV_Comp(uint16_t threshold,const uint16_t *CellVoltage)
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
                const uint16_t *CellVoltage,     //input
                const uint16_t COV_th,          //input
                const uint16_t COV_delay,       //input
                const uint16_t COV_rec,         //input
                const uint8_t recoverytime,     //input
                uint8_t *COV_alert,             //output
                uint8_t *COV_error              //output
)              
{
    uint8_t alert,error;
    switch (COV_State)
    {
        case NORMAL:   
            // clearFlags(COV_ALERT,SafetyAlertA);
			// clearFlags(COV_FAULT,SafetyStatusA);
            // if(COV_CHG & *readSettings(CHGFETProtectionsA))//FET自主控制已开启
            //     clearFlags(XCHG,AlarmRawStatus);
            alert = 0;
            error = 0;
            if(COV_Comp(COV_th,CellVoltage))
            {
                COV_counter ++;
                COV_State = ALERT;
				printf("\nAlert: COV\n");
            }
            else if(COVL_counter > 0)
                COVL_DEC_counter ++;
            break;
        case ALERT:
            // setFlags(COV_ALERT,SafetyAlertA);	//Safety Alert A[COV] = 1;
            alert = 1;
            if(COV_counter == COV_delay-1 && COV_Comp(COV_th,CellVoltage))
            {
                COV_counter = 0;
                COV_State = TRIP;
				printf("\nError: COV\n");
                if(COVL_State != LATCH_TRIP)    //COVL触发后就不计数了
                    COVL_counter ++;    //放在这是为识别COV_Fault上升沿，即COV_Fault上升沿计数值达到锁存阈值，会触发COVL
            }    
            else if (!COV_Comp(COV_th,CellVoltage))
            {
                COV_counter = 0;
                COV_State = NORMAL;
            }
            else
                COV_counter ++;
            break;
        case TRIP:
			// clearFlags(COV_ALERT,SafetyAlertA);		//Safety Alert A[COV] = 0;
			// setFlags(COV_FAULT,SafetyStatusA);		//Safety Status A[COV] = 1;
            // if(COV_CHG & *readSettings(CHGFETProtectionsA))//FET自主控制已开启
            //     setFlags(XCHG,AlarmRawStatus);
            alert = 0;
            error = 1;
            if (COV_recounter == recoverytime-1 && !COV_Comp((COV_th - COV_rec),CellVoltage))
            {
                COV_recounter = 0;
                COVL_DEC_counter = 0;
                COV_State = NORMAL;
				printf("\nRecovery: COV\n");
            }   
            else if(!COV_Comp((COV_th - COV_rec),CellVoltage))
            {
                COV_recounter ++;
            }
            break;
        default:
            break;
    }
    // for(int i=0;i<2;i++)
    // {
    //     *COV_flag = flag[i];
    //     COV_flag ++;
    // }
    *COV_alert = alert;
    *COV_error = error;

}

void CUV_protect
(
                const uint16_t *CellVoltage,     //input
                const uint16_t CUV_th,          //input
                const uint16_t CUV_delay,       //input
                const uint16_t CUV_rec,         //input
                const uint8_t recoverytime,     //input
                uint8_t *CUV_alert,             //output
                uint8_t *CUV_error              //output
)
{
    uint8_t alert,error;
    switch (CUV_State)
    {
        case NORMAL:   
            // clearFlags(CUV_ALERT,SafetyAlertA);
			// clearFlags(CUV_FAULT,SafetyStatusA);
            // if(CUV_DSG & *readSettings(DSGFETProtectionsA))//FET自主控制已开启
            //     clearFlags(XDSG,AlarmRawStatus);
            alert = 0;
            error = 0;
            if(CUV_Comp(CUV_th,CellVoltage))
            {
                CUV_counter ++;
                CUV_State = ALERT;
				printf("\nAlert: CUV\n");
            }
            break;
        case ALERT:
            // setFlags(CUV_ALERT,SafetyAlertA);	//Safety Alert A[CUV] = 1;
            alert = 1;
            if(CUV_counter == CUV_delay-1 && CUV_Comp(CUV_th,CellVoltage))
            {
                CUV_counter = 0;
                CUV_State = TRIP;
				printf("\nError: CUV\n");
            }    
            else if (!CUV_Comp(CUV_th,CellVoltage))
            {
                CUV_counter = 0;
                CUV_State = NORMAL;
            }
            else
                CUV_counter ++;
            break;
        case TRIP:
            // clearFlags(CUV_ALERT,SafetyAlertA);	//Safety Alert A[CUV] = 0;
            // setFlags(CUV_FAULT,SafetyStatusA);	//Safety Status A[CUV] = 1;
            // if(CUV_DSG & *readSettings(DSGFETProtectionsA))//FET自主控制已开启
            //     setFlags(XDSG,AlarmRawStatus);
            alert = 0;
            error = 1;
            if (CUV_recounter == recoverytime-1 && !CUV_Comp((CUV_th + CUV_rec),CellVoltage))
            {
                CUV_recounter = 0;
                CUV_State = NORMAL;
				printf("\nRecovery: CUV\n");
            }   
            else if(!CUV_Comp((CUV_th + CUV_rec),CellVoltage))
            {
                CUV_recounter ++;
            }
            break;
        // case RECOVERY:
        //     COV_Fault = 0;	//Safety Status A[COV] = 0;
        //     COV_State = NORMAL;
        //     COVL_DEC_counter = 0;
        //     break;
        default:
            break;
    }
    *CUV_alert = alert;
    *CUV_error = error;
}

void COVL_protect
(
                const uint8_t COVL_limit,           //input
                const uint8_t COVL_dec_delay,       //input
                const uint8_t COVL_recoverytime,    //input
                uint8_t *COVL_alert,                //output
                uint8_t *COVL_error                 //output
)
{
    uint8_t alert,error;
    switch (COVL_State)
    {
        case LATCH_RESET:   
            // clearFlags(COVL_ALERT,SafetyAlertC);		//Safety Alert C[COVL] = 0;
			// clearFlags(COVL_FAULT,SafetyStatusC);		//Safety Status C[COVL] = 0;
            // if(COVL_CHG & *readSettings(CHGFETProtectionsC))//FET自主控制已开启
            //     clearFlags(XCHG,AlarmRawStatus);
            alert = 0;
            error = 0;
            if(COVL_counter > 0)
            {
				COVL_State = LATCH_ALERT;
				printf("\nAlert: COV Latch\n");
			}
            break;
        case LATCH_ALERT:
            // setFlags(COVL_ALERT,SafetyAlertC);			//Safety Alert C[COVL] = 1;
            alert = 1;
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
            // clearFlags(COVL_ALERT,SafetyAlertC);		//Safety Alert C[COVL] = 0;
			// setFlags(COVL_FAULT,SafetyStatusC);			//Safety Status C[COVL] = 1;
            // if(COVL_CHG & *readSettings(CHGFETProtectionsC))//FET自主控制已开启
            //     setFlags(XCHG,AlarmRawStatus);
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
    *COVL_alert = alert;
    *COVL_error = error;
}
