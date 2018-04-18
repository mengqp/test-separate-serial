/*******************************************************************************
 * 文件名:up.c
 * 文件描述：见头文件
 * 创建日期:2018/04/18 10:05:54
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
#include "usart.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include "string.h"

static const char RS485_DEV_NAME[] = "/dev/ttyO4";
static const u32_t BAND_RATE = 9600;
static const u08_t DATA_BITS = 8;
static const u08_t ODD_EVEN = 'e';
static const u08_t STOP_BITS = 1;

static const u16_t SELECT_TIMEOUT = 20000;

static s32_t gs_rs485_fd = -1;

/*******************************************************************************
 * 函数名:get_send_buf
 * 功能描述:获取报文
 * 参数:u08_t *pbuf
 * 参数:u32_t len
 * 被调用:
 * 返回值:static status_t
 ******************************************************************************/
static status_t  get_send_buf(u08_t *pbuf , u32_t *plen )
{
    int i;
    int len = 0;

    for (i = 0; i < 230; i++)
    {
        pbuf[i] = (u08_t )i;
        len ++;
    }

    *plen = len;

    return SUCCESS;
}   /*-------- end get_send_buf -------- */

/*******************************************************************************
 * 函数名:write_data
 * 功能描述: 串口写数据
 * 参数: s32_t rs485_fd, u08_t *pdata_bfr,
 * 返回值:void
 ******************************************************************************/
static void write_data( s32_t rs485_fd, u08_t *pdata_bfr, u08_t data_len, u32_t band_rate)
{
    tcflush( rs485_fd, TCOFLUSH );
    write(gs_rs485_fd, pdata_bfr, data_len);
    usleep( data_len * 11 * 1000000 /band_rate);
}   /*-------- end write_data -------- */

/*******************************************************************************
 * 函数名:_do_analysis_proc
 * 功能描述:接收处理
 * 参数:u08_t *pdata_bfr
 * 参数:u08_t rd_bytes
 * 被调用:
 * 返回值:static status_t
 ******************************************************************************/
static status_t _do_analysis_proc(u08_t *pdata_bfr, u08_t rd_bytes)
{
    /* 此处可以自己设置某种规则检查错误 */

    /* 我自己设置的规则 */
    int i;
    for (i = 0; i < rd_bytes; i++)
    {
        if ( pdata_bfr[i] != i )
        {
            printf("up analysis error byte%d!=%d\n", i, pdata_bfr[i]);
        }
    }


    return SUCCESS;
}   /*-------- end _do_analysis_proc(pdata_bfr, (u08_t)rd_bytes); -------- */


static void receive_and_parse(u08_t *pdata_bfr, s32_t rs485_fd)
{
    fd_set rfds;
    struct timeval tv, tvtimeout;
    s32_t ret_val = -1;
    s32_t rd_bytes = 0;
    char tmp_ch = 0;
	usleep( 16 * 11 * 1000000 /BAND_RATE + SELECT_TIMEOUT * 2 );

    FD_ZERO(&rfds);
    FD_SET(rs485_fd, &rfds);
    tvtimeout.tv_sec = 0;
    tvtimeout.tv_usec = SELECT_TIMEOUT * 8;

    ret_val = select(rs485_fd + 1, &rfds, NULL, NULL, &tvtimeout);
    if (-1 == ret_val)
    {
        perror("select");
    }
    else if (0 == ret_val)
    {
        printf("up丢帧\n");
    }
    else
    {
        if (0 != read(rs485_fd, &tmp_ch, 1))
        {
            pdata_bfr[rd_bytes++] = tmp_ch;
        }

        while (1)
        {
            // 发送数据然后接收数据
            FD_ZERO(&rfds);
            FD_SET(rs485_fd, &rfds);
            tv.tv_sec = 0;

			tv.tv_usec = 40 * 11 * 1000000 /BAND_RATE + SELECT_TIMEOUT ;


            ret_val = select(rs485_fd + 1, &rfds, NULL, NULL, &tv);
            if (-1 == ret_val)
            {
                perror("upselect");
            }
            else if (0 == ret_val)
            {
                if ( (rd_bytes < 256) )
                {
                    // 解析
                    _do_analysis_proc(pdata_bfr, (u08_t)rd_bytes);
                }

                break;
            }
            else
            {

                if (0 != read(rs485_fd, &tmp_ch, 1))
                {
                    pdata_bfr[rd_bytes++] = tmp_ch;
                }
            }
        }
    }

    tcflush( rs485_fd, TCIFLUSH );
}

/*******************************************************************************
 * 函数名:_download_init
 * 功能描述:初始化函数
 * 参数:void
 * 被调用:download_func
 * 返回值:static status_t
 ******************************************************************************/
static status_t  _download_init(void)
{
    s32_t rs485_fd = -1;

    if (SUCCESS != open_usart((char *)RS485_DEV_NAME, &rs485_fd))
    {
        return FAIL;
    }

    set_usart_attr(rs485_fd, BAND_RATE, DATA_BITS, ODD_EVEN, STOP_BITS);

    gs_rs485_fd = rs485_fd;
    return SUCCESS;
}   /*-------- end _download_init -------- */


/*******************************************************************************
 * 函数名:upload_func
 * 功能描述:上传数据的功能函数
 * 参数:void
 * 被调用:_start_upload
 * 返回值:void
 ******************************************************************************/
static void upload_func (void)
{
    u08_t buf[256];
    u32_t len = 0;
    if (SUCCESS != _download_init())
    {
        return ;
    }
    while ( 1 )
    {
        /* 获取buf */
        get_send_buf( buf, &len );

        /* 发送 */
        write_data(gs_rs485_fd, buf, len, BAND_RATE );
        memset( buf, 0, 256 );

        /* 接收处理 */
        receive_and_parse(buf ,gs_rs485_fd);
        memset( buf, 0, 256 );
    }

    close_usart( gs_rs485_fd );
}   /*-------- end upload_func  -------- */

/*******************************************************************************
 * 函数名:_start_upload
 * 功能描述:启动上行链路进程
 * 参数:void
 * 被调用:失败返回FAIL 成功返回SUCCESS
 * 返回值:status_t
 ******************************************************************************/
status_t  _start_upload(void)
{
    pid_t pid = -1;

    pid = fork();
    if (-1 == pid)
    {
        return FAIL;
    }
    else if (0 == pid)
    {
        upload_func();
    }
    else
    {
    }

    return SUCCESS;

}   /*-------- end _start_upload -------- */
