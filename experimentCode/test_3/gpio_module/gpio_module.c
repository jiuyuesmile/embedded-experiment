/*
 * gpio_module.c
 *
 *  Created on: 2016-11-15
 *      Author: THTF
 */

#define  __SYLIXOS_STDIO
#define  __SYLIXOS_KERNEL
#include <SylixOS.h>
#include <module.h>
/*********************************************************************************************************
** 函数名称: GpioIsr
** 功能描述: GPIO 的中断响应函数
** 输　入  : gpioNum 需要设置的 GPIO 引脚编号
**           ulVector          中断向量号
** 输　出  : 中断返回值
** 全局变量:
** 调用模块:
*********************************************************************************************************/
/*
	 * GPIO LED A8测试，D601 = 136(faile)、D602 = 137(OK)、D603 = 138(OK)、D604 = 51(OK)、D605 = 52(OK);
	   GPIO LED A9测试，LED6 = 228(OK)、LED7 = 267(OK)、LED8 = 274(OK), 高电平 LED 亮;
	 */
	/*
	 * GPIO 按键A8读取，Key6 = 155(OK)、Key7 = 156(OK)、Key8 = 157(OK)、Key9 = 158(OK)、Key0 = 159(OK);
	   GPIO 按键A9读取，Key4 = 272(OK)、Key5 = 273(OK)、Key6 = 276(OK)、Key7 = 277(OK) ;
	 */

static irqreturn_t  GpioIsr (INT iGpioNum, ULONG  ulVector)
{
    irqreturn_t		irqReturn;

	irqReturn = API_GpioSvrIrq(iGpioNum);                               /*  判断是否该 GPIO 触发的中断  */
    if (LW_IRQ_RETVAL(irqReturn)) {

        API_GpioClearIrq(iGpioNum);                                     /*  清除中断                    */
        printk("button is pressed!\n");
    }

    return  (irqReturn);
}
/*********************************************************************************************************
** 函数名称: __keyThread
** 功能描述: KEY thread
** 输　入  : NONE
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static PVOID  __keyThread (PVOID  pvArg)
{

	INT  iError;
	INT  iIrqNum;
	INT  iKeyGpioNum = 272;                                            	/*  GPIO KEY 的编号             */




    /*
     * 申请 KEY 的 GPIO
     */
    iError = API_GpioRequestOne(iKeyGpioNum, LW_GPIOF_IN, "KEY");
	if (iError != ERROR_NONE) {
		printk("failed to request gpio %d!\n", iKeyGpioNum);
        return  (NULL);
	}

	/*
	 * 获得 KEY 的 GPIO 的中断号，配置为下降沿触发中断
	 */
	iIrqNum = API_GpioSetupIrq(iKeyGpioNum, LW_FALSE, 0);
    if (iIrqNum == PX_ERROR) {
    	printk("failed to setup gpio %d irq!\n", iKeyGpioNum);
        return  (NULL);
    }

	/*
	 * 绑定 KEY 的 GPIO 的中断响应函数
	 */
	iError = API_InterVectorConnect((ULONG)iIrqNum,
									(PINT_SVR_ROUTINE)GpioIsr,
									(PVOID)iKeyGpioNum,
									"GpioIsr");
	if (iError != ERROR_NONE) {
		printk("failed to connect GpioIsr!\n");
        return  (NULL);
	}

	/*
	 * 使能  KEY 的 GPIO 的中断
	 */
	API_InterVectorEnable(iIrqNum);

	API_TimeSSleep(60);
	/*
	 * 释放 GPIO
	 */
	API_InterVectorDisconnect((ULONG)iIrqNum,
							   (PINT_SVR_ROUTINE)GpioIsr,
							   (PVOID)iKeyGpioNum);

	API_GpioFree(iKeyGpioNum);

    return  (NULL);
}
/*********************************************************************************************************
** 函数名称: module_init
** 功能描述: 模块安装函数
** 输　入  : NONE
** 输　出  : 错误号
** 全局变量:
** 调用模块:
*********************************************************************************************************/
void module_init (void)
{
	printk("gpio_module init!\n");

	API_ThreadCreate("t_Key", (PTHREAD_START_ROUTINE)__keyThread, LW_NULL, LW_NULL);
}
/*********************************************************************************************************
** 函数名称: module_exit
** 功能描述: 模块退出函数
** 输　入  : NONE
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
void module_exit (void)
{
	printk("gpio_module exit!\n");
}
/*********************************************************************************************************
  END
*********************************************************************************************************/

