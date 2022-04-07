/* Memory */

#include "common.h"


//内存初始化
int initMemory()
{
    //读取外部配置文件，初始化所有寄存器
    FILE *fp;
    fp = fopen("../sim/InitMemory.txt","r");
    if(fp==NULL)
    {  
		printf("File cannot open!\n" );  
		//exit;  
		return 0;
	}
    else
        printf("***** DirectMemory initialization begin *****\n" );   
    
    for(int i=0;i<SIZE_OF_DIRECT;i++)
    {
        fscanf(fp,"%s %x",&mm.DirectMemory[i].name,&mm.DirectMemory[i].addr);
        mm.DirectMemory[i].data = 0;
        //printf("%-20s\t%#x\n", mm.DirectMemory[i].name,mm.DirectMemory[i].addr); 
    }
        printf("***** DirectMemory initialization completed *****\n\n" ); 
	
    fclose(fp);
    return 0;
}

//找到寄存器在数组中的索引
int find_DM_info(uint8_t addr)
{
    for(int i=0;i<SIZE_OF_DIRECT;i++)
    {
        if(addr ==  mm.DirectMemory[i].addr)
            return i;
    }
	if(addr < mm.DirectMemory[0].addr || addr > mm.DirectMemory[SIZE_OF_DIRECT-1].addr)
		return SIZE_OF_DIRECT;
	else 
		return -1;
}

int find_DM_info_name(char name[])
{
    for(int i=0;i<SIZE_OF_DIRECT;i++)
    {
        if(strcmp(name,mm.DirectMemory[i].name) == 0)
    	{
//	    	printf("FOUND!\n");
	        return find_DM_info(mm.DirectMemory[i].addr);
    	}
			
    }
	return -1;
}


//写直接寄存器
void writeDirectMemory(uint16_t data, uint8_t addr)
{
    int info = find_DM_info(addr);
    if(info >SIZE_OF_DIRECT-1)
    {
        printf("Error: the address %#x is cross the border!\n",addr);
		return;
    }
	else if(info < 0)
	{
		printf("Error: the address %#x is not correct!\n",addr);
	    return;
	}
    mm.Data_Memory_Settings[info].data = data;
    //printf("Data %#x >>> %s\n",mm.Data_Memory_Settings[info].data,mm.DirectMemory[info].name);
}

uint16_t readDirectMemory(uint8_t addr)
{
    int info = find_DM_info(addr);
    if(info >SIZE_OF_DIRECT-1)
    {
        printf("Error: the address %#x is cross the border!\n",addr);
        return 0;
    }
	else if(info < 0)
	{
		printf("Error: the address %#x is not matched!\n",addr);
	    return 0;
	}
    printf("%#x %s = %#x\n",addr,mm.DirectMemory[info].name,mm.Data_Memory_Settings[info].data);
    return mm.Data_Memory_Settings[info].data;
}


uint16_t readDirectMemory_name(char name[])
{
    int info = find_DM_info_name(name);
    if(info >SIZE_OF_DIRECT-1 || info < 0)
    {
        printf("Error: the command %s is not matched!\n",name);
        return -1;
    }
    printf("%#x %s = %#x\n",mm.DirectMemory[info].addr,mm.DirectMemory[info].name,mm.Data_Memory_Settings[info].data);
    return mm.Data_Memory_Settings[info].data;
}

void setFlags(uint8_t data,uint8_t addr)
{
	int info = find_DM_info(addr);
    if(addr > mm.DirectMemory[SIZE_OF_DIRECT-1].addr || addr < 0)
    {
        printf("Error: the address %#x is cross the border!\n",addr);
        return;
    }
	if(data)
    mm.Data_Memory_Settings[info].data |= data;
}
void clearFlags(uint8_t data,uint8_t addr)
{
	int info = find_DM_info(addr);
    if(addr > mm.DirectMemory[SIZE_OF_DIRECT-1].addr || addr < 0)
    {
        printf("Error: the address %#x is cross the border!\n",addr);
        return;
    }
	if(data)
    mm.Data_Memory_Settings[info].data &= ~data;
}




//test
// void main()
// {
//     initMemory();
//     writeDirectMemory(0x06,SafetyAlertA);
// }
