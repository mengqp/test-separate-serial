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
// ͷ�ļ�
#include "usart.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>



////////////////////////////////////////////////////////////////////////////////
// �ֲ��궨��

////////////////////////////////////////////////////////////////////////////////
// �ֲ��������ͼ����ݽṹ����

////////////////////////////////////////////////////////////////////////////////
// ȫ�ֱ���

////////////////////////////////////////////////////////////////////////////////
// �ֲ�����

////////////////////////////////////////////////////////////////////////////////
// �ֲ���������

/*******************************************************************************
*
*	@brief		��usart�豸
*
*	@param[in]	pdev_name���ڴ���Ҫ�򿪵�usart�豸·����
*	@param[out]	psp_fd���ڴ���Ҫ������豸��������ָ��
*
*	@retval		ʧ�ܷ���FAIL �ɹ�����SUCCESS
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
*	@brief		����usart����
*
*	@param[in]	sp_fd���ڴ����豸������
*	@param[in]	band_rate���ڴ��벨����
*	@param[in]	data_bits���ڴ�������λ
*	@param[in]	odd_even���ڴ�����żУ��
*	@param[in]	stop_bits���ڴ���ֹͣλ
*
*	@retval		ʧ�ܷ���FAIL �ɹ�����SUCCESS
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

	// ��ȡ��ǰ����
	if (-1 == tcgetattr(sp_fd, &opt))
	{
		return FAIL;
	}

	opt.c_cflag |= CLOCAL | CREAD;

	opt.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	opt.c_oflag &= ~OPOST;
	opt.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);

	// ���ò�����
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

	// ��������λ
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

	// ������żУ��λ
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
	// ����ֹͣλ
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

	// ���õ�ǰ����
	if (-1 == tcsetattr(sp_fd, TCSANOW, &opt))
	{
		return FAIL;
	}

	return SUCCESS;
}

/*******************************************************************************
 * ������:close_fd
 * ��������:�رմ���
 * ����:s32_t psp_fd
 * ������:
 * ����ֵ:void
 ******************************************************************************/
void close_fd(s32_t psp_fd)
{
    if ( psp_fd >0 )
    {
        close( psp_fd );
    }
}   /*-------- end close_fd -------- */
