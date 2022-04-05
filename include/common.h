#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "BQ769x2Header.h"

#define BIT(n) 1 << n

typedef unsigned char       uint8_t;    //1 byte
typedef unsigned short int  uint16_t;   //2 bytes
typedef unsigned int        uint32_t;   //4 bytes

typedef enum{
        NORMAL=1,
        ALERT,
        TRIP
        }StateMachine;

typedef enum{
        LATCH_RESET=1,
        LATCH_ALERT,
        LATCH_TRIP
        }StateMachine_Lacth;

#define SIZE_OF_DIRECT 50
#define SIZE_OF_SUB 78
#define SIZE_OF_SETTING 272
#define SIZE_OF_COMMAND 20

struct RAM_DIRECT{              //直接命令寄存器
        char name[SIZE_OF_COMMAND];
        uint8_t addr;           //0x00-0xff地址
        uint16_t data;          //16位数据
};

struct RAM_SUB{                 //间接命令寄存器
        uint16_t addr;          //0x0000-0xffff地址
        uint32_t data;          //32位数据      
};

struct DATA_MEMORY_SETTINGS{    //配置寄存器
        uint16_t addr;          //0x0000-0xffff地址
        uint32_t data;          //32位数据
};

struct MemoryManager{
        struct RAM_DIRECT DirectMemory[SIZE_OF_DIRECT];
        struct RAM_SUB SubMemory[SIZE_OF_SUB];
        struct DATA_MEMORY_SETTINGS Data_Memory_Settings[SIZE_OF_SETTING];
}mm;

int initMemory();
int find_DM_info(uint8_t addr);
int find_DM_info_name(char name[]);
void writeDirectMemory(uint16_t data, uint8_t addr);
uint16_t readDirectMemory(uint8_t addr);
uint16_t readDirectMemory_name(char name[]);
void setFlags(uint8_t data,uint8_t addr);
void clearFlags(uint8_t data,uint8_t addr);

// void writeSubMemory(uint32_t data, uint16_t addr);
// void writeData_Memory_Settings(uint32_t data, uint16_t addr);

// void readDirectMemory(uint8_t addr);
// void readSubMemory(uint16_t addr);
// void readData_Memory_Settings(uint16_t addr);


