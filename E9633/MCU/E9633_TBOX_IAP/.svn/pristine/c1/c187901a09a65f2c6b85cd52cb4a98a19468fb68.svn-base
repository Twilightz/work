
#include "Shell.h"
#include "MPU_power.h"



//#define MPU_RESET_INTERVAL     5
#define MPU_RESET_INTERVAL     15 // 15*12ms=180ms
#define MPU_DELAY_INTERVAL     250 //250*12ms=3000mS  
#define MPU_POWER_INTERVAL	   15  //15*12ms=180ms
#define MPU_POWERON_SUCCESS_INTERVAL		250 //250////  powerKey =1   ? status = 0 (>=2.5S)   250*12ms + 15*12ms  ??????  test:5--false  200 --faild   210 --faild 250---success
#define MPU_WAIT_POWER_STABLE_INTERVAL	 5  //  5*12ms = 60ms  
#define MPU_WAIT_PORT_STABLE_INTERVAL	 1000//1000 // (>=13S)1000*12 +3240ms==15240ms
#define MPU_POWERON_MAX_TIMES	3   

uint8_t EC20Status = 0;
static uint16_t Mpu_Power_Timer;
uint16_t Mpu_Power_State = _MPU_POWER_OFF;

extern void Mpu_Reset(void)
{
	 //bMpu_Reset = 1;
}

extern int IS_MPU_POWERON(void)
{
	 //return (!GET_BIT(PPR9, 2));
	 return (1);
}

// ARM复位开始
static void MPU_Reset_Start(void)
{

   MPU_RESET_Set; //
   return;
}

// ARM 复位结束
static void MPU_Reset_End(void)
{
   MPU_RESET_Clr;  // 低电平--->ARM端高电平
   //test2=Testlbin;
   return;
}

extern void Mpu_Power_IntoPowerOff(void)
{
	 Mpu_Power_State = _MPU_POWER_OFF;
}

extern int Mpu_Power_Ready(void)
{
   if (Mpu_Power_State==_MPU_RUNNING_OK)
   {
    	return 1;
   }
   return 0;
}

 void Mpu_Power_Manage(void)  //   (250+15+5)*12 = 3240;
{

    static uint16_t MPU_PowerOn_Times = 0;
	 
    switch (Mpu_Power_State)
    {
    	case _MPU_POWER_OFF: 
            Mpu_Power_State=_MPU_POWER_EN;
            Mpu_Power_Timer=0;				
            break;

    	case _MPU_POWER_EN:
						if (Mpu_Power_Timer==0)
						{
								ENABLE_4G_4V();			// 4G_4V_EN
								/*
								ENABLE_4G_1V8SW();		// 1.8VSW_EN
								ENABLE_4G_3V3SW();		// 3.3VSW_EN 
			
								ENABLE_4G_RF();			// 4G_RF_OFF 
			
								ENABLE_4G_GPS();  			// GPS_ON
								ENABLE_4G_MIC();  			//  MIC_EN
								*/
								MPU_PowerOn_Times = 0;
								Mpu_Power_Timer++;
			
						}		
						else if (Mpu_Power_Timer++ >= MPU_WAIT_POWER_STABLE_INTERVAL)//60 ms
						{
								Mpu_Power_State=_MPU_POWER_ON;
								Mpu_Power_Timer=0;
						} 
						else
						;
						break;
		
			case _MPU_POWER_ON:  
						if (Mpu_Power_Timer==0)
						{
								MPU_PWRKEY_ENABEL();  //P0_15 = 1  EC20 Pwrkey =0
								Mpu_Power_Timer++;
								break;
						}
						else if (Mpu_Power_Timer++ >= MPU_POWER_INTERVAL)//180ms
	        	{
	           	 	MPU_PWRKEY_DISABLE();
	           	 	Mpu_Power_State=_MPU_WAIT_POWERON_FINISH;
	            	Mpu_Power_Timer=0;
	            break;
	        	} 
						break;
		
   	 case  _MPU_WAIT_POWERON_FINISH:
						Mpu_Power_Timer++;
		       
						if(Mpu_Power_Timer>=MPU_POWERON_SUCCESS_INTERVAL)// 3s 后
						{
								if(IS_MPU_POWERON())  //检测4G 开机状态位
								{
										RTU_DEBUG("\r\nMPU PowerOn success!");
										Mpu_Power_State = _MPU_WAIT_PORT_STABLE;
                    EC20Status = 1;									
				
								}else // 4G 开机不成功
								{
										RTU_DEBUG("\r\nMPU PowerOn  faild! %d",MPU_PowerOn_Times);
										MPU_PowerOn_Times ++;
										if(MPU_PowerOn_Times >= MPU_POWERON_MAX_TIMES) //尝试超过三次开机
										{
												Mpu_Power_State=_MPU_POWEROFF_ERROR;
												MPU_PowerOn_Times = 0;
					
										}else
										{
												Mpu_Power_State=_MPU_POWER_ON; //重复开机步骤
										}
								}
								Mpu_Power_Timer=0;
					}
					break;

		case _MPU_WAIT_PORT_STABLE:
					Mpu_Power_Timer++;
					if(Mpu_Power_Timer >= MPU_WAIT_PORT_STABLE_INTERVAL)//15240ms
					{
							RTU_DEBUG("\r\nMPU Port Stable! ! ");
							Mpu_Power_Timer=0;
							Mpu_Power_State=_MPU_RUNNING_OK;
					}
					break;
	
	  case _MPU_RUNNING_OK:  //MPU
					break;
  
	  case _MPU_RESET_START:	//执行对MPU复位的动作
					MPU_Reset_Start();
					Mpu_Power_Timer=0;
					Mpu_Power_State=_MPU_RESET_END;
					break;
			
 	 case _MPU_RESET_END:  //等待复位时间达到后，释放复位端口
					Mpu_Power_Timer++;	
       		if (Mpu_Power_Timer<MPU_RESET_INTERVAL)break;
					// 复位时间已有180ms，可以松开复位
					MPU_Reset_End();
				  Mpu_Power_State = _MPU_RUNNING_OK;	
       	  break;
		
	 case _MPU_POWEROFF_START: //MPU模块关机开始
					Mpu_Power_State=_MPU_POWEROFF_DELAY;
					Mpu_Power_Timer = 0;
					break;
	
   case _MPU_POWEROFF_DELAY:
        	Mpu_Power_Timer++;
        	if (Mpu_Power_Timer==MPU_DELAY_INTERVAL)//|| Mpu_Power_Off_Ready())
        	{
           		Mpu_Power_State=_MPU_POWER_OFF;
           		Mpu_Power_Timer=0;
           		break;
       		}
					break;
					
	 case  _MPU_POWEROFF_ERROR: //OPEN 4G ERROR
				 Mpu_Power_Timer++;
				 if(Mpu_Power_Timer>200) /// 2.4S
				 {
						RTU_DEBUG("\r\nMPU Error!! ");
						Mpu_Power_Timer = 0;
				 }
				 break;
	 
   }
   return;
}





