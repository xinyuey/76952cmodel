/*OCD1、OCD2、OCD3、SCD、OCC、OCDL、SCDL*/

#include "common.h"

#define OCD1_ALERT BIT(5)   // Safety Alert A
#define OCD2_ALERT BIT(6)   // Safety Alert A
#define OCD3_ALERT BIT(7)   // Safety Alert C
#define SCD_ALERT  BIT(7)   // Safety Alert A
#define OCC_ALERT  BIT(4)   // Safety Alert A
#define OCDL_ALERT BIT(5)   // Safety Alert C
#define SCDL_ALERT BIT(6)   // Safety Alert C

#define OCD1_FAULT BIT(5)   // Safety Status A
#define OCD2_FAULT BIT(6)   // Safety Status A
#define OCD3_FAULT BIT(7)   // Safety Status C
#define SCD_FAULT  BIT(7)   // Safety Status A
#define OCC_FAULT  BIT(4)   // Safety Status A
#define OCDL_FAULT BIT(5)   // Safety Status C
#define SCDL_FAULT BIT(6)   // Safety Status C

extern  uint16_t current,charger,LD;

StateMachine OCD1_State = NORMAL;
StateMachine OCD2_State = NORMAL;
StateMachine OCD3_State = NORMAL;
StateMachine SCD_State = NORMAL;
StateMachine OCC_State = NORMAL;

StateMachine_Lacth OCDL_State = LATCH_RESET;
StateMachine_Lacth SCDL_State = LATCH_RESET;

uint8_t CC_Comp(uint16_t threshold)
{
    if(current >= threshold)
        return 1;
    else
        return 0;
}

uint8_t OCD1_protect()
{

}

uint8_t OCDL_protect()
{

}