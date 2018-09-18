1、MCU_WAKE_4G（MCU PB1）--->EC20的WAKEUP_IN（低电平唤醒）
2、4G_ON/OFF（MCU PB0）--->EC20的PWRKEY
3、4G_RST(MCU PC5)--->EC20的RESET_N
4、3V3_EN(MCU PA6)--->打开和关闭G-SENSOR电源
5、DTR_MCU(MCU PA1)--->EC20的MAIN_RTS
6、4G_WAKE_MCU(MCU PA0_WKUP)--->EC20的WAKE_MCU
7、ACC_DET（MCU PC1）
8、CHG_EN（MCU PD2）  功能：PD2为输出引脚。若输出高电平，则打开内置电池充电功能；若输出低电平时，则关闭内置电池充电功能。
9、5V_EN（MCU PC12）
10、4G_4V_EN（MCU PA12） 功能：
11、ACC_ON（MCU PA11）功能：PA11为输出引脚。当ACC OFF后，ACC_DET为低电平。这时，正常情况下系统供电截止。如果这时ACC_ON输出高电平
就能继续驱动TPS54540工作，防止突然断电下的数据丢失。
12、USB_EN(MCU PC9) 功能：

一、开机逻辑

