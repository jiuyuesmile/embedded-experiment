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
** ��������: GpioIsr
** ��������: GPIO ���ж���Ӧ����
** �䡡��  : gpioNum ��Ҫ���õ� GPIO ���ű��
**           ulVector          �ж�������
** �䡡��  : �жϷ���ֵ
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
/*
	 * GPIO LED A8���ԣ�D601 = 136(faile)��D602 = 137(OK)��D603 = 138(OK)��D604 = 51(OK)��D605 = 52(OK);
	   GPIO LED A9���ԣ�LED6 = 228(OK)��LED7 = 267(OK)��LED8 = 274(OK), �ߵ�ƽ LED ��;
	 */
	/*
	 * GPIO ����A8��ȡ��Key6 = 155(OK)��Key7 = 156(OK)��Key8 = 157(OK)��Key9 = 158(OK)��Key0 = 159(OK);
	   GPIO ����A9��ȡ��Key4 = 272(OK)��Key5 = 273(OK)��Key6 = 276(OK)��Key7 = 277(OK) ;
	 */

static irqreturn_t  GpioIsr (INT iGpioNum, ULONG  ulVector)
{
    irqreturn_t		irqReturn;

	irqReturn = API_GpioSvrIrq(iGpioNum);                               /*  �ж��Ƿ�� GPIO �������ж�  */
    if (LW_IRQ_RETVAL(irqReturn)) {

        API_GpioClearIrq(iGpioNum);                                     /*  ����ж�                    */
        printk("button is pressed!\n");
    }

    return  (irqReturn);
}
/*********************************************************************************************************
** ��������: __keyThread
** ��������: KEY thread
** �䡡��  : NONE
** �䡡��  : NONE
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
static PVOID  __keyThread (PVOID  pvArg)
{

	INT  iError;
	INT  iIrqNum;
	INT  iKeyGpioNum = 272;                                            	/*  GPIO KEY �ı��             */




    /*
     * ���� KEY �� GPIO
     */
    iError = API_GpioRequestOne(iKeyGpioNum, LW_GPIOF_IN, "KEY");
	if (iError != ERROR_NONE) {
		printk("failed to request gpio %d!\n", iKeyGpioNum);
        return  (NULL);
	}

	/*
	 * ��� KEY �� GPIO ���жϺţ�����Ϊ�½��ش����ж�
	 */
	iIrqNum = API_GpioSetupIrq(iKeyGpioNum, LW_FALSE, 0);
    if (iIrqNum == PX_ERROR) {
    	printk("failed to setup gpio %d irq!\n", iKeyGpioNum);
        return  (NULL);
    }

	/*
	 * �� KEY �� GPIO ���ж���Ӧ����
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
	 * ʹ��  KEY �� GPIO ���ж�
	 */
	API_InterVectorEnable(iIrqNum);

	API_TimeSSleep(60);
	/*
	 * �ͷ� GPIO
	 */
	API_InterVectorDisconnect((ULONG)iIrqNum,
							   (PINT_SVR_ROUTINE)GpioIsr,
							   (PVOID)iKeyGpioNum);

	API_GpioFree(iKeyGpioNum);

    return  (NULL);
}
/*********************************************************************************************************
** ��������: module_init
** ��������: ģ�鰲װ����
** �䡡��  : NONE
** �䡡��  : �����
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
void module_init (void)
{
	printk("gpio_module init!\n");

	API_ThreadCreate("t_Key", (PTHREAD_START_ROUTINE)__keyThread, LW_NULL, LW_NULL);
}
/*********************************************************************************************************
** ��������: module_exit
** ��������: ģ���˳�����
** �䡡��  : NONE
** �䡡��  : NONE
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
void module_exit (void)
{
	printk("gpio_module exit!\n");
}
/*********************************************************************************************************
  END
*********************************************************************************************************/

