/*
 * gpio_in.c
 *
 *  Created on: 2016-11-15
 *      Author: THTF
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/gpiofd.h>

/*
 * GPIO ����A9��ȡ��Key4 = 272(OK)��Key5 = 273(OK)��Key6 = 276(OK)��Key7 = 277(OK) ;
 */

/*
 * GPIO ����A8��ȡ��Key6 = 155(OK)��Key7 = 156(OK)��Key8 = 157(OK)��Key9 = 158(OK)��Key0 = 159(OK);
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
	int      ret;
    fd_set   fdset;
    uint8_t  value;
    int      key_num  = 155;                                            /*  GPIO KEY �ı��             */
    int      keyfd;

    /*
     * �� KEY �� GPIO �ļ�������Ϊ�½��ش����ж�
     */
    keyfd = gpiofd(key_num, O_RDONLY, GPIO_FLAG_TRIG_FALL);
    if (keyfd < 0) {
        printf("gpiofd create error!\n");
        return  (-1);
    }

    /*
     * ���ϵص��� select �ȴ�����������
     */
    FD_ZERO(&fdset);

    while (1) {
        FD_SET(keyfd, &fdset);

        ret = select(keyfd + 1, &fdset, NULL, NULL, NULL);
        if (ret == 1) {
            gpiofd_read(keyfd, &value);
            printf("Key4 is clicked, value : %d\n", value);
            } else if (ret < 0) {
                printf("select error!\n");
                break;
            }
    }
    close(keyfd);
	return  (0);
}
/*********************************************************************************************************
  END
*********************************************************************************************************/
