#ifndef COMMON
#define COMMON

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "BQ769x2Header.h"

#define BIT(n) 1 << n

typedef unsigned char       uint8_t;    	//1 byte
typedef unsigned short int  uint16_t;   	//2 bytes
typedef short int           int16_t;   		//2 bytes
typedef unsigned int        uint32_t; 		//4 bytes
typedef int           		int32_t;   		//4 bytes

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

#define SIZE_OF_DIRECT 86					//直接命令
#define SIZE_OF_SETTING 272					//配置
#define SIZE_OF_DATASUB 32					//数据相关子命令
#define SIZE_OF_COMSUB 46					//仅命令子命令

#define SIZE_OF_COMMAND 30					//命令名字长度
#define SIZE_OF_DATA_BUFFER 32				//0x40-5f长度	
	
struct RAM_DIRECT{           	   			//直接命令寄存器
        char name[SIZE_OF_COMMAND];
        uint8_t addr;           			//0x00-0xff地址
        uint16_t data;          			//16位数据
};

struct RAM_DATASUB{                 		//间接命令寄存器
		char name[SIZE_OF_COMMAND];
		int data_length;
        uint16_t addr;          			//0x0000-0xffff地址
        uint8_t data[SIZE_OF_DATA_BUFFER];  //32字节数据
};

struct RAM_COMSUB{                 			//间接命令寄存器
		char name[SIZE_OF_COMMAND];
        uint16_t addr;          			//0x0000-0xffff地址 
};

struct DATA_MEMORY_SETTINGS{    			//配置寄存器
		char name[SIZE_OF_COMMAND];
		int data_length;
        uint16_t addr;          			//0x0000-0xffff地址
        uint8_t data[SIZE_OF_DATA_BUFFER];  //32字节数据
};

struct MemoryManager{
        struct RAM_DIRECT DirectMemory[SIZE_OF_DIRECT];
        struct RAM_DATASUB DataSubMemory[SIZE_OF_DATASUB];
		struct RAM_COMSUB ComSubMemory[SIZE_OF_COMSUB];
        struct DATA_MEMORY_SETTINGS Data_Memory_Settings[SIZE_OF_SETTING];
}mm;

/*下面是设备内部的寄存器访问逻辑*/

int initMemory();
int find_DM_info(uint8_t addr);

void writeDirectMemory(uint16_t data, uint8_t addr);
uint16_t readDirectMemory(uint8_t addr);
void setFlags(uint8_t data,uint8_t addr);
void clearFlags(uint8_t data,uint8_t addr);

int find_SM_info(uint16_t addr);
int find_SM_type(uint16_t addr);

void writeSubMemory(uint16_t addr,uint8_t offset,uint8_t *write_data,uint8_t write_len);
uint8_t* readSubMemory(uint16_t addr,uint8_t offset,uint8_t read_len);
void writeSettings(uint16_t addr,uint8_t *write_data,uint8_t write_len);
uint8_t* readSettings(uint16_t addr);


/*下面是外部子命令交互时的寄存器访问逻辑*/

int Reg_data_to_Buffer(int mem_info,int mem_type);
int Buffer_data_to_Reg(int mem_info,uint8_t data_length,int mem_type);

int ComSubCommand(int info);
void SubCommand_Sequence();
void Command_Sequence();


//向缓冲区连续读写指定字节数据
void writeBuffer(uint8_t write_addr,uint8_t *write_buff,uint8_t write_bytes);
void readBuffer(uint8_t read_addr,uint8_t read_bytes);

//外部读写直接命令寄存器
#define write(write_addr,write_data) writeDirectMemory(write_data,write_addr)

#define read(read_addr) readDirectMemory(read_addr)


#endif
