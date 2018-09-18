#ifndef _IOPWR_H_
#define _IOPWR_H_


#define hw_IO_Pwr_Press_timing 	3000
#define hw_IO_Pwr_timer_interval  200


#define hw_IO_POWER_PRESS		key_Press_G28
#define hw_IO_POWER_DIR_SET	key_Pwr_IODirection_G286
#define hw_IO_POWER_ON			key_PwrOn_G86
#define hw_IO_POWER_OFF		key_PwrOff_G86



BOOL key_Press_G28(void);
void key_PwrOn_G86(void);
void key_PwrOff_G86(void);
void key_Pwr_IODirection_G286(void);




#endif 

