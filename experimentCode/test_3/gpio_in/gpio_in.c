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
 * GPIO 按键A9读取，Key4 = 272(OK)、Key5 = 273(OK)、Key6 = 276(OK)、Key7 = 277(OK) ;
 */

/*
 * GPIO 按键A8读取，Key6 = 155(OK)、Key7 = 156(OK)、Key8 = 157(OK)、Key9 = 158(OK)、Key0 = 159(OK);
 */

/*********************************************************************************************************
** 函数名称: main
** 功能描述: 程序主函数
** 输　入  : 无
** 输　出  : 无
** 全局变量:
** 调用模块:
*********************************************************************************************************/
int  main (int  argc, char  *argv[])
{
	int      ret;
    fd_set   fdset;
    uint8_t  value;
    int      key_num  = 155;                                            /*  GPIO KEY 的编号             */
    int      keyfd;

    /*
     * 打开 KEY 的 GPIO 文件，配置为下降沿触发中断
     */
    keyfd = gpiofd(key_num, O_RDONLY, GPIO_FLAG_TRIG_FALL);
    if (keyfd < 0) {
        printf("gpiofd create error!\n");
        return  (-1);
    }

    /*
     * 不断地调用 select 等待按键被按下
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
