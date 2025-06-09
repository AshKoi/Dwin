/*
 ******************************************************************************
 **
 ** \note Other information.
 **
 ******************************************************************************
 */
#include "global.h"

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
INT8U SpiFlashBuff[SST25_SPI_FLASH_BLOCK_SIZE];

/*!
 ******************************************************************************
 ** \brief Initialize the spi flash SST25F032 
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void SST25Init( void )
{
    uint16_t tBGR;
    uint32_t tSysFrePCLK2;
	
    /* Spi Initialize */
    bFM3_GPIO_PFR5_P0 = 1;
    bFM3_GPIO_PFR5_P1 = 1;
    bFM3_GPIO_PFR5_P2 = 1;
    FM3_GPIO->EPFR07 |= 0x0A800000;

	FM3_MFS3_CSIO->SCR = 0x00;
    FM3_MFS3_CSIO->SMR = 0x00;
    
	bFM3_MFS3_CSIO_SCR_UPCL = 1;
    FM3_MFS3_CSIO->SCR |= 0x21;
    FM3_MFS3_CSIO->SMR = 0x46;
    tSysFrePCLK2 = SystemCoreClock/(1<<((FM3_CRG->APBC2_PSR&0x03)));
    tBGR = (uint16_t)(((tSysFrePCLK2 + (SST25BAUDRATE / 2)) / (uint32_t)SST25BAUDRATE) - 1);
    FM3_MFS3_CSIO->BGR = tBGR;    
    FM3_MFS3_CSIO->SSR |= 0x80;
	bFM3_MFS3_CSIO_SCR_TXE = 0;
    bFM3_MFS3_CSIO_SCR_RXE = 0;
    bFM3_MFS3_CSIO_SMR_SOE = 1;

    bFM3_MFS3_CSIO_SMR_SCINV = 0;
    bFM3_MFS3_CSIO_SMR_BDS = 1;
		
    /* SCS1,SCS2,WP1 Initialize */
   
    bFM3_GPIO_PFR0_P8 = 0;
    bFM3_GPIO_DDR0_P8 = 1;
    bFM3_GPIO_PDOR0_P8 = 1;

    bFM3_GPIO_PFR0_P9 = 0;
    bFM3_GPIO_DDR0_P9 = 1;
    bFM3_GPIO_PDOR0_P9 = 1;

    bFM3_GPIO_PFR0_P7 = 0;
    bFM3_GPIO_DDR0_P7 = 1;
    bFM3_GPIO_PDOR0_P7 = 0;

    SST25WRSR(1,0);
    SST25WRSR(2,0);

}


/*!
 ******************************************************************************
 ** \brief Send single byte  
 **
 ** \param data
 **         
 ** \return send byte number
 **
 ******************************************************************************
 */
INT8U SSTSendByte( INT8U data )
{   
    INT32U delay = 0;
	
    while( !bFM3_MFS3_CSIO_SSR_TBI )
	{
        delay++;
        if( delay == 1000 )
            return 0;
	}
    bFM3_MFS3_CSIO_SCR_TXE = 1;
    bFM3_MFS3_CSIO_SCR_RXE = 1;
    FM3_MFS3_CSIO->TDR = data;
	delay = 0;
	while( !bFM3_MFS3_CSIO_SSR_RDRF )
    {
        delay++;
        if( delay == 1000 )
            return 0;
    }
	FM3_MFS3_CSIO->RDR;
    bFM3_MFS3_CSIO_SCR_RXE = 0;
    return 1;
}

/*!
 ******************************************************************************
 ** \brief receive single byte  
 **
 ** \param data
 **         
 ** \return send byte number
 **
 ******************************************************************************
 */
INT8U SSTGetByte( void )
{
    INT32U delay = 0;
	
    while( !bFM3_MFS3_CSIO_SSR_TBI )
	{
        delay++;
        if( delay == 1000 )
            return 0;
	}
    bFM3_MFS3_CSIO_SCR_TXE = 1;
    bFM3_MFS3_CSIO_SCR_RXE = 1;
    FM3_MFS3_CSIO->TDR = 0xff;
	delay = 0;    
	while( !bFM3_MFS3_CSIO_SSR_RDRF )
    {
        delay++;
        if( delay == 1000 )
            return 0;
    }
    bFM3_MFS3_CSIO_SCR_RXE = 0;
    return (FM3_MFS3_CSIO->RDR);	
}

/*!
 ******************************************************************************
 ** \brief the SST25 write enable  
 **
 ** \param bEnable: TRUE or FALSE
 **         
 ** \return None
 **
 ******************************************************************************
 */
void SST25WriteEnable(  INT8U chip, BOOLEAN bEnable)
{
	
    if( bEnable )
    {
        SST25ChipEnable(chip,TRUE);
        SSTSendByte( SST25_CMD_WRITE_ENABLE );
        SST25ChipEnable(chip,FALSE);
    }
    else
    {
        SST25ChipEnable(chip,TRUE);
        SSTSendByte( SST25_CMD_WRITE_DISABLE );
        SST25ChipEnable(chip,FALSE);
    }
}

/*!
 ******************************************************************************
 ** \brief the SST25 chip enable  
 **
 ** \param bEnable: TRUE or FALSE
 **        chip: 1/2
 ** \return None
 **
 ******************************************************************************
 */
void SST25ChipEnable( INT8U chip, BOOLEAN bEnable )
{
	if( chip == 1 )
	{
		if( bEnable ){	SST25SCS1EN(); }
		else         {	SST25SCS1DIS();}
	}
	else if( chip == 2 )
	{
		if( bEnable ){	SST25SCS2EN(); }
		else         {	SST25SCS2DIS();}
	}
}

/*!
 ******************************************************************************
 ** \brief read the SST25 status register  
 **
 ** \param chip
 **         
 ** \return status
 **
 ******************************************************************************
 */
INT8U SST25ReadStatus( INT8U chip )
{
    INT8U ret;
    
    SST25ChipEnable(chip,TRUE);
    SSTSendByte( SST25_CMD_STATUS_READ );
    ret = SSTGetByte();	
    SST25ChipEnable(chip,FALSE);
    return ret;    
}

/*!
 ******************************************************************************
 ** \brief write into the SST25 WRSR 
 **
 ** \param state
 **        chip
 ** \return None
 **
 ******************************************************************************
 */
void  SST25WRSR( INT8U chip, INT8U state )
{
	INT32U delay=0;
	
    SST25ChipEnable(chip,TRUE);
    SSTSendByte( SST25_CMD_EWSR );
    SST25ChipEnable(chip,FALSE);
    
    SST25ChipEnable(chip,TRUE);
    SSTSendByte( SST25_CMD_WRSR );
    SSTSendByte( state );
    SST25ChipEnable(chip,FALSE);
	while( SST25ReadStatus(chip) & SST25_STATUS_BUSY )
	{		
        delay++;
        if( delay == 100 )
            break;
	}
}

/*!
 ******************************************************************************
 ** \brief read the SST25 ID 
 **
 ** \param chip
 **         
 ** \return ID, it should be 0xBF254A00
 **
 ******************************************************************************
 */
INT32U SST25ReadID( INT8U chip )
{
    INT32U temp = 0;
	
    SST25ChipEnable(chip,TRUE);
    SSTSendByte( SST25_CMD_JEDEC_ID );
    temp = (temp | SSTGetByte()) << 8;	
    temp = (temp | SSTGetByte()) << 8;	
    temp = (temp | SSTGetByte()); 	
    SST25ChipEnable(chip,FALSE);
    return temp;
}

/*!
 ******************************************************************************
 ** \brief read data from the SST25 
 **
 ** \param buff, addr, len
 **         
 ** \return read bytes number
 **
 ******************************************************************************
 */
INT32U SST25SPIFlashRead( INT8U* buff, INT32U addr, INT32U len )
{
	INT8U chip, beyond ;
	INT32U secAddr, i;
	INT32U cnt,readnum=0;

	if( (addr < SST25_SPI_FLASH_CHIP_SIZE) && (addr+len)<SST25_SPI_FLASH_CHIP_SIZE )
	{
		chip = 1;
		beyond = 0;
	}
	else if( (addr < SST25_SPI_FLASH_CHIP_SIZE) && (addr+len)>=SST25_SPI_FLASH_CHIP_SIZE )
	{
		chip = 1;
		beyond = 1;	// 跨片
	}
	else if( (addr < 2*SST25_SPI_FLASH_CHIP_SIZE) && (addr+len)<2*SST25_SPI_FLASH_CHIP_SIZE )
	{
		chip = 2;
		beyond = 0;
	}
	else
		return 0;
	
	if( beyond != 1 )	// 在同一芯片内
	{
		secAddr = addr % SST25_SPI_FLASH_CHIP_SIZE;
		
    	SST25ChipEnable(chip,TRUE);
		SSTSendByte( SST25_CMD_READ_HIGH );
		SSTSendByte( (INT8U)(((addr) & 0xFFFFFF)>>16) );		 	
		SSTSendByte( (INT8U)(((addr) & 0xFFFF)>>8) );		 	
		SSTSendByte( (INT8U)(addr) );		 	
		SSTSendByte( 0 );		 	
		
		for( i=0; i<len; i++ )
		{
			*buff = SSTGetByte();
			readnum++;
			buff++;
		}
    	SST25ChipEnable(chip,FALSE);
	}
	else				// 跨片
	{
		// 读取第一片内的数据
		secAddr = addr % SST25_SPI_FLASH_CHIP_SIZE;
		cnt = SST25_SPI_FLASH_CHIP_SIZE - secAddr;
		
    	SST25ChipEnable(chip,TRUE);
		SSTSendByte( SST25_CMD_READ_HIGH );
		SSTSendByte( (INT8U)(secAddr>>16) );
		SSTSendByte( (INT8U)(secAddr>>8) );
		SSTSendByte( (INT8U)(secAddr) );
		SSTSendByte( 0 );	
		for( i=0; i<cnt; i++ )
		{
			*buff = SSTGetByte();
			readnum++;
			buff++;
		}			
    	SST25ChipEnable(chip,FALSE);
		
		// 读取第二片内的数据
		cnt = len - cnt ;
		chip += 1;
		secAddr = 0;		
    	SST25ChipEnable(chip,TRUE);
		SSTSendByte( SST25_CMD_READ_HIGH );
		SSTSendByte( (INT8U)(secAddr>>16) );
		SSTSendByte( (INT8U)(secAddr>>8) );
		SSTSendByte( (INT8U)(secAddr) );
		SSTSendByte( 0 );	
		for( i=0; i<cnt; i++ )
		{
			*buff = SSTGetByte();
			readnum++;
			buff++;
		}			
    	SST25ChipEnable(chip,FALSE);
	}
	    
    return readnum;
}

/*!
 ******************************************************************************
 ** \brief erase the SST25 
 **
 ** \param type, addr
 **         
 ** \return None
 **
 ******************************************************************************
 */
void SST25SPIFlashErase( INT8U type, INT32U addr )
{	
	INT8U chip;
	INT32U i=0;
	
	if( addr < SST25_SPI_FLASH_CHIP_SIZE )
		chip = 1;
	else if( addr < 2*SST25_SPI_FLASH_CHIP_SIZE )
		chip = 2;
	else 
		return;
	SST25ReadStatus(chip);
	SST25WriteEnable(chip,TRUE);	
SST25ReadStatus(chip);	
	SST25ChipEnable(chip,TRUE);
	if( type == SST25_ERASE_TYPE_CHIP )
	{
        SSTSendByte( SST25_CMD_ERASE_CHIPALL );	
	}
	else if( type == SST25_ERASE_TYPE_BLOCK )
	{
        SSTSendByte( SST25_CMD_ERASE_BLOCK );		 	
        SSTSendByte( (INT8U)((addr & 0xFFFFFF)>>16) );		 	
        SSTSendByte( (INT8U)((addr & 0xFFFF)>>8) );		 	
        SSTSendByte( (INT8U)(addr) );		 	
	}
	else if( type == SST25_ERASE_TYPE_SECTOR )
	{
        SSTSendByte( SST25_CMD_ERASE_SECTOR );		 	
        SSTSendByte( (INT8U)((addr & 0xFFFFFF)>>16) );		 	
        SSTSendByte( (INT8U)((addr & 0xFFFF)>>8) );		 	
        SSTSendByte( (INT8U)(addr) );		 	
	}
	else if( type == SST25_ERASE_TYPE_TWOSECTOR )
	{
        SSTSendByte( SST25_CMD_ERASE_TWOSECTOR );		 	
        SSTSendByte( (INT8U)((addr & 0xFFFFFF)>>16) );		 	
        SSTSendByte( (INT8U)((addr & 0xFFFF)>>8) );		 	
        SSTSendByte( (INT8U)(addr) );		 	
    }
	SST25ChipEnable(chip,FALSE);
	// 查询状态：擦除是否已完成
	while(( SST25ReadStatus(chip)& SST25_STATUS_BUSY )&&(i<1000))
	{		
		Delayus(100);
		i++;
	}
}

/*!
 ******************************************************************************
 ** \brief write data to the SST25 (同一片内)
 **
 ** \param  pStr, addr, len
 **         
 ** \return write num
 **
 ******************************************************************************
 */
INT32U SST25SPIFlashWrite( INT8U* pStr, INT32U addr, INT32U len )
{
	INT32U  j;
	INT32U iCount;
	INT8U chip;
	
	if( len == 0 )
		return 0;	
	if( addr < SST25_SPI_FLASH_CHIP_SIZE )
		chip = 1;
	else if( addr < 2*SST25_SPI_FLASH_CHIP_SIZE )
		chip = 2;
	else
		return 0;
    
	for( j=0; j<len; j++ )
	{
		SST25WriteEnable(chip,TRUE);	
		SST25ChipEnable(chip,TRUE);
        SSTSendByte( SST25_CMD_PROGRAM_BYTE );
        SSTSendByte( (INT8U)(((addr+j) & 0xFFFFFF)>>16) );
        SSTSendByte( (INT8U)(((addr+j) & 0xFFFF)>>8) );
        SSTSendByte( (INT8U)(addr+j) );
        SSTSendByte( *(pStr+j) );
		SST25ChipEnable(chip,FALSE);
        //iCount = 1000;
        iCount = 2000;
        while( (SST25ReadStatus(chip)) & SST25_STATUS_BUSY )
        {
            if( iCount-- == 0 )
                return j;
        }

	}
	return j;	    
}

/*!
 ******************************************************************************
 ** \brief write data to the SST25 (用AAI方式)
 **
 ** \param  pStr, addr, len
 **         
 ** \return write num
 **
 ******************************************************************************
 */
INT32U SST25SPIFlashWriteBuffer(INT8U* pBuffer, INT32U WriteAddr, INT32U NumByteToWrite)
{
	INT8U NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	INT32U NumByte;
	
	NumByte = NumByteToWrite;
	Addr = WriteAddr % SST25_SPI_FLASH_PAGE_SIZE;
	count = SST25_SPI_FLASH_PAGE_SIZE - Addr;
	NumOfPage =  NumByteToWrite / SST25_SPI_FLASH_PAGE_SIZE;
	NumOfSingle = NumByteToWrite % SST25_SPI_FLASH_PAGE_SIZE;
	if (Addr == 0) /*!< WriteAddr is sFLASH_PAGESIZE aligned  */
	{
		if (NumOfPage == 0) /*!< NumByteToWrite < sFLASH_PAGESIZE */
		{
			SST25SPIFlashWritePage(pBuffer, WriteAddr, NumByteToWrite);
		}
		else /*!< NumByteToWrite > sFLASH_PAGESIZE */
		{
			while (NumOfPage--)
			{
				SST25SPIFlashWritePage(pBuffer, WriteAddr, SST25_SPI_FLASH_PAGE_SIZE);
				WriteAddr +=  SST25_SPI_FLASH_PAGE_SIZE;
				pBuffer += SST25_SPI_FLASH_PAGE_SIZE;
			}
			
			SST25SPIFlashWritePage(pBuffer, WriteAddr, NumOfSingle);
		}
	}
	else /*!< WriteAddr is not sFLASH_PAGESIZE aligned  */
	{
		if (NumOfPage == 0) /*!< NumByteToWrite < sFLASH_PAGESIZE */
		{
			if (NumOfSingle > count) /*!< (NumByteToWrite + WriteAddr) > sFLASH_PAGESIZE */
			{
				temp = NumOfSingle - count;
				
				SST25SPIFlashWritePage(pBuffer, WriteAddr, count);
				WriteAddr +=  count;
				pBuffer += count;
				
				SST25SPIFlashWritePage(pBuffer, WriteAddr, temp);
			}
			else
			{
				SST25SPIFlashWritePage(pBuffer, WriteAddr, NumByteToWrite);
			}
		}
		else /*!< NumByteToWrite > sFLASH_PAGESIZE */
		{
			NumByteToWrite -= count;
			NumOfPage =  NumByteToWrite / SST25_SPI_FLASH_PAGE_SIZE;
			NumOfSingle = NumByteToWrite % SST25_SPI_FLASH_PAGE_SIZE;
			
			SST25SPIFlashWritePage(pBuffer, WriteAddr, count);
			// sFLASH_ReadBuffer(pBuffer, WriteAddr, count);
			WriteAddr +=  count;
			pBuffer += count;
			
			while (NumOfPage--)
			{
				SST25SPIFlashWritePage(pBuffer, WriteAddr, SST25_SPI_FLASH_PAGE_SIZE);
				WriteAddr +=  SST25_SPI_FLASH_PAGE_SIZE;
				pBuffer += SST25_SPI_FLASH_PAGE_SIZE;
			}
			
			if (NumOfSingle != 0)
			{
				SST25SPIFlashWritePage(pBuffer, WriteAddr, NumOfSingle);
			}
		}
	}
	return (NumByte);
}

/*!
 ******************************************************************************
 ** \brief write data to the SST25 (写一页)
 **
 ** \param  pStr, addr, len
 **         
 ** \return write num
 **
 ******************************************************************************
 */
INT32U SST25SPIFlashWritePage(INT8U* pBuffer, INT32U WriteAddr, INT32U NumByteToWrite)
{
	INT32U count,iCount;
	INT8U chip;
   
	if( WriteAddr < SST25_SPI_FLASH_CHIP_SIZE )
		chip = 1;
	else if( WriteAddr < 2*SST25_SPI_FLASH_CHIP_SIZE )
		chip = 2;
	else
		return 0;
	
	if(NumByteToWrite==0)
	{
		return 0;
	}
	else if(NumByteToWrite==1)
	{
		SST25SPIFlashWriteByte(WriteAddr,*pBuffer);
	}
	else
	{
		count=NumByteToWrite;
		if(WriteAddr%2)
		{		
			SST25SPIFlashWriteByte(WriteAddr,*pBuffer++);
			WriteAddr++;
			count--;
		}
		if(count>=2)
		{
			/*!< Enable the write access to the FLASH */
			SST25WriteEnable(chip,TRUE);	
			/*!< Select the FLASH: Chip Select low */
			SST25ChipEnable(chip,TRUE);
			/*!< Send "Write AAI " instruction */
			SSTSendByte(SST25_CMD_PROGRAM_AAI);
			/*!< Send WriteAddr high nibble address byte to write to */
			SSTSendByte((WriteAddr & 0xFF0000) >> 16);
			/*!< Send WriteAddr medium nibble address byte to write to */
			SSTSendByte((WriteAddr & 0xFF00) >> 8);
			/*!< Send WriteAddr low nibble address byte to write to */
			SSTSendByte(WriteAddr & 0xFF);
			/*!< Send the current byte */
			SSTSendByte(*pBuffer++);
			SSTSendByte(*pBuffer++);
			/*!< Deselect the FLASH: Chip Select high */
			SST25ChipEnable(chip,FALSE);
			count-=2;
			WriteAddr+=2;
			/*!< Wait the end of Flash writing */
			iCount = 100;
			while( (SST25ReadStatus(chip)) & SST25_STATUS_BUSY )
			{
				if( iCount-- == 0 )
					return 0;
			}
			for(;count>1;count-=2)
			{
				/*!< Select the FLASH: Chip Select low */
				SST25ChipEnable(chip,TRUE);
				/*!< Send "Write AAI " instruction */
				SSTSendByte(SST25_CMD_PROGRAM_AAI);				
				/*!< Send the current byte */
				SSTSendByte(*pBuffer++);
				SSTSendByte(*pBuffer++);
				/*!< Deselect the FLASH: Chip Select high */
				SST25ChipEnable(chip,FALSE);
				  /*!< Wait the end of Flash writing */
				iCount = 100;
				while( (SST25ReadStatus(chip)) & SST25_STATUS_BUSY )
				{
					if( iCount-- == 0 )
						return 0;
				}
				WriteAddr+=2;		
			}
			SST25ChipEnable(chip,TRUE);			
			/*!< Send "Write Enable" instruction */
			SSTSendByte(SST25_CMD_WRITE_DISABLE);		
			/*!< Deselect the FLASH: Chip Select high */
			SST25ChipEnable(chip,FALSE);
		}
		if(count==1)
		{
			SST25ChipEnable(chip,TRUE);
			SST25SPIFlashWriteByte(WriteAddr,*pBuffer);
		}
	
	}
	
	/*!< Wait the end of Flash writing */
	iCount = 100;
	while( (SST25ReadStatus(chip)) & SST25_STATUS_BUSY )
	{
		if( iCount-- == 0 )
			return 0;
	}
	return NumByteToWrite;
}

/*!
 ******************************************************************************
 ** \brief write byte to the SST25 (写字节)
 **
 ** \param  pStr, addr, len
 **         
 ** \return 
 **
 ******************************************************************************
 */
void SST25SPIFlashWriteByte( INT32U WriteAddr, INT8U WriteData )
{
	INT8U chip,iCount;
	
	if( WriteAddr < SST25_SPI_FLASH_CHIP_SIZE )
		chip = 1;
	else if( WriteAddr < 2*SST25_SPI_FLASH_CHIP_SIZE )
		chip = 2;
	else
		return;
	
	/*!< Enable the write access to the FLASH */
	SST25WriteEnable(chip,TRUE);
	/*!< Select the FLASH: Chip Select low */
	SST25ChipEnable(chip,TRUE);
	/*!< Send "Write to Memory " instruction */
	SSTSendByte(SST25_CMD_PROGRAM_BYTE);
	/*!< Send WriteAddr high nibble address byte to write to */
	SSTSendByte((WriteAddr & 0xFF0000) >> 16);
	/*!< Send WriteAddr medium nibble address byte to write to */
	SSTSendByte((WriteAddr & 0xFF00) >> 8);
	/*!< Send WriteAddr low nibble address byte to write to */
	SSTSendByte(WriteAddr & 0xFF);
	/*!< Send the current byte */
	SSTSendByte(WriteData);
	/*!< Deselect the FLASH: Chip Select high */
	SST25ChipEnable(chip,FALSE);
	/*!< Wait the end of Flash writing */
	iCount = 100;
	while( (SST25ReadStatus(chip)) & SST25_STATUS_BUSY )
	{
		if( iCount-- == 0 )
			return ;
	}
}
/*!
 ******************************************************************************
 ** \brief erase and write data to the SST25 
 **
 ** \param  buff, addr, len
 **         
 ** \return write num
 **
 ******************************************************************************
 */
INT32U SST25SPIFlashEraseWrite( INT8U* buff, INT32U addr, INT32U len )
{
	INT32U iBlockAddr;		// 扇区起始地址
	INT32U offset;			// 扇区内偏移量
	INT32U nBytesWrited;	// 记录当前已写入Flash的字节总数
	INT32U nCurrBytes;		// 本次需要写入Flash的字节数


	iBlockAddr = addr & 0x00FFF000;
	offset = addr & 0x0FFF;
	nBytesWrited = 0;
	while( nBytesWrited < len )
	{
		WatchDogClear(1);
		// 如果剩余字节数 >= SST25_SPI_FLASH_BLOCK_SIZE, 则取 SST25_SPI_FLASH_BLOCK_SIZE; 否则取剩余字节数。
		nCurrBytes = ( (len-nBytesWrited) >= SST25_SPI_FLASH_BLOCK_SIZE ) ? SST25_SPI_FLASH_BLOCK_SIZE : (len-nBytesWrited) ;
	
		if( (offset + nCurrBytes) > SST25_SPI_FLASH_BLOCK_SIZE )
			nCurrBytes = nCurrBytes - (( offset + nCurrBytes ) % SST25_SPI_FLASH_BLOCK_SIZE );
			
		// 1. 保存当前扇区的 4KBytes 数据至临时缓冲区
		SST25SPIFlashRead( SpiFlashBuff, iBlockAddr, SST25_SPI_FLASH_BLOCK_SIZE );
		// 2. 将 buff 中的数据填入临时缓冲区
		memcpy( SpiFlashBuff+offset, buff+nBytesWrited, nCurrBytes );
		// 3. 擦除当前扇区
		SST25SPIFlashErase( SST25_ERASE_TYPE_BLOCK, iBlockAddr );
		// 4. 将临时缓冲区中的数据写入当前扇区
#if SST25_WRITE_AAI
		if( SST25SPIFlashWritePage( SpiFlashBuff, iBlockAddr, SST25_SPI_FLASH_BLOCK_SIZE ) != SST25_SPI_FLASH_BLOCK_SIZE )
			return 0;
#else
		if( SST25SPIFlashWrite( SpiFlashBuff, iBlockAddr, SST25_SPI_FLASH_BLOCK_SIZE ) != SST25_SPI_FLASH_BLOCK_SIZE )
			return 0;
#endif
		// 5. 统计		
		iBlockAddr += SST25_SPI_FLASH_BLOCK_SIZE;
		
		nBytesWrited += nCurrBytes;
		offset = 0;
	}	
	return nBytesWrited;
}

////////////////////////////////////////////////////////////////////////////////
OS_EVENT* g_FlashLocker = NULL;
//#define FLASH_LOCKER_TIMEOUT		10000
INT32U    g_FlashTimeout = 0;

INT32U FlashInit( void )
{
    //INT32U 	nId 	= 0;
	//INT8U	i 		= 0;
	//INT8U	nNum	= 2;

	
    /*for (i = 0; i < nNum; i++)
	{
		nId = 0;
		FlashReadID(&nId);
		if (nId == 0xbf254a)
		{
			break;
		}
		OSTimeDlyHMSM(0, 0, 0, 100);
	}
    if (i >= nNum)
    
    {
	    SST25Init();
		nId = 0;
		FlashReadID(&nId);
		if (nId != 0xbf254a)
		{
			return FLASH_ERROR_BASE;
		}
	}*/
    SST25Init();
	
	
	g_FlashLocker = OSSemCreate( 1 );
	if( g_FlashLocker == NULL )
		return FLASH_ERROR_INIT;
	
	return FLASH_SUCC;
}

INT32U FlashReset( INT32U time )
{
	INT8U err;
	
	if( g_FlashLocker != NULL )
	{
		OSSemDel( g_FlashLocker, OS_DEL_NO_PEND, &err );
		g_FlashLocker = NULL;		
	}
    bFM3_GPIO_PDORA_P5 = 0;
	WatchDogClear(1);
	Delayms(200);
	WatchDogClear(1);
	return FlashInit( );
}

void FlashPowerOffOn( void )
{
#if 1
	INT8U i = 0;
    //bFM3_GPIO_PDORA_P5 = 0;
    
    V4_FLASH_OFF();
    bFM3_GPIO_PFR5_P0 = 0;
    bFM3_GPIO_PFR5_P1 = 0;
    bFM3_GPIO_PFR5_P2 = 0;
    GpioConfig(IO_PORT5, IO_PINx0, GPIO_MODE_OUTPUT);	//
    GpioConfig(IO_PORT5, IO_PINx1, GPIO_MODE_OUTPUT);	//
    GpioConfig(IO_PORT5, IO_PINx2, GPIO_MODE_OUTPUT);	//
    GpioConfig(IO_PORT0, IO_PINx7, GPIO_MODE_OUTPUT);	//
    GpioConfig(IO_PORT0, IO_PINx8, GPIO_MODE_OUTPUT);	//
    GpioConfig(IO_PORT0, IO_PINx9, GPIO_MODE_OUTPUT);	//
    
    GpioConfig(IO_PORTA, IO_PINx0, GPIO_MODE_OUTPUT);	//
    GpioConfig(IO_PORTA, IO_PINx1, GPIO_MODE_OUTPUT);	//
    GpioConfig(IO_PORTA, IO_PINx2, GPIO_MODE_OUTPUT);	//


	GpioClearOutput(IO_PORT5, IO_PINx0);	//初始为亮状态
	GpioClearOutput(IO_PORT5, IO_PINx1);	//初始为亮状态
	GpioClearOutput(IO_PORT5, IO_PINx2);	//初始为亮状态
	GpioClearOutput(IO_PORT0, IO_PINx7);	//初始为亮状态
	GpioClearOutput(IO_PORT0, IO_PINx8);	//初始为亮状态
	GpioClearOutput(IO_PORT0, IO_PINx9);	//初始为亮状态  
        
    GpioClearOutput(IO_PORTA, IO_PINx0);	//初始为亮状态
	GpioClearOutput(IO_PORTA, IO_PINx1);	//初始为亮状态
	GpioClearOutput(IO_PORTA, IO_PINx2);	//初始为亮状态  
    
    for (i = 0; i < 4; i++)
    {
		//Delayms(500);
		OSTimeDlyHMSM(0, 0, 0, 200);
		WatchDogClear(1);
    }
        
    GpioConfig(IO_PORT5, IO_PINx0, GPIO_MODE_INPUT);	//
    GpioConfig(IO_PORT5, IO_PINx1, GPIO_MODE_INPUT);	//
    GpioConfig(IO_PORT5, IO_PINx2, GPIO_MODE_INPUT);	//
    GpioConfig(IO_PORT0, IO_PINx7, GPIO_MODE_INPUT);	//
    GpioConfig(IO_PORT0, IO_PINx8, GPIO_MODE_INPUT);	//
    GpioConfig(IO_PORT0, IO_PINx9, GPIO_MODE_INPUT);	//
    
     GpioConfig(IO_PORTA, IO_PINx0, GPIO_MODE_INPUT);	//
    GpioConfig(IO_PORTA, IO_PINx1, GPIO_MODE_INPUT);	//
    GpioConfig(IO_PORTA, IO_PINx2, GPIO_MODE_INPUT);	//
    
    V4_FLASH_ON();
    /*
    GpioSetOutput(IO_PORT5, IO_PINx0);	//初始为亮状态
	GpioSetOutput(IO_PORT5, IO_PINx1);	//初始为亮状态
	GpioSetOutput(IO_PORT5, IO_PINx2);	//初始为亮状态
	GpioSetOutput(IO_PORT0, IO_PINx7);	//初始为亮状态
	GpioSetOutput(IO_PORT0, IO_PINx8);	//初始为亮状态
	GpioSetOutput(IO_PORT0, IO_PINx9);	//初始为亮状态  
        
    GpioSetOutput(IO_PORTA, IO_PINx0);	//初始为亮状态
	GpioSetOutput(IO_PORTA, IO_PINx1);	//初始为亮状态
	GpioSetOutput(IO_PORTA, IO_PINx2);	//初始为亮状态  
    
    */
    bFM3_GPIO_PFR5_P0 = 1;
    bFM3_GPIO_PFR5_P1 = 1;
    bFM3_GPIO_PFR5_P2 = 1;
    
  
    bFM3_GPIO_PFR0_P8 = 0;
    bFM3_GPIO_DDR0_P8 = 1;
    bFM3_GPIO_PDOR0_P8 = 1;

    bFM3_GPIO_PFR0_P9 = 0;
    bFM3_GPIO_DDR0_P9 = 1;
    bFM3_GPIO_PDOR0_P9 = 1;

    bFM3_GPIO_PFR0_P7 = 0;
    bFM3_GPIO_DDR0_P7 = 1;
    bFM3_GPIO_PDOR0_P7 = 0;
    //bFM3_GPIO_PDORA_P5 = 1;

	//SST25Init();
#endif
}

INT32U FlashReadID( INT32U* chipID )
{
	INT32U id[2];
	//INT8U err;
	//OS_SEM_DATA semData;
	
	//if( g_FlashLocker == NULL )	return FLASH_ERROR_INIT;
	if( chipID == NULL )		return FLASH_ERROR_ARGUMENTS;
	
	//OSSemPend( g_FlashLocker, g_FlashTimeout, &err );
	//if( err != OS_ERR_NONE )
		//return FLASH_ERROR_TIMEOUT;
	
	id[0] = SST25ReadID( 1 );
	id[1] = SST25ReadID( 2 );
	
	//OSSemQuery( g_FlashLocker, &semData );
	//if( semData.OSCnt == 0 )
		//OSSemPost( g_FlashLocker );
	
	if( id[0] == id[1] )
	{
		*chipID = id[0];
		return FLASH_SUCC;
	}
	else
	{
		return FLASH_ERROR_ID;
	}
}

INT32U FlashRead( INT32U addr, INT8U* buff, INT32U len )
{
	INT32U ret;
	INT8U err;
	OS_SEM_DATA semData;
	
	if( g_FlashLocker == NULL )	return FLASH_ERROR_INIT;
	if( buff == NULL )			return FLASH_ERROR_ARGUMENTS;
	if( len == 0 )				return 0;
	
	OSSemPend( g_FlashLocker, g_FlashTimeout, &err );
	if( err != OS_ERR_NONE )
		return FLASH_ERROR_TIMEOUT;

	ret = SST25SPIFlashRead( buff, addr, len );
	
	OSSemQuery( g_FlashLocker, &semData );
	if( semData.OSCnt == 0 )
		OSSemPost( g_FlashLocker );
	
	return ret;	
}

INT32U FlashWrite( INT32U addr, const INT8U* buff, INT32U len )
{
	INT32U ret;
	INT8U err;
	OS_SEM_DATA semData;
	
	if( g_FlashLocker == NULL )	return FLASH_ERROR_INIT;
	if( buff == NULL )			return FLASH_ERROR_ARGUMENTS;
	if( len == 0 )				return 0;

	OSSemPend( g_FlashLocker, g_FlashTimeout, &err );
	if( err != OS_ERR_NONE )
		return FLASH_ERROR_TIMEOUT;

	ret = SST25SPIFlashWrite( (INT8U*)buff, addr, len );
	
	OSSemQuery( g_FlashLocker, &semData );
	if( semData.OSCnt == 0 )
		OSSemPost( g_FlashLocker );
	
	return ret;	
}

INT32U FlashEraseWrite( INT32U addr, const INT8U* buff, INT32U len )
{
	INT32U ret;
	INT8U err;
	OS_SEM_DATA semData;
	
	if( g_FlashLocker == NULL )	return FLASH_ERROR_INIT;
	if( buff == NULL )			return FLASH_ERROR_ARGUMENTS;
	if( len == 0 )				return 0;

	OSSemPend( g_FlashLocker, g_FlashTimeout, &err );
	if( err != OS_ERR_NONE )
		return FLASH_ERROR_TIMEOUT;

	ret = SST25SPIFlashEraseWrite( (INT8U*)buff, addr, len );
	
	OSSemQuery( g_FlashLocker, &semData );
	if( semData.OSCnt == 0 )
		OSSemPost( g_FlashLocker );
	
	return ret;	
}

INT32U FlashErase( INT8U type, INT32U addr )
{
	SST25SPIFlashErase(type, addr);
	return FLASH_SUCC;
}
