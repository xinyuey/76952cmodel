/* OCC OCD SCD */
#include "common.h"
 /*-------------相关寄存器配置--------------       7       6       5       4       3       2       1        0     */ 
// Settings:Protection:Protection Configuration                                                                    	
// Settings:Protection:Enabled Protections A	 SCD     OCD2    OCD1    OCC     COV     CUV     RSVD_0   RSVD_0	
// Settings:Protection:Enabled Protections B	 OTF     OTINT   OTD     OTC     RSVD_0  UTINT   UTD      UTC	
// Settings:Protection:Enabled Protections C	 OCD3    SCDL    OCDL    COVL    RSVD    PTO     HWDF     RSVD_0	
// Protections:OCC:Threshold                     8  bits                                                                                                 
// Protections:OCC:Delay                         8  bits                                                                 
// Protections:OCC:Recovery Threshold	         16 bits                                                                         
// Protections:OCC:PACK-TOS Delta                16 bits                                                                             
// Protections:OCD1:Threshold                    8  bits                                                              
// Protections:OCD1:Delay                        8  bits                                                          
// Protections:OCD2:Threshold                    8  bits                                                              
// Protections:OCD2:Delay                        8  bits                                                          
// Protections:SCD:Threshold                     8  bits                                                             
// Protections:SCD:Delay                         8  bits                                                         
// Protections:SCD:Recovery Time                 8  bits                                                                 
// Protections:OCD3:Threshold                    16 bits                                                              
// Protections:OCD3:Delay                        8  bits                                                          
// Protections:OCD:Recovery Threshold            16 bits                                                                      
// Protections:OCDL:Latch Limit                  8  bits                                                                
// Protections:OCDL:Counter Dec Delay            8  bits                                                                      
// Protections:OCDL:Recovery Time                8  bits                                                                  
// Protections:OCDL:Recovery Threshold           8  bits                                                                       
// Protections:SCDL:Latch Limit                  8  bits                                                                
// Protections:SCDL:Counter Dec Delay            8  bits               
// Protections:SCDL:Recovery Time                8  bits           
// Protections:SCDL:Recovery Threshold           16 bits
// Protections:Recovery:Time                     8  bits            
                            
//#define OCD1_ALERT BIT(5)                        //Safety Alert A
//#define OCD2_ALERT BIT(6)                        //Safety Alert A
//#define OCD3_ALERT BIT(7)                        //Safety Alert C
//#define  SCD_ALERT BIT(7)                        //Safety Alert A
//#define  OCC_ALERT BIT(4)                        //Safety Alert A
//#define OCDL_ALERT BIT(5)                        //Safety Alert C
//#define SCDL_ALERT BIT(6)                        //Safety Alert C
//
//#define OCD1_FAULT BIT(5)                        //Safety Status A
//#define OCD2_FAULT BIT(6)                        //Safety Status A
//#define OCD3_FAULT BIT(7)                        //Safety Status C
//#define  SCD_FAULT BIT(7)                        //Safety Status A
//#define  OCC_FAULT BIT(4)                        //Safety Status A
//#define OCDL_FAULT BIT(5)                        //Safety Status C
//#define SCDL_FAULT BIT(6)                        //Safety Status C

//extern uint16_t charger,LD;                      //0 or 1
//extern int16_t  current;                         //充电为正，放电为负，单位mv
//int16_t CC1_INPUT = -10;                         //CC1读数结果,用于OCD3判断

//uint16_t Protection_Configuration = 0x0002;
//uint16_t SCDL_CURR_RECOV =0x0000;                //Protection_Configuration & (1<<10)    是否使能SCDL 基于电流恢复
//uint16_t OCDL_CURR_RECOV =0x0000;                //Protection_Configuration & (1<<9)    是否使能OCDL 基于电流恢复

//uint8_t REC_DLY = 2;                             //对应 Protections:Recovery:Time恢复时间

//uint8_t SCD_TH = 100;                            //单位mv,原始寄存器为译码值
//uint8_t SCD_DLY = 2;                             //实际延时很小 us 级别
//uint8_t SCD_REC_DLY = 2;                         //实际 s 级别延时

//uint8_t SCDL_Limit = 3;                          
//uint8_t SCDL_DEC_DLY = 100;                      //实际 s 级别延时
//uint8_t SCDL_RecoveryTime = 150;                 //实际 s 级别延时 Settings:Protection:Protection Configuration[SCDL_CURR_RECOV] 有效时该恢复机制才会起作用
//int16_t SCDL_REC_TH = -10;                       //mV单位  原始寄存器单位为mA注意连接时单位

//uint8_t OCC_TH = 4;                              //mV
//uint8_t OCC_DLY = 2;                             //实际延时 ms 级别
//int16_t OCC_CREC_TH = 1;                         // mV单位  原始寄存器单位为mA注意连接时单位
//int16_t OCC_VREC_Delta = 200;                    // 10mV单位
//int16_t Stack_Pack_Delta = 10;                   // 10mV单位   Vstack - Vpack   从VADC获取结果

//uint8_t OCD1_TH = 100;                           //单位2mv
//uint8_t OCD1_DLY = 2;                            //实际延时 ms 级别
//uint8_t OCD2_TH = 200;                           //单位2mv
//uint8_t OCD2_DLY = 2;                            //实际延时 ms 级别
//uint8_t OCD3_TH = -100;                          //基于CC1读数结果,单位userA(可配置)
//uint8_t OCD3_DLY = 2;                            //实际延时 s 级别
//int16_t OCD_REC_TH = -10;                        //mV单位  原始寄存器单位为mA注意连接时单位
//
//uint8_t OCDL_Limit = 3;                          
//uint8_t OCDL_DEC_DLY = 100;                      //实际 s 级别延时
//uint8_t OCDL_RecoveryTime = 150;                 //实际 s 级别延时 Settings:Protection:Protection Configuration[OCDL_CURR_RECOV] 有效时该恢复机制才会起作用
//int16_t OCDL_REC_TH = -10;                       //mV单位  原始寄存器单位为mA注意连接时单位

StateMachine        SCD_State = NORMAL;
StateMachine        OCC_State = NORMAL;
StateMachine        OCD1_State = NORMAL;
StateMachine        OCD2_State = NORMAL;
StateMachine        OCD3_State = NORMAL;
StateMachine_Lacth  SCDL_State = LATCH_RESET;
StateMachine_Lacth  OCDL_State = LATCH_RESET;

uint8_t SCD_counter = 0;         
uint8_t SCD_recounter = 0; 
uint8_t SCDL_counter = 0;         
uint8_t SCDL_DEC_counter = 0; 
uint8_t SCDL_recounter = 0;

uint8_t OCD1_counter = 0;         
uint8_t OCD1_recounter = 0; 
uint8_t OCD2_counter = 0;         
uint8_t OCD2_recounter = 0;
uint8_t OCDL_counter = 0;         
uint8_t OCDL_DEC_counter = 0; 
uint8_t OCDL_recounter = 0;

uint8_t OCD3_counter = 0;         
uint8_t OCD3_recounter = 0;

uint8_t OCC_counter = 0;         
uint8_t OCC_CREC_recounter = 0;
uint8_t OCC_VREC_recounter = 0;

uint8_t OC_Comp(const int16_t current,int16_t threshold)
{
    if(current >= threshold)
        return 1;
    else
        return 0;
} 

void SCD_protect
(
				//input
				const int16_t current,
				const uint8_t SCD_th,
				const uint8_t SCD_delay,
				const uint8_t SCD_rec_delay,
				//output
				uint8_t *SCD_alert,
				uint8_t *SCD_error)

{
	uint8_t alert,error;
    switch (SCD_State)
    {
        case NORMAL:   
//            clearFlags(SCD_ALERT,SafetyAlertA);
//			clearFlags(SCD_FAULT,SafetyStatusA);
			alert = 0;
			error = 0;
            if(!OC_Comp(current,-(int16_t)SCD_th))
            {
                SCD_counter ++;
                SCD_State = ALERT;
				printf("\nAlert: SCD\n");
            }
            else if(SCDL_counter > 0)
                SCDL_DEC_counter ++;
            break;
        case ALERT:
//            setFlags(SCD_ALERT,SafetyAlertA);	//Safety Alert A[SCD] = 1;
			alert = 1;
			error = 0;
            if(SCD_counter == SCD_delay-1 && !OC_Comp(current,-(int16_t)SCD_th))
            {
                SCD_counter = 0;
                SCD_State = TRIP;
				printf("\nError: SCD\n");
                if(SCDL_State != LATCH_TRIP)    //SCDL触发后就不计数了
                    SCDL_counter ++;    //放在这是为识别SCD_Fault上升沿，即SCD_Fault上升沿计数值达到锁存阈值，会触发SCDL
            }    
            else if (OC_Comp(current,-(int16_t)SCD_th))
            {
                SCD_counter = 0;
                SCD_State = NORMAL;
            }
            else
                SCD_counter ++;
            break;
        case TRIP:
//			clearFlags(SCD_ALERT,SafetyAlertA);		//Safety Alert A[SCD] = 0;
//			setFlags(SCD_FAULT,SafetyStatusA);		//Safety Status A[SCD] = 1;
			alert = 0;
			error = 1;
            if (SCD_recounter == SCD_rec_delay-1 && OC_Comp(current,-(int16_t)SCD_th))
            {
                SCD_recounter = 0;
                SCDL_DEC_counter = 0;
                SCD_State = NORMAL;
				printf("\nRecovery: SCD\n");
            }   
            else if(OC_Comp(current,-(int16_t)SCD_th))
            {
                SCD_recounter ++;
            }
            else 
            {
                SCD_recounter = 0;
            }
            break;
        default:
            break;
    } 

	*SCD_alert = alert;
	*SCD_error = error;
}

void SCDL_protect
(
				//input
				const int16_t current,
				const uint16_t LD,
				const uint8_t SCDL_limit,
				const uint8_t SCDL_dec_delay,
				const uint8_t SCDL_recoverytime,
				const uint8_t SCDL_rec_th,
				const uint8_t SCDL_curr_reccov,
				//output
				uint8_t *SCDL_alert,
				uint8_t *SCDL_error
)
{
//    SCDL_CURR_RECOV = Protection_Configuration & (1<<10);
//    OCDL_CURR_RECOV = Protection_Configuration & (1<<9); 
	uint8_t alert,error;
    switch (SCDL_State)
    {
        case LATCH_RESET:   
//          clearFlags(SCDL_ALERT,SafetyAlertC);		//Safety Alert C[SCDL] = 0;
//			clearFlags(SCDL_FAULT,SafetyStatusC);		//Safety Status C[SCDL] = 0;
			alert = 0;
			error = 0;
            if(SCDL_counter > 0)
            {
				SCDL_State = LATCH_ALERT;
				printf("\nAlert: SCD Latch\n");
			}
            break;
        case LATCH_ALERT:
//            setFlags(SCDL_ALERT,SafetyAlertC);			//Safety Alert C[SCDL] = 1;
            alert = 1;
			error = 0;
            if(SCDL_counter == SCDL_limit)
            {
                SCDL_counter = 0;
                SCDL_State = LATCH_TRIP;
				printf("\nError: SCD Latch\n");
            }
            else if(SCDL_counter == 0)
            {
                SCDL_State = LATCH_RESET;
            }
            else if(SCDL_DEC_counter >= SCDL_dec_delay) 
            {   
                SCDL_counter --;
            }
            break;
        case LATCH_TRIP:
//            clearFlags(SCDL_ALERT,SafetyAlertC);		//Safety Alert  C[SCDL] = 0;
//			setFlags(SCDL_FAULT,SafetyStatusC);			//Safety Status C[SCDL] = 1;
			alert = 0;
			error = 1;
            if(!LD)
            {
                SCDL_recounter = 0;
                SCDL_State = LATCH_RESET;
				printf("\nRecovery: SCD Latch from LD detect\n");
            }
            else if(SCDL_curr_reccov && (SCDL_recounter == SCDL_recoverytime-1) && OC_Comp(current,(int16_t)SCDL_rec_th))
            {
                SCDL_recounter = 0;
                SCDL_State = LATCH_RESET;
				printf("\nRecovery: SCD Latch from Current\n");
            } 
            //else if (基于主机子命令恢复)

            else if(SCDL_curr_reccov && OC_Comp(current,(int16_t)SCDL_rec_th))
            {
                SCDL_recounter ++;
            }  
            else
            {
                SCDL_recounter = 0;
            }
            break;
        default:
            break;
    }

	*SCDL_alert = alert;
	*SCDL_error = error;
}

void OCD1_protect
(
				//input
				const int16_t current,
				const uint8_t OCD1_th,
				const uint8_t OCD1_delay,
				const int16_t OCD_rec_th,
				const uint8_t recoverytime,
				//output
				uint8_t *OCD1_alert,
				uint8_t *OCD1_error
)
{
	uint8_t alert,error;
    switch (OCD1_State)
    {
        case NORMAL:   
//            clearFlags(OCD1_ALERT,SafetyAlertA);
//			clearFlags(OCD1_FAULT,SafetyStatusA);
			alert = 0;
			error = 0;
            if (!OC_Comp(current,-(int16_t)OCD1_th))
            {
                OCD1_counter++;
                OCD1_State = ALERT;
                printf("\nAlert: OCD1\n");
            }
            else if(OCDL_counter > 0)
                OCDL_DEC_counter ++;
            break;
        case ALERT:
//            setFlags(OCD1_ALERT, SafetyAlertA); // Safety Alert A[OCD1] = 1;
			alert = 1;
			error = 0;
            if (OCD1_counter == OCD1_delay - 1 && !OC_Comp(current,-(int16_t)OCD1_th))
            {
                OCD1_counter = 0;
                OCD1_State = TRIP;
                printf("\nError: OCD1\n");
                if (OCDL_State != LATCH_TRIP) // OCDL触发后就不计数了
                    OCDL_counter++;           //放在这是为识别OCD_Fault上升沿，即OCD_Fault上升沿计数值达到锁存阈值，会触发OCDL
            }
            else if (OC_Comp(current,-(int16_t)OCD1_th))
            {
                OCD1_counter = 0;
                OCD1_State = NORMAL;
            }
            else
                OCD1_counter++;
            break;
        case TRIP:
//            clearFlags(OCD1_ALERT, SafetyAlertA); // Safety Alert A[OCD1] = 0;
//            setFlags(OCD1_FAULT, SafetyStatusA);  // Safety Status A[OCD1] = 1;
			alert = 0;
			error = 1;
            if (OCD1_recounter == recoverytime - 1 && OC_Comp(current,-(int16_t)OCD_rec_th))
            {
                OCD1_recounter = 0;
                OCDL_DEC_counter = 0;
                OCD1_State = NORMAL;
				printf("\nRecovery: OCD1\n");
            }   
            else if(OC_Comp(current,-(int16_t)OCD_rec_th))
            {
                OCD1_recounter ++;
            }
            else
            {
                OCD1_recounter = 0;
            }
            break;
        default:
            break;
    }

	*OCD1_alert = alert;
	*OCD1_error = error;
}

void OCD2_protect
(
				//input
				const int16_t current,
				const uint8_t OCD2_th,
				const uint8_t OCD2_delay,
				const int16_t OCD_rec_th,
				const uint8_t recoverytime,
				//output
				uint8_t *OCD2_alert,
				uint8_t *OCD2_error
)
{
	uint8_t alert,error;
    switch (OCD2_State)
    {
        case NORMAL:   
//            clearFlags(OCD2_ALERT,SafetyAlertA);
//			clearFlags(OCD2_FAULT,SafetyStatusA);
			alert = 0;
			error = 0;
            if (!OC_Comp(current,-(int16_t)OCD2_th))
            {
                OCD2_counter++;
                OCD2_State = ALERT;
                printf("\nAlert: OCD2\n");
            }
            else if(OCDL_counter > 0)
                OCDL_DEC_counter ++;
            break;
        case ALERT:
//            setFlags(OCD2_ALERT, SafetyAlertA); // Safety Alert A[OCD2] = 1;
			alert = 1;
			error = 0;
            if (OCD2_counter == OCD2_delay - 1 && !OC_Comp(current,-(int16_t)OCD2_th))
            {
                OCD2_counter = 0;
                OCD2_State = TRIP;
                printf("\nError: OCD2\n");
                if (OCDL_State != LATCH_TRIP) // OCDL触发后就不计数了
                    OCDL_counter++;           //放在这是为识别OCD_Fault上升沿，即OCD_Fault上升沿计数值达到锁存阈值，会触发OCDL
            }
            else if (OC_Comp(current,-(int16_t)OCD2_th))
            {
                OCD2_counter = 0;
                OCD2_State = NORMAL;
            }
            else
                OCD2_counter++;
            break;
        case TRIP:
//            clearFlags(OCD2_ALERT, SafetyAlertA); // Safety Alert A[OCD2] = 0;
//            setFlags(OCD2_FAULT, SafetyStatusA);  // Safety Status A[OCD2] = 1;
			alert = 0;
			error = 1;
            if (OCD2_recounter == recoverytime - 1 && OC_Comp(current,-(int16_t)OCD_rec_th))
            {
                OCD2_recounter = 0;
                OCDL_DEC_counter = 0;
                OCD2_State = NORMAL;
				printf("\nRecovery: OCD2\n");
            }   
            else if(OC_Comp(current,-(int16_t)OCD_rec_th))
            {
                OCD2_recounter ++;
            }
            else
            {
                OCD2_recounter = 0;
            }
            break;
        default:
            break;
    }

	*OCD2_alert = alert;
	*OCD2_error = error;
}

void OCD3_protect
(
				//input
				const int16_t CC1_current,
				const int16_t OCD3_th,
				const uint8_t OCD3_delay,
				const int16_t OCD_rec_th,
				const uint8_t recoverytime,
				//output
				uint8_t *OCD3_alert,
				uint8_t *OCD3_error	
)
{
	uint8_t alert,error;
    switch (OCD3_State)
    {
        case NORMAL:   
//            clearFlags(OCD3_ALERT,SafetyAlertA);
//			clearFlags(OCD3_FAULT,SafetyStatusA);
			alert = 0;
			error = 0;
            if (!OC_Comp(CC1_current,OCD3_th))
            {
                OCD3_counter++;
                OCD3_State = ALERT;
                printf("\nAlert: OCD3\n");
            }
            else if(OCDL_counter > 0)
                OCDL_DEC_counter ++;
            break;
        case ALERT:
//            setFlags(OCD3_ALERT, SafetyAlertA); // Safety Alert A[OCD3] = 1;
			alert = 1;
			error = 0;
            if (OCD3_counter == OCD3_delay - 1 && !OC_Comp(CC1_current,OCD3_th))
            {
                OCD3_counter = 0;
                OCD3_State = TRIP;
                printf("\nError: OCD3\n");
                if (OCDL_State != LATCH_TRIP) // OCDL触发后就不计数了
                    OCDL_counter++;           //放在这是为识别OCD_Fault上升沿，即OCD_Fault上升沿计数值达到锁存阈值，会触发OCDL
            }
            else if (OC_Comp(CC1_current,OCD3_th))
            {
                OCD3_counter = 0;
                OCD3_State = NORMAL;
            }
            else
                OCD3_counter++;
            break;
        case TRIP:
//            clearFlags(OCD3_ALERT, SafetyAlertA); // Safety Alert A[OCD3] = 0;
//            setFlags(OCD3_FAULT, SafetyStatusA);  // Safety Status A[OCD3] = 1;
			alert = 0;
			error = 1;
            if (OCD3_recounter == recoverytime - 1 && OC_Comp(CC1_current,OCD_rec_th))
            {
                OCD3_recounter = 0;
                OCDL_DEC_counter = 0;
                OCD3_State = NORMAL;
				printf("\nRecovery: OCD3\n");
            }   
            else if(OC_Comp(CC1_current,OCD_rec_th))
            {
                OCD3_recounter ++;
            }
            else
            {
                OCD3_recounter = 0;
            }
            break;
        default:
            break;
    }

	*OCD3_alert = alert;
	*OCD3_error = error;
}

void OCDL_protect
(
					//input
					const int16_t current,
					const uint16_t LD,
					const uint8_t OCDL_limit,                         
					const uint8_t OCDL_dec_delay,                     //实际 s 级别延时
					const uint8_t OCDL_recoverytime,                 //实际 s 级别延时 Settings:Protection:Protection Configuration[OCDL_CURR_RECOV] 有效时该恢复机制才会起作用
					const int16_t OCDL_rec_th,
					const uint16_t OCDL_curr_recov,
					//output
					uint8_t *OCDL_alert,
					uint8_t *OCDL_error
					
)
{
	uint8_t alert,error;
    switch (OCDL_State)
    {
        case LATCH_RESET:   
//            clearFlags(OCDL_ALERT,SafetyAlertC);		//Safety Alert C[OCDL] = 0;
//			clearFlags(OCDL_FAULT,SafetyStatusC);		//Safety Status C[OCDL] = 0;
			alert = 0;
			error = 0;
            if(OCDL_counter > 0)
            {
				OCDL_State = LATCH_ALERT;
				printf("\nAlert: OCD Latch\n");
			}
            break;
        case LATCH_ALERT:
//            setFlags(OCDL_ALERT,SafetyAlertC);			//Safety Alert C[OCDL] = 1;
			alert = 1;
			error = 0;
            if(OCDL_counter == OCDL_limit)
            {
                OCDL_counter = 0;
                OCDL_State = LATCH_TRIP;
				printf("\nError: OCD Latch\n");
            }
            else if(OCDL_counter == 0)
            {
                OCDL_State = LATCH_RESET;
            }
            else if(OCDL_DEC_counter >= OCDL_dec_delay) 
            {   
                OCDL_counter --;
            }
            break;
        case LATCH_TRIP:
//            clearFlags(OCDL_ALERT,SafetyAlertC);		//Safety Alert  C[OCDL] = 0;
//			setFlags(OCDL_FAULT,SafetyStatusC);			//Safety Status C[OCDL] = 1;
			alert = 0;
			error = 1;
            if(!LD)
            {
                OCDL_recounter = 0;
                OCDL_State = LATCH_RESET;
				printf("\nRecovery: OCD Latch from LD detect\n");
            }
            else if(OCDL_curr_recov && (OCDL_recounter == OCDL_recoverytime-1) && OC_Comp(current,(int16_t)OCDL_rec_th))
            {
                OCDL_recounter = 0;
                OCDL_State = LATCH_RESET;
				printf("\nRecovery: OCD Latch from Current\n");
            } 
            //else if (基于主机子命令恢复)

            else if(OCDL_curr_recov && OC_Comp(current,(int16_t)OCDL_rec_th))
            {
                OCDL_recounter ++;
            }  
            else
            {
                OCDL_recounter = 0;
            }
            break;
        default:
            break;
    }

	*OCDL_alert = alert;
	*OCDL_error = error;
}

void OCC_protect
(
				//input
				const int16_t current,
				const uint8_t OCC_th,
				const uint8_t OCC_delay,
				const int16_t OCC_crec_th,
				const int16_t stack_pack_delta,
				const int16_t OCC_vrec_delta,
				const uint8_t recoverytime,
				//output
				uint8_t *OCC_alert,
				uint8_t *OCC_error
)
{
	uint8_t alert,error;
    switch (OCC_State)
    {
        case NORMAL:   
//            clearFlags(OCC_ALERT,SafetyAlertA);
//			clearFlags(OCC_FAULT,SafetyStatusA);
			alert = 0;
			error = 0;
            if(OC_Comp(current,(int16_t)OCC_th))
            {
                OCC_counter ++;
                OCC_State = ALERT;
				printf("\nAlert: OCC\n");
            }

            break;
        case ALERT:
//            setFlags(OCC_ALERT,SafetyAlertA);	//Safety Alert A[SCD] = 1;
			alert = 1;
			error = 0;
            if(OCC_counter == OCC_delay-1 && OC_Comp(current,(int16_t)OCC_th))
            {
                OCC_counter = 0;
                OCC_State = TRIP;
				printf("\nError: OCC\n");
            }    
            else if (!OC_Comp(current,(int16_t)OCC_th))
            {
                OCC_counter = 0;
                OCC_State = NORMAL;
            }
            else
                OCC_counter ++;
            break;
        case TRIP:
//			clearFlags(OCC_ALERT,SafetyAlertA);		//Safety Alert A[OCC] = 0;
//			setFlags(OCC_FAULT,SafetyStatusA);		//Safety Status A[OCC] = 1;
			alert = 0;
 			error = 1;
            if (OCC_CREC_recounter == recoverytime-1 && !OC_Comp(current,(int16_t)OCC_crec_th))           //基于电流恢复
            {
                OCC_CREC_recounter = 0;
                OCC_State = NORMAL;
				printf("\nRecovery: OCC from current \n");
                break;
            }
            else if(!OC_Comp(current,(int16_t)OCC_crec_th))
            {
                OCC_CREC_recounter ++;
            }
            else
            {
                OCC_CREC_recounter = 0;
            }

            if(OCC_VREC_recounter == recoverytime-1 && (stack_pack_delta >= OCC_vrec_delta) )   //基于pack-stack电压恢复
            {
                OCC_VREC_recounter = 0;
                OCC_State = NORMAL;
				printf("\nRecovery: OCC from Stack-Pack voltage\n");
                break;
            }
            else if(stack_pack_delta >= OCC_vrec_delta)
            {
                OCC_VREC_recounter ++;
            }
            else
            {
                OCC_VREC_recounter = 0;
            }    
            break;
        default:
            break;
    } 

	*OCC_alert = alert;
	*OCC_error = error;
}
