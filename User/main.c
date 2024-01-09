#include "sys.h"
#include "systick.h"
#include "usart.h"
#include "logs.h"
#include "W25QXX.h"
#include "ff.h"


//系统文件句柄
FATFS fs;    
//只有格式化的时候要用work
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
        /* 格式化 */
        res=f_mkfs("0:",0,work,sizeof work);

        if (res == FR_OK)
        {
            LOG_I("FLASH formate success!!!\r\n");
            /* 格式化后，先取消挂载 */
            res = f_mount(NULL,"0:",1);
            /* 重新挂载 */
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
        /* 将指定存储区内容写入到文件内 */
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
        /* 不再读写，关闭文件 */
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
    
    /* 不再读写，关闭文件 */
    f_close(&fp);

    /* 不再使用文件系统，取消挂载文件系统 */
    f_mount(NULL,"0:",1);    
#endif    
    
    
    for(;;)
    {
    }
}



