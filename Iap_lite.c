/******************************************************************
* �ļ�����IAP_LITE.C
* ���ܣ�ʵ��IAP_Lite���ܣ������ֽڲ�����̲���
* ���ڣ�2003.11.10
* ˵����
******************************************************************/  
#define  uint8   unsigned  char
#define  uint16  unsigned  int

/* ����ҳ���������(16�ֽ�ʱΪ0x0F��64�ֽ�ʱΪ0x3F) */
#define  PAGE_SIZE  0x3F

/* IAP_Lite��ؼĴ������� */
sfr	FMADRH	= 0xE7;
sfr	FMADRL  = 0xE6;
sfr	FMCON   = 0xE4;
sfr	FMDATA  = 0xE5;

/* ����FLASH���������� */
#define  LOAD       0x00      	/* �������� 	*/
#define  PROG       0x48        /* ���		*/
#define  ERS_PROG   0x68        /* ������� 	*/
#define  ERS_S      0x71        /* �������� 	*/
#define  ERS_P      0x70        /* ����ҳ   	*/


/******************************************************************
* ���ƣ�FLASH_WriteByte
* ���ܣ�ָ��FLASH��ַ��д���ֽ�����
* ��ڲ�����addr		ָ���ĵ�ַ
*	   dat		��д�������
* ���ڲ���������0ʱ��ʾ��������
******************************************************************/
//bit  FLASH_WriteByte(uint16 addr, uint8 dat)
//{  FMCON = LOAD;		// ���ü�����������  
//   FMADRH = addr>>8;		// ����д���ַ
//   FMADRL = addr&0xFF;		
//   FMDATA = dat;		// д������
//   FMCON = ERS_PROG;		// �������
//   
//   if( (FMCON&0x0F)!=0x00 ) return(0);
//     else return(1);
//}


/******************************************************************
* ���ƣ�FLASH_ReadByte
* ���ܣ�ָ��FLASH��ַ�������ֽ�����
* ��ڲ�����addr		ָ���ĵ�ַ
* ���ڲ���������ֵ���Ƕ���������
******************************************************************/
//uint8  FLASH_ReadByte(uint16 addr)
//{  uint8  code *readp;
//
//   readp = addr;
//   return(*readp);
//}


/******************************************************************
* ���ƣ�FLASH_WriteNByte
* ���ܣ�ָ��FLASH��ʼ��ַ��д���ֽ�����
* ��ڲ�����addr		ָ������ʼ��ַ
*	   dat_buf	��д������ݻ�����ָ��
*          no		Ҫд�����ݸ���
* ���ڲ���������0ʱ��ʾ��������
******************************************************************/
bit  FLASH_WriteNByte(uint16 addr, uint8  *dat_buf, uint8 no)
{  if(0==no) return(0);
   
   FMCON = LOAD;			// ���ü�����������  
   FMADRH = addr>>8;			// ����д���ַ
   FMADRL = addr&0xFF;
   while(1)
   {  FMDATA = *dat_buf;		// д������
      dat_buf++;			// ָ����һ����д�������
      addr++;				
      no--;
      if(0==no) break;			// �ж��Ƿ�д���������ݣ��������˳�
      
      if( (addr&PAGE_SIZE)==0 )		// �ж��Ƿ�Ҫ��ҳ
      {  FMCON = ERS_PROG;		// ���ǣ���������
         if( (FMCON&0x0F)!=0x00 ) return(0);
         /* �л�����һҳ */
         FMCON = LOAD;			
         FMADRH = addr>>8;
         FMADRL = addr&0xFF;
      }
   }
   FMCON = ERS_PROG;			// �������
   if( (FMCON&0x0F)!=0x00 ) return(0);      
     else  return(1);
}

/******************************************************************
* ���ƣ�FLASH_ReadNByte
* ���ܣ�ָ��FLASH��ʼ��ַ�������ֽ�����
* ��ڲ�����addr		ָ������ʼ��ַ
*	   dat_buf	�������ݵĻ�����ָ��
*          no		Ҫд�����ݸ���
* ���ڲ�������
******************************************************************/
void  FLASH_ReadNByte(uint16 addr, uint8 *dat_buf, uint8 no)
{  uint8 code *readp;
	
   if(0==no) return;
   readp = addr;				// ���ö�ȡ�ĵ�ַ
   for(; no>0; no--)
   {  *dat_buf = *readp;		// ��ȡ����
      dat_buf++;			
      readp++;				// ָ����һ��ַ
   }  
}


/******************************************************************
* ���ƣ�FLASH_EraseSector
* ���ܣ���������
* ��ڲ�����addr		ָ�������������ڵĵ�ַ
* ���ڲ���������0ʱ��ʾ��������
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
* ���ƣ�FLASH_ErasePage
* ���ܣ�ҳ����
* ��ڲ�����addr		ָ��������ҳ�ڵĵ�ַ
* ���ڲ���������0ʱ��ʾ��������
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
* ���ƣ�FLASH_FillPage
* ���ܣ�ҳ���
* ��ڲ�����addr		ָ�������ҳ�ڵĵ�ַ
*          dat		����������
* ���ڲ���������0ʱ��ʾ��������
* ˵�����������ȵ���FLASH_ErasePage��������
******************************************************************/
//bit  FLASH_FillPage(uint16 addr, uint8  dat)
//{  uint8  i;
//	
//   /* �������� */
//   if( FLASH_ErasePage(addr)==0 ) return(0);
//   /* ������� */
//   FMADRH = addr>>8;			// ����ҳ��ʼ��ַ
//   FMADRL = addr&(~PAGE_SIZE);
//   FMCON = LOAD;			// ���ü�����������
//   for(i=0; i<=PAGE_SIZE; i++)
//   {  FMDATA = dat;			// ��������
//   }
//   FMCON = PROG;			// ��ʼ���
//   
//   if( (FMCON&0x0F)!=0x00 ) return(0);
//     else return(1);
//}

		

























