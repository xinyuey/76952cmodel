/* Memory */

#include "common.h"

//内存初始化
int initMemory()
{
    //读取外部配置文件，初始化直接寄存器
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
        printf("%-30s\t%#x\n", mm.DirectMemory[i].name,mm.DirectMemory[i].addr); 
    }
		//printf("SIZE_OF_DIRECT = %d\n",SIZE_OF_DIRECT);
        printf("***** DirectMemory initialization completed *****\n\n" ); 
	
    fclose(fp);

	//读取外部配置文件，初始化间接数据寄存器
	FILE *fp1;
	fp1 = fopen("../sim/InitDataSubMemory.txt","r");
    if(fp1==NULL)
    {  
		printf("File cannot open!\n" );  
		//exit;  
		return 0;
	}
    else
        printf("***** DataSubMemory initialization begin *****\n" );   

    for(int i=0;i<SIZE_OF_DATASUB;i++)
    {
        fscanf(fp1,"%s%x%d",&mm.DataSubMemory[i].name,&mm.DataSubMemory[i].addr,&mm.DataSubMemory[i].data_length);
        memset(mm.DataSubMemory[i].data,0,SIZE_OF_DATA_BUFFER);
		
        printf("%-30s\t%#x\t%d\n", mm.DataSubMemory[i].name,mm.DataSubMemory[i].addr,mm.DataSubMemory[i].data_length); 
    }
        printf("***** DataSubMemory initialization completed *****\n\n" ); 
	
    fclose(fp1);

	//读取外部配置文件，初始化间接命令寄存器
	FILE *fp2;
	fp2 = fopen("../sim/InitComSubMemory.txt","r");
    if(fp2==NULL)
    {  
		printf("File cannot open!\n" );  
		//exit;  
		return 0;
	}
    else
        printf("***** ComSubMemory initialization begin *****\n" );   
    
    for(int i=0;i<SIZE_OF_COMSUB;i++)
    {
        fscanf(fp2,"%s %x",&mm.ComSubMemory[i].name,&mm.ComSubMemory[i].addr);
        printf("%-30s\t%#x\n", mm.ComSubMemory[i].name,mm.ComSubMemory[i].addr); 
    }
        printf("***** ComSubMemory initialization completed *****\n\n" ); 
	
    fclose(fp2);
	//读取外部配置文件，初始化配置寄存器
	FILE *fp3;
	fp3 = fopen("../sim/InitDataMemorySettings.txt","r");
    if(fp3==NULL)
    {  
		printf("File cannot open!\n" );  
		//exit;  
		return 0;
	}
    else
        printf("***** DataMemorySettings initialization begin *****\n" );   
    
    for(int i=0;i<SIZE_OF_SETTING;i++)
    {
        fscanf(fp1,"%s%x%d",&mm.Data_Memory_Settings[i].name,&mm.Data_Memory_Settings[i].addr,&mm.Data_Memory_Settings[i].data_length);
        memset(mm.Data_Memory_Settings[i].data,0,SIZE_OF_DATA_BUFFER);
		
        printf("%-30s\t%#x\t%d\n", mm.Data_Memory_Settings[i].name,mm.Data_Memory_Settings[i].addr,mm.Data_Memory_Settings[i].data_length); 
    }
        printf("***** DataMemorySettings initialization completed *****\n\n" ); 
	
    fclose(fp3);
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
	if(addr < mm.DirectMemory[0].addr || addr > mm.DirectMemory[SIZE_OF_DIRECT-1].addr)//地址越界，未找到
		return SIZE_OF_DIRECT;
	else 
		return -1;//地址其他错误，未找到
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
		printf("Error: the address %#x cant't match one command!\n",addr);
	    return;
	}
    mm.DirectMemory[info].data = data;
    //printf("Data %#x >>> %#x %s\n",mm.DirectMemory[info].data,addr,mm.DirectMemory[info].name);
}

//读直接寄存器
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
		printf("Error: the address %#x cant't match one command!\n",addr);
	    return 0;
	}
    //printf("%#x %s = %#x\n",addr,mm.DirectMemory[info].name,mm.DirectMemory[info].data);
    return mm.DirectMemory[info].data;
}

//置位保护状态标志位
void setFlags(uint8_t data,uint8_t addr)
{
	int info = find_DM_info(addr);
    if(addr > mm.DirectMemory[SIZE_OF_DIRECT-1].addr || addr < 0)
    {
        printf("Error: the address %#x is cross the border!\n",addr);
		printf("setFlags");
        return;
    }
	if(data)
    		mm.DirectMemory[info].data |= data;

}

//清除保护状态标志位
void clearFlags(uint8_t data,uint8_t addr)
{
	int info = find_DM_info(addr);
    if(addr > mm.DirectMemory[SIZE_OF_DIRECT-1].addr || addr < 0)
    {
        printf("Error: the address %#x is cross the border!\n",addr);
        return;
    }
	if(data)
    		mm.DirectMemory[info].data &= ~data;
}

//按地址索引子命令寄存器
int find_SM_info(uint16_t addr)
{
	for(int i=0;i<SIZE_OF_DATASUB;i++)//data_submemory
    {
        if(addr ==  mm.DataSubMemory[i].addr)
            return i;
    }
	for(int i=0;i<SIZE_OF_COMSUB;i++)//command_submemory
    {
        if(addr ==  mm.ComSubMemory[i].addr)
            return i;
    }
	for(int i=0;i<SIZE_OF_SETTING;i++)//data_memory_settings
    {
        if(addr ==  mm.Data_Memory_Settings[i].addr)
            return i;
    }

	return -1;
}

//判断数据相关或仅命令子命令
int find_SM_type(uint16_t addr)
{
	
	for(int i=0;i<SIZE_OF_DATASUB;i++)//data_submemory
    {
        if(addr ==  mm.DataSubMemory[i].addr)
            return 1;
    }
	for(int i=0;i<SIZE_OF_COMSUB;i++)//command_submemory
    {
        if(addr ==  mm.ComSubMemory[i].addr)
            return 0;
    }
	for(int i=0;i<SIZE_OF_SETTING;i++)//data_memory_settings
    {
        if(addr ==  mm.Data_Memory_Settings[i].addr)
            return 2;
    }

	return -1;
}

/*	
	内部向二级寄存器的指定偏移地址写入指定字节数据
	uint16_t addr:双字节子命令地址
	uint8_t offset:数据偏移地址
	uint8_t *write_data:指向单字节数据的指针
	uint8_t write_len:写入数据字节数
*/
void writeSubMemory(uint16_t addr,uint8_t offset,uint8_t *data,uint8_t write_len)
{
	int info = find_SM_info(addr);
	if(info < 0)
	{
		return;
	}
	else
	{
		if(offset+write_len > mm.DataSubMemory[info].data_length)
		{
			return;
		}
		else
		{
			while(write_len > 0)
			{
				mm.DataSubMemory[info].data[offset] = *data;
				//printf("data")
				write_len --;
				if(write_len > 0)
				{
					offset ++;
					data ++;
				}
					
			}
		}
		
	}	
}

/*
	内部从二级寄存器的指定偏移地址读出指定字节数据
	uint16_t addr:双字节子命令地址
	uint8_t offset:数据偏移地址
	uint8_t read_len:读取数据字节数	
	return pdata：返回一个指向目标数据数组的指针，再用一个数组接收即可
*/
uint8_t* readSubMemory(uint16_t addr,uint8_t offset,uint8_t read_len)
{
	uint8_t data[read_len];
	uint8_t *pdata = data;
	int info = find_SM_info(addr);
	if(info < 0)
	{
		return 0;
	}
	else
	{
		if(offset+read_len > mm.DataSubMemory[info].data_length)
		{
			return 0;
		}
		else
		{
			for(int i=0;i<read_len;i++)
			{
				data[i] = mm.DataSubMemory[info].data[offset+i];
			}
		}
		
	}
	return pdata;
}


/*	
	内部连续多字节写配置寄存器
	uint16_t addr:双字节子命令地址
	uint8_t *write_data:指向单字节数据数组的指针
	uint8_t write_len:写入数据字节数
*/
void writeSettings(uint16_t addr,uint8_t *write_data,uint8_t write_len)
{
	int info = find_SM_info(addr);
	if(info < 0)
	{
		return;
	}
	else
	{
		if(write_len > mm.Data_Memory_Settings[info].data_length)
		{
			return;
		}
		else
		{
			for(int i=0;i<write_len;i++)
			{
				mm.Data_Memory_Settings[info].data[i] = *write_data;
				write_data ++;
			}
		}
		
	}	
}

/*	
	内部连续多字节读配置寄存器
	uint16_t addr:双字节子命令地址
	return pdata：返回一个指向目标数据数组的指针，再用一个数组接收即可
*/
uint8_t* readSettings(uint16_t addr)
{
	int info = find_SM_info(addr);
	int data_len = mm.Data_Memory_Settings[info].data_length;
	uint8_t data[data_len];
	uint8_t *pdata = data;
	
	if(info < 0)
	{
		return 0;
	}
	else
	{
		for(int i=0;i<data_len;i++)
		{
			data[i] = mm.Data_Memory_Settings[info].data[i];
		}
		
	}
	return pdata;
}





/*下面是外部输入子命令的执行逻辑,内部逻辑用不到*/

//子命令数据写入缓冲区
int Reg_data_to_Buffer(int mem_info,int mem_type)
{
	int data_length;
	if(mem_type == 1)//data_submemory
	{	
		data_length = mm.DataSubMemory[mem_info].data_length;
		
		for(int i=0;i<data_length;i++)
		{
			writeDirectMemory(mm.DataSubMemory[mem_info].data[i], 0x40+i);
		}
		writeDirectMemory(data_length+4, 0x61);
		//这里还应该再写CRC
		return 0;
	}
	else if(mem_type == 2)//data_memory_settings
	{	
		data_length = mm.Data_Memory_Settings[mem_info].data_length;
		
		for(int i=0;i<data_length;i++)
		{
			writeDirectMemory(mm.Data_Memory_Settings[mem_info].data[i], 0x40+i);
		}
		writeDirectMemory(data_length+4, 0x61);
		//这里还应该再写CRC
		return 0;
	}
	else
		return -1;
}

//缓冲区数据写入子命令数据区域
int Buffer_data_to_Reg(int mem_info,uint8_t data_length,int mem_type)

{	
	if(mem_type == 1)//data_submemory
	{
		if(data_length <= mm.DataSubMemory[mem_info].data_length)
		{

			for(int i=0;i<data_length;i++)
			{
				mm.DataSubMemory[mem_info].data[i] = readDirectMemory(0x40 + i);
			}
			return 0;
		}
		else
		{
			printf("Warning: Please input %d bytes!\n",mm.DataSubMemory[mem_info].data_length);
			return -1;	
		}
	}
	else if(mem_type == 2)//data_memory_settings
	{
		if(data_length <= mm.Data_Memory_Settings[mem_info].data_length)
		{
			for(int i=0;i<data_length;i++)
			{
				mm.Data_Memory_Settings[mem_info].data[i] = readDirectMemory(0x40 + i);
			}
			return 0;
		}
		else
		{
			printf("Warning: Please input %d bytes!\n",mm.Data_Memory_Settings[mem_info].data_length);
			return -1;	
		}	
	}
	else
		return -1;
}

//仅命令子命令执行逻辑（仅内部逻辑，无外部输入序列）
int ComSubCommand(int info)
{
	return 0;
}

//子命令执行状态机（仅内部逻辑，无外部输入序列）
//void SubCommand()
//{
//	uint8_t subcommand_l,subcommand_h;
//	uint16_t subcommand;
//	
//	subcommand_l = readDirectMemory(0x3E);   
//	subcommand_h = readDirectMemory(0x3F);
//	subcommand = subcommand_l | (subcommand_h << 8);
//}

//外部子命令序列（子命令地址已写入3e3f）
void SubCommand_Sequence()
{
	int wr_buffer = 0;
	uint8_t subcommand_l,subcommand_h;
	uint16_t subcommand;
	uint8_t data_in[32] = {0};
	uint8_t data_length = 0;
	
	subcommand_l = readDirectMemory(0x3E);   
	subcommand_h = readDirectMemory(0x3F);
	subcommand = subcommand_l | (subcommand_h << 8);

	writeDirectMemory(0xFF,0x3E);
	writeDirectMemory(0xFF,0x3F);

	int info = find_SM_info(subcommand);
	if(info < 0)
	{
		printf("Error: the address %#x cant't match one SubCommand!\n",subcommand);
	    return;
	}
	
	int type = find_SM_type(subcommand);
	if(type == 1 || type == 2)//数据相关子命令、data_memory_settings
	{
		Reg_data_to_Buffer(info,type);
		
		printf(">>> Write(1) or Read(0):\n");
		scanf("%d",&wr_buffer);
		if(wr_buffer==1)//write buffer
		{
			printf(">>> Bytes to Write(HEX,little_endian):\n");//接收外部输入的数据，并写入40-5f

			int i=0;
			do
				{
					scanf("%x",&data_in[i]);
					i++;
					data_length++;
				}
			while (getchar() != '\n');//以空格符按字节分割外部输入数据
			
			for(int i=0;i<data_length;i++)
			{
				writeDirectMemory(data_in[i],0x40+i);//按字节将外部输入数据写入缓冲区
			}
			
			//这里之后要添加计算crc的函数，并写入0x60
			writeDirectMemory(data_length+4, 0x61);//将外部输入数据长度写入0x61

			if(!Buffer_data_to_Reg(info,data_length,type))//缓冲区指定长度的数据写入子命令对应的存储区域
			{
				writeDirectMemory(subcommand_l,0x3E);//子命令完成任务的标志
				writeDirectMemory(subcommand_h,0x3F);
			}
			return;
		}
		else if(wr_buffer==0)//read buffer
		{
			for(int i=0;i<mm.DataSubMemory[info].data_length;i++)
			{
				readDirectMemory(0x40+i);
			}
			
			writeDirectMemory(subcommand_l,0x3E);
			writeDirectMemory(subcommand_h,0x3F);
			return;
		}
		else
			return;
	}	
	else//仅动作子命令
		ComSubCommand(info);
	
	return;
}

//外部命令序列
void Command_Sequence()
{
	int command_in,wr_in;
	int data_in;
	uint8_t subcommand_in[2];

	printf(">>> waiting for command......\n");
	scanf("%x",&command_in);	
	switch (command_in)
	{
		case 0x3E://只有向3e、3f连续写入两字节才能触发子命令序列
			printf(">>> Write(1) or Read(0):\n");
			scanf("%d",&wr_in);
			if(wr_in)
			{
				printf(">>> 2 Bytes to Write (HEX,little_endian):\n");
				int i=0;
				do
					{
						scanf("%x",&subcommand_in[i]);
						i++;
					}
				while (getchar() != '\n');//以空格符分隔子命令高低字节
				
				writeDirectMemory(subcommand_in[0],0x3E);
				writeDirectMemory(subcommand_in[1],0x3F);
				SubCommand_Sequence();//转换得到子命令地址；任务完成前往3e、3f写入ff，任务完成后往3e、3f写入该子命令地址；根据地址触发相应的子命令操作，返回数据至缓冲区						
			}
			else
				readDirectMemory(0x3E);
			break;
			
		default://direct command
			printf(">>> Write(1) or Read(0):\n");
			scanf("%d",&wr_in);
			if(wr_in)	
			{
				printf(">>> Bytes to Write (HEX):\n");
				scanf("%x",&data_in);
				writeDirectMemory(data_in,command_in);
			}
			else
				readDirectMemory(command_in);						
			break;
	}	
}

void writeBuffer(uint8_t write_addr,uint8_t *write_buff,uint8_t write_bytes)//向缓冲区连续写指定字节数据
{
	//uint8_t write_len;
	
	while(write_bytes > 0)
	{
		writeDirectMemory(*write_buff,write_addr);
		write_bytes --;
		if(write_bytes > 0)
		{
			write_addr ++;
			write_buff ++;
		}
		
	}
	return;
}

void readBuffer(uint8_t read_addr,uint8_t read_bytes)//向缓冲区连续读指定字节数据
{
	for(int i=0;i<read_bytes;i++)
	{
		readDirectMemory(read_addr+i);
	}
	return;
}
