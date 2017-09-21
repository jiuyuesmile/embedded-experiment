#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
/*********************************************************************************************************
** ��������: set_speed
** ��������: ���ô��ڲ�����
** �䡡��  : fd    ���ڲ������
** ��  ��  : speed �������õĲ�����
** �䡡��  : NONE
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
int  set_speed (int  fd, int  speed)
{

    int            	ret;
    struct termios 	opt;

    ret = tcgetattr(fd, &opt);                         	            		/*��ȡ���ڵ� termios ��Ϣ   */
    if (ret !=  0) {
         printf("Get Serial speed failed");
         return (1);
    }
   tcflush(fd, TCIOFLUSH);                             	               	/*  ˢ�������������           */

   cfsetispeed(&opt, speed);               	           	              	/*  �������벨����              */
   cfsetospeed(&opt, speed);               	            	            	/*  �������������              */
   /* ���������ն˵���ز���, TCSANOW �������ݴ�����Ͼ������ı�����	 */
   ret = tcsetattr(fd, TCSANOW, &opt);
   if (ret != 0) {
        printf("Setup Serial speed failed");
        return (2);
   }
   return (0);
}
/*********************************************************************************************************
** ��������: set_Parity
** ��������: ���ô��ڵ�ͨ�Ų���
** �䡡��  : fd        ���ڲ������
** ��  ��  : databits  ����λ
** ��  ��  : stopbits  ֹͣλ
** ��  ��  : parity    У��λ
** ����  ֵ: 0 ���óɹ���-1 ����ʧ��
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
int  set_parity (int  fd, int  databits, int  stopbits, int  parity)
{
	int             ret;
	struct termios  options;

	ret = tcgetattr(fd, &options);
	if (ret !=  0) {
		printf("Setup Serial failed");
		return  (1);
	}

	options.c_cflag &= ~CSIZE;
	switch (databits) {                                                 /*  ��������λ��                */

	case 5:
		options.c_cflag |= CS5;
		break;

	case 6:
		options.c_cflag |= CS6;
		break;

	case 7:
		options.c_cflag |= CS7;
		break;

	case 8:
		options.c_cflag |= CS8;
		break;

	default:
		printf("Unsupported data size\n");
		return  (2);
	}

	switch (parity) {                                                   /*  ����У�鷽ʽ                */

	case 'n':
	case 'N':
		options.c_cflag &= ~PARENB;
		options.c_iflag &= ~INPCK;
		break;

	case 'o':
	case 'O':
		options.c_cflag |= (PARODD | PARENB);
		options.c_iflag |= INPCK;
		break;

	case 'e':
	case 'E':
		options.c_cflag |= PARENB;
		options.c_cflag &= ~PARODD;
		options.c_iflag |= INPCK;
		break;

	default:
		printf("Unsupported parity\n");
		return  (3);
	}

	switch (stopbits) {	                                                /* ����ֹͣλ                   */

	case 1:
		options.c_cflag &= ~CSTOPB;
		break;

	case 2:
		options.c_cflag |= CSTOPB;
		break;

	default:
		printf("Unsupported stop bits\n");
		return  (4);
	}

	tcflush(fd, TCIOFLUSH);                                              /* ��������շ�����ȥ           */
	/*
	 * ���������ն˵���ز���, TCSANOW �������ݴ�����Ͼ������ı�����
	 */
	ret = tcsetattr(fd, TCSANOW, &options);
	if (ret != 0) {
		printf("Setup Serial failed");
		return  (5);
	}
	return  (0);
}
/*********************************************************************************************************
** ��������: main
** ��������: ����������
** �䡡��  : �豸��,ʹ�÷�ʽ ./uart_test /dev/ttyS1
** �䡡��  : ��
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
int  main (int  argc, char  *argv[])
{
	int       ret;
	int       nread;
	int       fd_tty;
	char      buff[512];

	/*
	 * ��������С��2�򷵻�
	 */
	if (argc < 2) {
		printf("useage: %s dev\n", argv[0]);
		return  (-1);
	}

	fd_tty = open(argv[1], O_RDWR);                                     /*  �򿪴���                    */
	if (fd_tty < 0) {
		printf("open device %s faild\n", argv[1]);
		return  (-1);
	}

	ret = set_speed(fd_tty, 115200);                                 /*  ���ô��ڲ�����              */
	if (ret != 0) {
		close(fd_tty);
		return  (-1);
	}

	ret = set_parity(fd_tty, 8, 1, 'n');                             /*  ����λ 8��ֹͣλ 1����У��  */
	if (ret != 0) {
		close(fd_tty);
		return  (-1);
	}

	/*
	 * �� while ѭ���У����ϵļ�����ݣ����յ����ݣ�������ͨ���˴��ڷ��ͻ�����
	 */
	while(1) {
		nread = read(fd_tty, buff, 255);
		if (nread > 0) {                                                /*  ��������                    */
			 write(fd_tty, buff, nread);                                /*  �����յ�����ͨ��ԭ�������  */
		}
	}

	close(fd_tty);
	return  (0);
}
/*********************************************************************************************************
  END
*********************************************************************************************************/
