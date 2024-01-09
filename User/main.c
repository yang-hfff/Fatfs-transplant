#include "sys.h"
#include "systick.h"
#include "usart.h"
#include "logs.h"
#include "W25QXX.h"
#include "ff.h"


//ϵͳ�ļ����
FATFS fs;    
//ֻ�и�ʽ����ʱ��Ҫ��work
BYTE work[FF_MAX_SS]; 



int main(void)
{
    FIL fp;   
    FRESULT res;               
    UINT fnum;                     
    BYTE readBuffer[1024]= {0};      
    BYTE writeBuffer[] = "hello world\r\n";
    
    uart_init(115200);
#if 0
    SPI_FLASH_Init();
    LOG_I("erase flash now,please waite...\r\n");
    SPI_FLASH_BulkErase();
    LOG_I("erase flash finished!!!\r\n");
#else   
    res = f_mount(&fs,"0:",1);
    if (res == FR_NO_FILESYSTEM)
    {
        LOG_I("FLASH have no file system,start init...\r\n");
        /* ��ʽ�� */
        res=f_mkfs("0:",0,work,sizeof work);

        if (res == FR_OK)
        {
            LOG_I("FLASH formate success!!!\r\n");
            /* ��ʽ������ȡ������ */
            res = f_mount(NULL,"0:",1);
            /* ���¹��� */
            res = f_mount(&fs,"0:",1);
        }
        else
        {
            LOG_E("formate fail!!!\r\n");
            while (1);
        }
    }
    else if (res != FR_OK)
    {
        LOG_E("Fatfs mount fail,return (%d)\r\n",res);
        LOG_E("may due to:SPI Flash init fail\r\n");
        while (1);
    }
    else
    {
        LOG_I("FatFs mount success!!!\r\n");
    }
    
    
    
    

    res = f_open(&fp,"0:readme.txt",FA_CREATE_ALWAYS | FA_WRITE );
    if ( res == FR_OK )
    {
        LOG_I("open/create 0:readme.txt success!!! write character to file...\r\n");
        /* ��ָ���洢������д�뵽�ļ��� */
        res=f_write(&fp,writeBuffer,sizeof(writeBuffer),&fnum);
        if (res==FR_OK)
        {
            LOG_I("write success,writed data length:%d\n",fnum);
            LOG_I("data write to file:\r\n%s\r\n",writeBuffer);
        }
        else
        {
            LOG_I("file writing fail(%d)\r\n",res);
        }
        /* ���ٶ�д���ر��ļ� */
        f_close(&fp);
    }
    else
    {
        LOG_E("open/creat file fail!\r\n");
    }


    
    res = f_open(&fp,"0:readme.txt",FA_OPEN_EXISTING | FA_READ);
    if (res == FR_OK)
    {
        LOG_I("file open success!!!\r\n");
        res = f_read(&fp, readBuffer, sizeof(readBuffer), &fnum);
        if (res==FR_OK)
        {
            LOG_I("file read success,get data lemgth:%d\r\n",fnum);
            LOG_I("data get from file is:\r\n%s \r\n", readBuffer);
        }
        else
        {
            LOG_E("file read fail,return (%d)\r\n",res);
        }
    }
    else
    {
        LOG_E("file open fail!!!\r\n");
    }
    
    /* ���ٶ�д���ر��ļ� */
    f_close(&fp);

    /* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
    f_mount(NULL,"0:",1);    
#endif    
    
    
    for(;;)
    {
    }
}



