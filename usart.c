/*******************************************************************************
*
*	Modify Record:
*	  <date>		 <author>		<version>		<desc>
*	--------------------------------------------------------------------------
*	2012/08/08		sunhaifeng		  v1.0			Create
*
*
*
*******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// 头文件
#include "usart.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>



////////////////////////////////////////////////////////////////////////////////
// 局部宏定义

////////////////////////////////////////////////////////////////////////////////
// 局部数据类型及数据结构定义

////////////////////////////////////////////////////////////////////////////////
// 全局变量

////////////////////////////////////////////////////////////////////////////////
// 局部变量

////////////////////////////////////////////////////////////////////////////////
// 局部函数声明

/*******************************************************************************
*
*	@brief		打开usart设备
*
*	@param[in]	pdev_name用于传入要打开的usart设备路径名
*	@param[out]	psp_fd用于传出要保存的设备描述符的指针
*
*	@retval		失败返回FAIL 成功返回SUCCESS
*
*	@note
*
*******************************************************************************/
status_t open_usart(const char *pdev_name, s32_t *psp_fd)
{
	s32_t fd = -1;

	fd = open(pdev_name, O_RDWR | O_NOCTTY);	//  | O_NDELAY
	if (-1 == fd)
	{
		*psp_fd = -1;
		return FAIL;
	}

	*psp_fd = fd;

	return SUCCESS;
}

/*******************************************************************************
*
*	@brief		配置usart属性
*
*	@param[in]	sp_fd用于传入设备描述符
*	@param[in]	band_rate用于传入波特率
*	@param[in]	data_bits用于传入数据位
*	@param[in]	odd_even用于传入奇偶校验
*	@param[in]	stop_bits用于传入停止位
*
*	@retval		失败返回FAIL 成功返回SUCCESS
*
*	@note
*
*******************************************************************************/
status_t set_usart_attr(s32_t sp_fd, u32_t band_rate, u08_t data_bits, u08_t odd_even, u08_t stop_bits)
{
	struct termios opt;

	if (-1 == sp_fd)
	{
		return FAIL;
	}

	bzero(&opt, sizeof(struct termios));

	// 获取当前配置
	if (-1 == tcgetattr(sp_fd, &opt))
	{
		return FAIL;
	}

	opt.c_cflag |= CLOCAL | CREAD;

	opt.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	opt.c_oflag &= ~OPOST;
	opt.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);

	// 设置波特率
	switch (band_rate)
	{
	case 600:
		if (-1 == cfsetospeed(&opt, B600))
		{
			return FAIL;
		}
		if (-1 == cfsetispeed(&opt, B600))
		{
			return FAIL;
		}
		break;
	case 1200:
		if (-1 == cfsetospeed(&opt, B1200))
		{
			return FAIL;
		}
		if (-1 == cfsetispeed(&opt, B1200))
		{
			return FAIL;
		}
		break;
	case 2400:
		if (-1 == cfsetospeed(&opt, B2400))
		{
			return FAIL;
		}
		if (-1 == cfsetispeed(&opt, B2400))
		{
			return FAIL;
		}
		break;
	case 4800:
		if (-1 == cfsetospeed(&opt, B4800))
		{
			return FAIL;
		}
		if (-1 == cfsetispeed(&opt, B4800))
		{
			return FAIL;
		}
		break;
	case 9600:
		if (-1 == cfsetospeed(&opt, B9600))
		{
			return FAIL;
		}
		if (-1 == cfsetispeed(&opt, B9600))
		{
			return FAIL;
		}
		break;
	case 19200:
		if (-1 == cfsetospeed(&opt, B19200))
		{
			return FAIL;
		}
		if (-1 == cfsetispeed(&opt, B19200))
		{
			return FAIL;
		}
		break;
	case 38400:
		if (-1 == cfsetospeed(&opt, B38400))
		{
			return FAIL;
		}
		if (-1 == cfsetispeed(&opt, B38400))
		{
			return FAIL;
		}
		break;
	case 57600:
		if (-1 == cfsetospeed(&opt, B57600))
		{
			return FAIL;
		}
		if (-1 == cfsetispeed(&opt, B57600))
		{
			return FAIL;
		}
		break;
	case 115200:
		if (-1 == cfsetospeed(&opt, B115200))
		{
			return FAIL;
		}
		if (-1 == cfsetispeed(&opt, B115200))
		{
			return FAIL;
		}
		break;
	default:
		if (-1 == cfsetospeed(&opt, B115200))
		{
			return FAIL;
		}
		if (-1 == cfsetispeed(&opt, B115200))
		{
			return FAIL;
		}
		break;
	}

	// 设置数据位
	switch (data_bits)
	{
	case 5:
		opt.c_cflag &= ~CSIZE;
		opt.c_cflag |= CS5;
		break;
	case 6:
		opt.c_cflag &= ~CSIZE;
		opt.c_cflag |= CS6;
		break;
	case 7:
		opt.c_cflag &= ~CSIZE;
		opt.c_cflag |= CS7;
		break;
	case 8:
	default:
		opt.c_cflag &= ~CSIZE;
		opt.c_cflag |= CS8;
		break;
	}

	// 设置奇偶校验位
	switch (odd_even)
	{
	case 'n':
	case 'N':
		//opt.c_cflag &= ~INPCK;
		opt.c_cflag &= ~PARENB;
		break;
	case 'o':
	case 'O':
		opt.c_cflag |= INPCK;
		opt.c_cflag |= PARENB;
		opt.c_cflag |= PARODD;
		break;
	case 'e':
	case 'E':
		opt.c_cflag |= INPCK;
		opt.c_cflag |= PARENB;
		opt.c_cflag &= ~PARODD;
		break;
	default:
		//opt.c_cflag &= ~INPCK;
		opt.c_cflag &= ~PARENB;
		break;
	}
	// 设置停止位
	switch (stop_bits)
	{
	case 1:
		opt.c_cflag &= ~CSTOPB;
		break;
	case 2:
		opt.c_cflag |= CSTOPB;
		break;
	default:
		opt.c_cflag &= ~CSTOPB;
		break;
	}

	if (-1 == tcflush(sp_fd, TCIOFLUSH))
	{
		return FAIL;
	}

	// 设置当前配置
	if (-1 == tcsetattr(sp_fd, TCSANOW, &opt))
	{
		return FAIL;
	}

	return SUCCESS;
}

/*******************************************************************************
 * 函数名:close_fd
 * 功能描述:关闭串口
 * 参数:s32_t psp_fd
 * 被调用:
 * 返回值:void
 ******************************************************************************/
void close_fd(s32_t psp_fd)
{
    if ( psp_fd >0 )
    {
        close( psp_fd );
    }
}   /*-------- end close_fd -------- */
