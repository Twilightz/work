#ifndef __MPU_POWER_H
#define __MPU_POWER_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "stm32f10x.h"



#define MPU_RESET_Set        			GPIO_SetBits(GPIOC, GPIO_Pin_5); 	 //设置PC5为1,EC20 RESET_N 为0
#define MPU_RESET_Clr        			GPIO_ResetBits(GPIOC, GPIO_Pin_5)	 //设置PC5为0,EC20 RESET_N 为1

#define MPU_PWRKEY_ENABEL()	  	GPIO_SetBits(GPIOB, GPIO_Pin_0);
#define MPU_PWRKEY_DISABLE()	 	GPIO_ResetBits(GPIOB, GPIO_Pin_0);

#define ENABLE_4G_4V()				GPIO_SetBits(GPIOA, GPIO_Pin_12);
#define DISABLE_4G_4V()				GPIO_ResetBits(GPIOA, GPIO_Pin_12);

#define ENABLE_4G_1V8SW()			
#define DISABLE_4G_1V8SW()			


#define ENABLE_4G_3V3SW()			
#define DISABLE_4G_3V3SW()			

#define ENABLE_4G_RF()				
#define DISABLE_4G_RF()				

#define ENABLE_4G_GPS()				
#define DISABLE_4G_GPS()				

#define ENABLE_4G_MIC()				
#define DISABLE_4G_MIC()				


enum
{
    _MPU_POWER_OFF,
    _MPU_POWER_EN,
    _MPU_POWER_ON,	
    _MPU_WAIT_POWERON_FINISH,
    _MPU_WAIT_PORT_STABLE,  //等待mpu的串口和usb稳定
    _MPU_RUNNING_OK,
    _MPU_RESET_START,
    _MPU_RESET_END,
    _MPU_POWEROFF_START,
    _MPU_POWEROFF_DELAY,
    _MPU_POWEROFF_ERROR
};
void KDT_gpio_init(void);
void Mpu_Power_Manage(void) ; //   (250+15+5)*12 = 3240;

#ifdef __cplusplus
}
#endif

#endif /* __MPU_POWER_H */ 
