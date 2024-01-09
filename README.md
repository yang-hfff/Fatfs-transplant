# FatFs移植笔记 

## 1. 裁剪 

> ffconf.h

|       宏       |         含义         |
| :------------: | :------------------: |
|  FF_USE_MKFS   |    格式化功能选择    |
|  FF_CODE_PAGE  |     语言功能选择     |
|   FF_USE_LFN   |     长文件名支持     |
|   FF_VOLUMES   |     逻辑设备数量     |
|   FF_MIN_SS    | 指定扇区大小的最小值 |
|   FF_MAX_SS    | 指定扇区大小的最大值 |
|  FF_FS_NORTC   |      不使用RTC       |

## 2.底层驱动函数

>diskio.c 

- 定义设备号

```C
#define DEV_FLASH	0
#define ATA			1
#define TF			2
#define SD			3
//......
```

- 补充驱动函数

  |     函数名      |        功能        |
  | :-------------: | :----------------: |
  |   disk_status   |    设备状态获取    |
  | disk_initialize |     设备初始化     |
  |    disk_read    |      读取扇区      |
  |   disk_write    |      扇区写入      |
  |   disk_ioctl    |      其他控制      |
  |   get_fattime   | 时间戳获取（可选） |

  

## 3.挂载与格式化

```C
//...
#include "ff.h"

FATFS fs;    
BYTE work[FF_MAX_SS]; 


int main(void)
{    
	//...
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

    /* 不再使用文件系统，取消挂载文件系统 */
    f_mount(NULL,"0:",1);      
    
    
    for(;;)
    {
    }
}



```