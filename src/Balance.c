// 相关寄存器：
// Direct Command：
// Alarm Status Register [CB]
// Alarm Raw Status Register [CB]
// Alarm Enable Register [CB]

// SUBCOMMAND WITH DATA:  
// CB_ACTIVE_CELLS 0x0083
// CB_SET_LVL
// CBSTATUS1
// CBSTATUS2
// CBSTATUS3

// DATA Memory Settings ：
// 互联单元配置
// Settings:Cell Balancing Config:Balancing Configuration
// Settings:Cell Balancing Config:Min Cell Temp
// Settings:Cell Balancing Config:Max Cell Temp
// Settings:Cell Balancing Config:Max Internal Temp
// Settings:Cell Balancing Config:Cell Balance Interval
// Settings:Cell Balancing Config:Cell Balance Max Cells
// Settings:Cell Balancing Config:Cell Balance Min Cell V (Charge)
// Settings:Cell Balancing Config:Cell Balance Min Delta (Charge)
// Settings:Cell Balancing Config:Cell Balance Stop Delta (Charge)
// Settings:Cell Balancing Config:Cell Balance Min Cell V (Relax)          
// Settings:Cell Balancing Config:Cell Balance Min Delta (Relax)
// Settings:Cell Balancing Config:Cell Balance Stop Delta (Relax)
// Settings:Protections:Enabled Protections A occurs except for a COV fault.

//外部相关变量：工作模式、保护警告寄存器A、保护状态寄存器A（COV除外）、永久错误状态、充放电状态、主机命令

//主状态机：空闲模式、均衡检测、开启均衡、禁止均衡
//函数：均衡判断函数、获取电压最大值、获取电压最小值、开启均衡检测

//----------------------------------------------------------------------------------------------------------------//
//------------------------------------------外部环境信息与工作状态信息输入-------------------------------------------//
//----------------------------------------------------------------------------------------------------------------//


// extern uint16_t CellVoltage[];
// extern int16_t  current;                        //充电为正，放电为负，单位mv
// extern mode work_mode; 芯片工作状态
// uint8_t Temp = 20;                              //环境温度
// uint8_t Safety_AlertA  = 0x00;
// uint8_t Safety_StatusA = 0x00;
// 
// uint16_t CB_ACTIVE_CELLS_bal = 0x0000;              //主机写入的均衡单元    该地址写和读寄存器应当分开
// uint16_t CB_SET_LVL_bal      = 0x0000;              //主机写入的均衡电平    该地址写和读寄存器应当分开



//----------------------------------------------------------------------------------------------------------------//
//----------------------------------------------均衡检测相关信息输出------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------//

// uint16_t BLANCE_TIME[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //均衡活动时间寄存
// uint16_t BALANCE_CELL = 0x0000;            // 均衡开关寄存器
//#define  CB_Alarm BIT(5)                 //Alarm Raw Status


//----------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------寄存器配置信息-------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------//

// uint16_t Cell_active  = 0xffff;
// uint8_t Balancing_Config = 0x00;         // Settings:Cell Balancing Config:Balancing Configuration
// int16_t CHG_th = 10;                     // 判定充电阈值  单位 mv
// int16_t DSG_th = -10;                    // 判定放电阈值 单位 mv
// int16_t Min_Cell_Temp = -20;             // 最小环境温度  单位 摄氏度      Settings:Cell Balancing Config:Min Cell Temp
// int16_t Max_Cell_Temp = 70;              // 判定放电阈值  单位 摄氏度      Settings:Cell Balancing Config:Max Cell Temp
// int16_t CHG_MINV = 3600;                 // 充电 最小电压开始均衡标志      Settings:Cell Balancing Config:Cell Balance Min Cell V (Charge)
// int16_t CHG_delta = 200;                 // 充电 开始均衡差值             Settings:Cell Balancing Config:Cell Balance Min Delta (Charge)
// int16_t CHG_stop_delta = 150;            // 充电 结束均衡差值             Settings:Cell Balancing Config:Cell Balance Stop Delta (Charge)
// int16_t REX_MINV = 3600;                 // 静态 最小电压开始均衡标志      Settings:Cell Balancing Config:Cell Balance Min Cell V (Relax)
// int16_t REX_delta = 200;                 // 静态 开始均衡差值             Settings:Cell Balancing Config:Cell Balance Min Delta (Relax)
// int16_t REX_stop_delta = 150;            // 静态 结束均衡差值             Settings:Cell Balancing Config:Cell Balance Stop Delta (Relax)
// uint8_t Balance_Interval = 100;           // 均衡间隔                     Settings:Cell Balancing Config:Cell Balance Interval
// uint8_t Max_Cells = 3;                   // 最大均衡节数

//-----------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------模块内部信号与函数---------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------//

#include "common.h"

typedef enum{
        IDLE=1,
        CHG_CHECK,
        CHG_BALANCE,
        REX_CHECK,
        REX_BALANCE,
        HOST_BALANCE,
        DISABLE
        }Balance_StateMachine;

Balance_StateMachine Bal_state = IDLE;


uint16_t VC_min(uint16_t *CellVoltage,const uint16_t Cell_active)
{
    uint16_t min = *CellVoltage;
    //printf("\n*****  cell_1 is %d   *****\n",min);
    uint16_t *Cell_V;
    Cell_V = CellVoltage;
    for (int i = 0; i < 16; i++)
    {
        if (Cell_active & (1 << i))
        {
            if (*Cell_V < min)
            {
                min = *Cell_V;
            }
        }
        Cell_V += 1;
    }
    return min;
}
uint16_t VC_max(uint16_t *CellVoltage,const uint16_t Cell_active)
{
    uint16_t max = *CellVoltage;
    uint16_t *Cell_V;
    Cell_V = CellVoltage;
    for (int i = 0; i < 16; i++)
    {
        if (Cell_active & (1 << i))
        {
            if (*Cell_V > max)
            {
                max = *Cell_V;
            }
            
        }
        Cell_V += 1;
    }
    return max;
}

uint16_t balance_AUTO(uint16_t *CellVoltage,int16_t TH)
{
    uint16_t balance_cell = 0x0000;
    for (int i = 0; i < 16; i++)
    {
        if (*CellVoltage > TH)
        {
            balance_cell |= (uint16_t)(1 << i);
        }
        else
        {
            balance_cell &= ~(uint16_t)(1 << i);
        }
        CellVoltage += 1;
    }
    return balance_cell;
}

uint16_t balance_HOST(uint16_t *CellVoltage,const uint16_t CB_ACTIVE_CELLS_bal,const uint16_t CB_SET_LVL_bal)
{
    uint16_t temp = 0x0000;
    if(CB_ACTIVE_CELLS_bal)
    {
        return CB_ACTIVE_CELLS_bal;
    }
    else if(CB_SET_LVL_bal)
    {
        temp = balance_AUTO(CellVoltage,CB_SET_LVL_bal);
    }
    return temp;
}

uint16_t Balance_adjacent( const uint16_t ACTIVE_CELL)
{
    uint16_t ODD_MASK = 0x0000;  //奇数均衡掩码
    uint16_t EVEN_MASK = 0x0000; //偶数均衡掩码
    uint16_t mask_counter = 0;
    for (int i = 0 ; i < 16; i++)
    {
        if (ACTIVE_CELL & (1<<i))
        {
            if (mask_counter % 2 == 0)
            {
                EVEN_MASK |= 1<<i;
            }
            else
            {
                ODD_MASK |= 1<<i;
            }
            mask_counter++;
        }
    }
    return ODD_MASK;
}

void Balance_window(uint16_t* ODD_WINDOW_MASK,uint16_t* EVEN_WINDOW_MASK,uint16_t BALANCE_V,uint8_t Max_Cells,uint16_t ODD_MASK,uint16_t Cell_active)
{
    uint16_t EVEN_MASK = (~ODD_MASK) & Cell_active;          //窗口节数均衡掩码
    int cell_number = 0;
    int window_width = 0;
    static uint8_t ODD_window = 0;                   //奇數單元均衡窗口记录（均衡窗口滑动最后一个单元）
    static uint8_t EVEN_window = 0;                  //偶數單元均衡窗口记录（均衡窗口滑动最后一个单元）
    
    //printf("\n*****  window_width is %d %02d %02d  *****\n",window_width,ODD_window,EVEN_window);
    for(int i = 0 ; i < 16; i++)
    {
        if (Cell_active & (1 << i))
        {
            cell_number ++;
        }
    }
    //printf("\n*****  cell_number is %d   *****\n",cell_number);
    if (Max_Cells >= cell_number/2)
    {
        *ODD_WINDOW_MASK = ODD_MASK;
        *EVEN_WINDOW_MASK = EVEN_MASK;
        //printf("\n\n\n*****  error!  *****\n\n\n");
    }
    else
    {
        *ODD_WINDOW_MASK = 0;

        for (;window_width < Max_Cells;)
        {
            if (BALANCE_V & (1 << (ODD_window & 0x0f)) & ODD_MASK)
            {
                *ODD_WINDOW_MASK |=1 << ODD_window;
                window_width++;
            }
            if(ODD_window == 32)
            {
                ODD_window =0;
                break;
            }
            else if (window_width != 0 &&  ODD_window >= 16)
            {
                ODD_window =0;
            }
            else
            {
                ODD_window += 1;
            }
            //printf("\n*****  window_width is %d %04x %02d  *****\n",window_width,ODD_MASK,ODD_window);
        }
        
        window_width = 0;
        *EVEN_WINDOW_MASK = 0;
        for (;window_width < Max_Cells;)
        {
            if (BALANCE_V & (1 << (EVEN_window & 0x0f)) & EVEN_MASK)
            {
                *EVEN_WINDOW_MASK |=1 << EVEN_window;
                window_width++;
            }
            if(EVEN_window ==32)
            {
                EVEN_window =0;
                break;
            }
            else if (window_width != 0 &&  EVEN_window >= 16)
            {
                EVEN_window =0;
            }
            else
            {
                EVEN_window += 1;
            }
            //printf("\n*****  window_width is %d %04x %02d  *****\n",window_width,EVEN_MASK,EVEN_window);
        }
        
    }
    return ;
}

uint16_t balance_time(uint16_t *BLANCE_TIME,uint16_t cb)
{
    static uint16_t balance_time = 0x0000;
    for (int i = 0; i < 16; i++)
    {
        if (cb & (1<<i))
        {
            *BLANCE_TIME += 1;
        }
        BLANCE_TIME +=1;
    }
    if(cb){
        balance_time +=1;
    }
    return balance_time;
}

uint16_t Balance_V_check(
    uint16_t *CellVoltage,
    const int16_t current,
    const uint8_t Temp,    
    const uint8_t Safety_AlertA,
    const uint8_t Safety_StatusA,
    const uint16_t CB_ACTIVE_CELLS_bal,
    const uint16_t CB_SET_LVL_bal,
    const uint16_t Cell_active,
    const uint8_t Balancing_Config,
    const int16_t CHG_th,   
    const int16_t DSG_th,
    const int16_t Min_Cell_Temp,
    const int16_t Max_Cell_Temp, 
    const int16_t CHG_MINV,
    const int16_t CHG_delta,
    const int16_t CHG_stop_delta,
    const int16_t REX_MINV,
    const int16_t REX_delta,
    const int16_t REX_stop_delta,
    const uint8_t Balance_Interval)
{
    static uint8_t timer = 0;                       //均衡间隔计时器

    uint16_t BALANCE_V;
    uint8_t REX = (current > DSG_th) && (current < CHG_th);             //当作逻辑变量使用
    uint8_t CHG = current >= CHG_th;                                    //当作逻辑变量使用
    uint8_t DSG = current <= DSG_th;                                    //当作逻辑变量使用
    uint8_t Temp_en = (Temp > Min_Cell_Temp) && (Temp < Max_Cell_Temp); //当作逻辑变量使用

    uint8_t BALANCE_interupt = !Temp_en || DSG || CB_ACTIVE_CELLS_bal || CB_SET_LVL_bal || Safety_AlertA || Safety_StatusA;    //均衡中断信号产生
    switch (Bal_state)
    {
        case IDLE : 
            if(BALANCE_interupt) Bal_state = DISABLE;
            else if(CHG) Bal_state = CHG_CHECK;
            else if(REX) Bal_state = REX_CHECK;
            else Bal_state = IDLE;
            break;
        case CHG_CHECK :
            if(BALANCE_interupt) Bal_state = DISABLE;
            else if (timer == 0) Bal_state = IDLE;
            else if ( VC_min(CellVoltage,Cell_active) > CHG_MINV && (VC_max(CellVoltage,Cell_active) - VC_min(CellVoltage,Cell_active)) > CHG_delta) Bal_state = CHG_BALANCE;
            else Bal_state = CHG_CHECK;  
            break;
        case CHG_BALANCE :
            if(BALANCE_interupt) Bal_state = DISABLE;
            else if (timer == 0) Bal_state = IDLE;
            else if ( VC_min(CellVoltage,Cell_active) > CHG_MINV && (VC_max(CellVoltage,Cell_active) - VC_min(CellVoltage,Cell_active)) <= CHG_stop_delta) Bal_state = CHG_CHECK;
            else Bal_state = CHG_BALANCE;
            break;
        case REX_CHECK :
            if(BALANCE_interupt) Bal_state = DISABLE;
            else if (timer == 0) Bal_state = IDLE;
            else if ( VC_min(CellVoltage,Cell_active) > REX_MINV && (VC_max(CellVoltage,Cell_active) - VC_min(CellVoltage,Cell_active)) > REX_delta) Bal_state = REX_BALANCE;
            else Bal_state = REX_CHECK;
            break;
        case REX_BALANCE :
            if(BALANCE_interupt) Bal_state = DISABLE;
            else if (timer == 0) Bal_state = IDLE;
            else if ( VC_min(CellVoltage,Cell_active) > REX_MINV && (VC_max(CellVoltage,Cell_active) - VC_min(CellVoltage,Cell_active)) <= REX_stop_delta) Bal_state = REX_CHECK;
            else Bal_state = REX_BALANCE;
            break;
        case HOST_BALANCE :
            if(BALANCE_interupt) Bal_state = DISABLE;
            else if (timer == 0) Bal_state = IDLE;
            else Bal_state = HOST_BALANCE;
            break;
        case DISABLE : 
            if (CB_ACTIVE_CELLS_bal || CB_SET_LVL_bal) Bal_state = HOST_BALANCE;
            else if(BALANCE_interupt) Bal_state = DISABLE;            
            else Bal_state = IDLE;
            break;
        default : Bal_state = IDLE;
    }

    switch (Bal_state)
        {
            case IDLE : 
                BALANCE_V =0x00;
                break;
            case CHG_CHECK :
                BALANCE_V =0x00;
                break;
            case CHG_BALANCE :
                BALANCE_V =balance_AUTO(CellVoltage,(VC_min(CellVoltage,Cell_active)+CHG_stop_delta));
                break;
            case REX_CHECK :
                BALANCE_V =0x00;
                break;
            case REX_BALANCE :
                BALANCE_V =balance_AUTO(CellVoltage,(VC_min(CellVoltage,Cell_active)+REX_stop_delta));
                break;
            case HOST_BALANCE :
                BALANCE_V =balance_HOST(CellVoltage,CB_ACTIVE_CELLS_bal,CB_SET_LVL_bal);
                break;
            case DISABLE : 
                BALANCE_V =0x0000;
                break;
            default : BALANCE_V =0x0000;
        }
    if(timer == 0){
        timer = Balance_Interval; 
    }
    else if(Bal_state != IDLE){
        timer -= 1;
    }
    //printf("\n*****  timer is %04x  *****\n",timer);
    return BALANCE_V;
}


//----------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------主要功能函数-----------------------------------------------//
//----------------------------------------------------------------------------------------------------------------//

void Balance_timing(
    uint16_t *CellVoltage,
    const int16_t current,
    // extern mode work_mode; 芯片工作状态
    const uint8_t Temp,    
    const uint8_t Safety_AlertA,
    const uint8_t Safety_StatusA,
    const uint16_t CB_ACTIVE_CELLS_bal,
    const uint16_t CB_SET_LVL_bal,
    const uint16_t Cell_active,
    const uint8_t Balancing_Config,
    const int16_t CHG_th,   
    const int16_t DSG_th,
    const int16_t Min_Cell_Temp,
    const int16_t Max_Cell_Temp, 
    const int16_t CHG_MINV,
    const int16_t CHG_delta,
    const int16_t CHG_stop_delta,
    const int16_t REX_MINV,
    const int16_t REX_delta,
    const int16_t REX_stop_delta,
    const uint8_t Balance_Interval,
    const uint8_t Max_Cells,

    //output
    uint16_t *BLANCE_TIME,//单元均衡时间数组输出
    uint16_t *BALANCE_CELL)
{
    const  uint8_t adjacent_TH = 5;                 //奇偶单元均衡计时阈值
    static uint8_t adjacent_timer = 0; 

    static uint16_t sum_time = 0;
    static uint16_t BALANCE_ODD;                    //奇數单元窗口检测
    static uint16_t BALANCE_EVEN;                   //偶數單元窗口檢測
    static uint16_t BALANCE_WINDOW;
    static uint16_t BALANCE_adjacent;
    
    static uint16_t BALANCE_V_check;                //基于电压的均衡单元检测

    BALANCE_V_check = Balance_V_check(
                                        CellVoltage,
                                        current,
                                        Temp,    
                                        Safety_AlertA,
                                        Safety_StatusA,
                                        CB_ACTIVE_CELLS_bal,
                                        CB_SET_LVL_bal,
                                        Cell_active,
                                        Balancing_Config,
                                        CHG_th,   
                                        DSG_th,
                                        Min_Cell_Temp,
                                        Max_Cell_Temp, 
                                        CHG_MINV,
                                        CHG_delta,
                                        CHG_stop_delta,
                                        REX_MINV,
                                        REX_delta,
                                        REX_stop_delta,
                                        Balance_Interval);

    //printf("\n*****  BALANCE_V_check is %04x  *****\n",BALANCE_V_check);

    if(adjacent_timer == 0)
    {
        BALANCE_adjacent = Balance_adjacent(Cell_active);
        Balance_window(&BALANCE_ODD,&BALANCE_EVEN,BALANCE_V_check, Max_Cells,BALANCE_adjacent,Cell_active);   
    }

    if (adjacent_timer < adjacent_TH)
    {
        BALANCE_WINDOW = BALANCE_ODD;
    }
    else if (adjacent_timer < 2 * adjacent_TH)
    {
        BALANCE_WINDOW = BALANCE_EVEN;
    }

    //printf("\n*****  Balance_WINDOW is %04x  *****\n",BALANCE_WINDOW);

    if (BALANCE_V_check)
    {
        adjacent_timer += 1;
    }

    if (adjacent_timer == 2 * adjacent_TH)
    {
        adjacent_timer = 0;
    }

    *BALANCE_CELL = BALANCE_WINDOW & BALANCE_V_check;

    sum_time = balance_time(BLANCE_TIME,*BALANCE_CELL);

    return;
}
