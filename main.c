/*******************************************************************************
 * 文件名:main.c
 * 文件描述:主函数
 * 创建日期:2018/04/18 10:02:42
 * 版本：Ver1.0
 *
 * Copyright © 2018 - 2018 mengqp.
 *
 * 历史版本：
 * 修改人                修改时间                修改版本        修改内容
 *
 *
 ******************************************************************************/
#include "up.h"
#include "download.h"
#include "datatype.h"

int main(int argc, char *argv[])
{
    if (SUCCESS != _start_download())
    {
        printf("start download error\n");
    }

    if (SUCCESS != _start_upload())
    {
        printf("start up error \n");
    }

    while (1)
    {
        sleep( 10 );
    }
    return 0;
}
