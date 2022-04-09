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
        printf("%-20s\t%#x\n", mm.DirectMemory[i].name,mm.DirectMemory[i].addr); 
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
        memset(mm.DataSubMemory[i].data,0,sizeof(mm.DataSubMemory[i].data));
		
        printf("%-20s\t%#x\t%d\n", mm.DataSubMemory[i].name,mm.DataSubMemory[i].addr,mm.DataSubMemory[i].data_length); 
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
        printf("%-20s\t%#x\n", mm.ComSubMemory[i].name,mm.ComSubMemory[i].addr); 
    }
        printf("***** ComSubMemory initialization completed *****\n\n" ); 
	
    fclose(fp2);
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
		printf("Error: the address %#x cant't match one command!\n",addr);
	    return;
	}
    mm.Data_Memory_Settings[info].data = data;
    //printf("Data %#x >>> %#x %s\n",mm.Data_Memory_Settings[info].data,addr,mm.DirectMemory[info].name);
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
		printf("Error: the address %#x cant't match one command!\n",addr);
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
        printf("Error: the input command %s cant't match!\n",name);
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

int find_SM_info(uint16_t addr)
{
	for(int i=0;i<SIZE_OF_DATASUB;i++)
    {
        if(addr ==  mm.DataSubMemory[i].addr)
            return i;
    }
	for(int i=0;i<SIZE_OF_COMSUB;i++)
    {
        if(addr ==  mm.ComSubMemory[i].addr)
            return i;
    }

	return -1;
}

int find_SM_type(uint16_t addr)
{
	
	for(int i=0;i<SIZE_OF_DATASUB;i++)
    {
        if(addr ==  mm.DataSubMemory[i].addr)
            return 1;
    }
	for(int i=0;i<SIZE_OF_COMSUB;i++)
    {
        if(addr ==  mm.ComSubMemory[i].addr)
            return 0;
    }

	return -1;
}

int Reg_data_to_Buffer(int info)
{
	int data_length;
	data_length = mm.DataSubMemory[info].data_length;
	
	for(int i=0;i<data_length;i++)
	{
		writeDirectMemory(mm.DataSubMemory[info].data[i], 0x40+i);
	}

	writeDirectMemory(data_length+4, 0x61);
	//这里还应该再写CRC
	
	return 0;
}


int Buffer_data_to_Reg(int info,uint8_t data_length)

{	
	if(data_length <= mm.DataSubMemory[info].data_length)
	{
		for(int i=0;i<data_length;i++)
		{
			mm.DataSubMemory[info].data[i] = readDirectMemory(0x40 + i);
		}
		return 0;
	}
	else
		printf("Warning: Please input %d bytes!\n",mm.DataSubMemory[info].data_length);
	return 1;	
}


int ComSubCommand(int info)
{
	;
}

void SubCommand()
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
	if(type == 1)
	{
		Reg_data_to_Buffer(info);
		
		printf(">>> Write(1) or Read(0):\n");
		scanf("%d",&wr_buffer);
		if(wr_buffer==1)//write buffer
		{
			printf(">>> Bytes to Write(HEX):\n");//接收外部输入的数据，并写入40-5f

			int i=0;
			do
				{
					scanf("%x",&data_in[i]);
					i++;
					data_length++;
				}
			while (getchar() != '\n');
			
			for(int i=0;i<data_length;i++)
			{
				writeDirectMemory(data_in[i],0x40+i);
			}
			
			//这里之后要添加计算crc的函数，并写入0x60
			writeDirectMemory(data_length+4, 0x61);//数据长度写入0x61

			if(!Buffer_data_to_Reg(info,data_length))//缓冲区指定长度的数据写入子命令对应的存储区域
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
	else
		ComSubCommand(info);
	return;
}

void Command_Sequence()
{
	int command_in,wr_in;
	int data_in;
	uint8_t subcommand_in[2];

	printf(">>> waiting for command......\n");
	scanf("%x",&command_in);	
	switch (command_in)
	{
		case 0x3E://only write 2 bytes to 0x3e continously can trigger the subcommand
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
				while (getchar() != '\n');
				
				writeDirectMemory(subcommand_in[0],0x3E);
				writeDirectMemory(subcommand_in[1],0x3F);
				SubCommand();//转换得到子命令地址；任务完成前往3e、3f写入ff，任务完成后写入该子命令地址；根据地址触发相应的子命令操作，返回数据至缓冲区						
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
