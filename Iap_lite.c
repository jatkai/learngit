/******************************************************************
* 文件名：IAP_LITE.C
* 功能：实现IAP_Lite功能，进行字节擦除编程操作
* 日期：2003.11.10
* 说明：
******************************************************************/  
#define  uint8   unsigned  char
#define  uint16  unsigned  int

/* 定义页宽度屏蔽字(16字节时为0x0F，64字节时为0x3F) */
#define  PAGE_SIZE  0x3F

/* IAP_Lite相关寄存器定义 */
sfr	FMADRH	= 0xE7;
sfr	FMADRL  = 0xE6;
sfr	FMCON   = 0xE4;
sfr	FMDATA  = 0xE5;

/* 定义FLASH控制命令字 */
#define  LOAD       0x00      	/* 加载数据 	*/
#define  PROG       0x48        /* 编程		*/
#define  ERS_PROG   0x68        /* 擦除编程 	*/
#define  ERS_S      0x71        /* 擦除扇区 	*/
#define  ERS_P      0x70        /* 擦除页   	*/


/******************************************************************
* 名称：FLASH_WriteByte
* 功能：指定FLASH地址，写单字节数据
* 入口参数：addr		指定的地址
*	   dat		待写入的数据
* 出口参数：返回0时表示操作错误
******************************************************************/
//bit  FLASH_WriteByte(uint16 addr, uint8 dat)
//{  FMCON = LOAD;		// 设置加载数据命令  
//   FMADRH = addr>>8;		// 设置写入地址
//   FMADRL = addr&0xFF;		
//   FMDATA = dat;		// 写入数据
//   FMCON = ERS_PROG;		// 擦除编程
//   
//   if( (FMCON&0x0F)!=0x00 ) return(0);
//     else return(1);
//}


/******************************************************************
* 名称：FLASH_ReadByte
* 功能：指定FLASH地址，读单字节数据
* 入口参数：addr		指定的地址
* 出口参数：返回值即是读出的数据
******************************************************************/
//uint8  FLASH_ReadByte(uint16 addr)
//{  uint8  code *readp;
//
//   readp = addr;
//   return(*readp);
//}


/******************************************************************
* 名称：FLASH_WriteNByte
* 功能：指定FLASH起始地址，写多字节数据
* 入口参数：addr		指定的起始地址
*	   dat_buf	待写入的数据缓冲区指针
*          no		要写入数据个数
* 出口参数：返回0时表示操作错误
******************************************************************/
bit  FLASH_WriteNByte(uint16 addr, uint8  *dat_buf, uint8 no)
{  if(0==no) return(0);
   
   FMCON = LOAD;			// 设置加载数据命令  
   FMADRH = addr>>8;			// 设置写入地址
   FMADRL = addr&0xFF;
   while(1)
   {  FMDATA = *dat_buf;		// 写入数据
      dat_buf++;			// 指向下一个待写入的数据
      addr++;				
      no--;
      if(0==no) break;			// 判断是否写完所有数据，若是则退出
      
      if( (addr&PAGE_SIZE)==0 )		// 判断是否要换页
      {  FMCON = ERS_PROG;		// 若是，则擦除编程
         if( (FMCON&0x0F)!=0x00 ) return(0);
         /* 切换到下一页 */
         FMCON = LOAD;			
         FMADRH = addr>>8;
         FMADRL = addr&0xFF;
      }
   }
   FMCON = ERS_PROG;			// 擦除编程
   if( (FMCON&0x0F)!=0x00 ) return(0);      
     else  return(1);
}

/******************************************************************
* 名称：FLASH_ReadNByte
* 功能：指定FLASH起始地址，读多字节数据
* 入口参数：addr		指定的起始地址
*	   dat_buf	保存数据的缓冲区指针
*          no		要写入数据个数
* 出口参数：无
******************************************************************/
void  FLASH_ReadNByte(uint16 addr, uint8 *dat_buf, uint8 no)
{  uint8 code *readp;
	
   if(0==no) return;
   readp = addr;				// 设置读取的地址
   for(; no>0; no--)
   {  *dat_buf = *readp;		// 读取数据
      dat_buf++;			
      readp++;				// 指向下一地址
   }  
}


/******************************************************************
* 名称：FLASH_EraseSector
* 功能：扇区擦除
* 入口参数：addr		指定待擦除扇区内的地址
* 出口参数：返回0时表示操作错误
******************************************************************/
//bit  FLASH_EraseSector(uint16 addr)
//{  FMADRH = addr>>8;
//   FMADRL = addr&0xFF;
//   FMCON = ERS_S;
//   
//   if( (FMCON&0x0F)!=0x00 ) return(0);
//     else return(1);
//}


/******************************************************************
* 名称：FLASH_ErasePage
* 功能：页擦除
* 入口参数：addr		指定待擦除页内的地址
* 出口参数：返回0时表示操作错误
******************************************************************/
//bit  FLASH_ErasePage(uint16 addr)
//{  FMADRH = addr>>8;
//   FMADRL = addr&0xFF;
//   FMCON = ERS_P;
//   
//   if( (FMCON&0x0F)!=0x00 ) return(0);
//     else return(1);
//}


/******************************************************************
* 名称：FLASH_FillPage
* 功能：页填充
* 入口参数：addr		指定待填充页内的地址
*          dat		待填充的数据
* 出口参数：返回0时表示操作错误
* 说明：函数会先调用FLASH_ErasePage擦除扇区
******************************************************************/
//bit  FLASH_FillPage(uint16 addr, uint8  dat)
//{  uint8  i;
//	
//   /* 擦除扇区 */
//   if( FLASH_ErasePage(addr)==0 ) return(0);
//   /* 填充数据 */
//   FMADRH = addr>>8;			// 设置页起始地址
//   FMADRL = addr&(~PAGE_SIZE);
//   FMCON = LOAD;			// 设置加载数据命令
//   for(i=0; i<=PAGE_SIZE; i++)
//   {  FMDATA = dat;			// 加载数据
//   }
//   FMCON = PROG;			// 开始编程
//   
//   if( (FMCON&0x0F)!=0x00 ) return(0);
//     else return(1);
//}

		

























