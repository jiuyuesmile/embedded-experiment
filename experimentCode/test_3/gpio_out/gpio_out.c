/*
 * gpio_out.c
 *
 *  Created on: 2016-11-15
 *      Author: THTF
 */

#include <sys/gpiofd.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
/*
 *
 * GPIO LED A9���ԣ�LED6 = 228(OK)��LED7 = 267(OK)��LED8 = 274(OK), �ߵ�ƽ LED ��;
 * GPIO LED A8���ԣ�D601 = 136(failed)��D602 = 137(OK)defaut��D603 = 138(OK)��D604 = 51(OK)��D605 = 52(OK);

 */
/*********************************************************************************************************
** ��������: main
** ��������: ����������
** �䡡��  : ��
** �䡡��  : ��
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
int  main (int  argc, char  *argv[])
{
	int  	led_num = 137;
	int  	led_num1 = 138;
	int  	i;
	int  	fd,fd1;

    /*
     * �� GPIO �ļ�
     */
	fd = gpiofd(led_num, O_WRONLY, GPIO_FLAG_OUT_INIT_LOW);
	if (fd < 0) {
		printf("failed to open gpio %d\n", led_num);
		return  (-1);
	}
	fd1 = gpiofd(led_num1, O_WRONLY, GPIO_FLAG_OUT_INIT_LOW);
	if (fd1 < 0) {
		printf("failed to open gpio %d\n", led_num1);
		return  (-1);
	}

    for (i = 0; i < 60; i++) {
        /*
         * �� LED ������˸��ÿ�θߵ͵�ƽ����ǰ��ʹ�� usleep ������ʱ 500ms��
         */
    	gpiofd_write(fd, 0);
    	gpiofd_write(fd1, 1);
        usleep(500 * 1000);

        gpiofd_write(fd, 1);
    	gpiofd_write(fd1, 0);
    	usleep(500 * 1000);
    }

    /*
     * �ر��豸
     */
    close(fd);
    close(fd1);

	return  (0);
}
/*********************************************************************************************************
  END
*********************************************************************************************************/
