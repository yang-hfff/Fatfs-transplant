/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "W25QXX.h"

/* Definitions of physical drive number for each drive */
#define DEV_FLASH		0	/* Example: Map Ramdisk to physical drive 0 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case DEV_FLASH :
		result = SPI_FLASH_ReadID();
        
		// translate the reslut code here
        stat = (result == W25Q128ID)?FR_OK:STA_NOINIT;
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case DEV_FLASH :
        SPI_FLASH_Init();
        for(uint16_t i = 0;i < 500;i++);
        SPI_Flash_WAKEUP();
        result=disk_status(DEV_FLASH);

		// translate the reslut code here
        stat = (result == FR_OK)?FR_OK:STA_NOINIT;
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_FLASH :
		// translate the arguments here
        /* 扇区偏移2MB，外部Flash文件系统空间放在SPI Flash后面的空间！！！ */
        sector+=512;
        SPI_FLASH_BufferRead(buff, sector <<12, count<<12);
        result = RES_OK;
    
		// translate the reslut code here
        res = result;
		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;
    
    uint32_t write_addr;
    
	switch (pdrv) {
	case DEV_FLASH :
		// translate the arguments here
        if (!count) {
            return RES_PARERR;    /* Check parameter */
        }
        /* 扇区偏移2MB，外部Flash文件系统空间放在SPI Flash后面的空间 */
        sector+=512;
        write_addr = sector<<12;
        SPI_FLASH_SectorErase(write_addr);
        SPI_FLASH_BufferWrite((u8 *)buff,write_addr,count<<12);
        result = RES_OK;

		// translate the reslut code here
        res = result;
		return res;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_FLASH :

		// Process of the command for the RAM drive
        switch (cmd) {
        /* 扇区数量：3584*4096/1024/1024=(16-2)(MB) */
        case GET_SECTOR_COUNT:
            *(DWORD * )buff = 3584;
            break;
        /* 扇区大小  */
        case GET_SECTOR_SIZE :
            *(WORD * )buff = 4096;
            break;
        /* 同时擦除扇区个数 */
        case GET_BLOCK_SIZE :
            *(DWORD * )buff = 1;
            break;
        }
        result = RES_OK;
        
        // translate the reslut code here
        res = result;
		return res;
	}

	return RES_PARERR;
}



