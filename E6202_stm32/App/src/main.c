/*
********************************************************************************
*  Copyright (C), 2009-2012, ����˼׳�����з�����
*
*  ��Ŀ���ƣ�E9624
*
*  �ļ�����: Main.c
*
*  �ļ�������E9624������main
*
*
*  �� �� ��: zengxf
*
*  �������ڣ�2015-09-18
*
*  �� �� �ţ�V1.0
*
*  �޸ļ�¼��
*
*      1. ��    �ڣ�
*         �� �� �ˣ�
*         �����޸ģ�
*      2. ...
********************************************************************************
*/

#define MAIN_GLOBALS
#include "main.h"
#include "CipherFunc.h"
#include "SysTick.h"
#include "Shell.h"
#include "bsp.h"
#include <string.h>




/*
********************************************************************************
*  �ڲ���������
********************************************************************************
*/
void UserCmd(void);
void CanToUtcp(u8 channel, CANX * pcan, COMX * pcom);
void WinceCmdInit(void);
u8 WinceCmdEntry(u8 dat, COMX * pcom);
void UartDataToCanData(u8 len, u8 *Data);

void GetNameInfo(u32 id);
void SendAddressClaim(u8 channel, u8 sa);
void RequestAddressClaim(void);
int CompareName(u8* name1, u8* name2);
int SelectAddress(int maxAddr);
int CheckReceive(u8 channel, CANQUEDATA* pData);

/*
********************************************************************************
*  �ⲿ��������
********************************************************************************
*/
extern void TIM2_IRQHandler(void);
/*
********************************************************************************
*  �ڲ���������
********************************************************************************
*/
enum { ACC_DEFAULT = 0, ACC_ON, ACC_OFF };
volatile u8 acconoff = ACC_DEFAULT;
u16 Counter = 0;
u16 ad_value;
u8 AsciiBuff[5];
#define VOLTAGE_AVG_NUM 6
static uint32_t battery_voltage_arrary[VOLTAGE_AVG_NUM] = { 0 };
static uint8_t battery_voltage_arrary_index = 0;
u16 batt_lvl_in_milli_volts;
#define THRESHOLD   500  //500mv
#define THRESHOLD_CNT  10
#define power_off_time     30
#define ADC_RESULT_IN_MILLI_VOLTS(ADC_VALUE)\
          ((((ADC_VALUE) * 439)  * 2 )/ 1024)

#define PAGE_ADDR  0x803F800

// add by qq.fan@unistrong.com
static u8 s_SA = 250;
static u8 s_DA_LIST[256];
static u8 MyName[8];
static u32 s_AddressClaimTime = 0;
static u8 s_inWait;

void DelayUS(uint32_t us)
{
	uint32_t i;

	while (us)
	{
		i = 2;
		while (i--);
		us--;
	}
	return;
}

void DelayMS(uint32_t ms)
{
	uint32_t i;

	while (ms)
	{
		i = 1800;
		while (i--);
		ms--;
	}
	return;
}

/*
********************************************************************************
*  ��������: GPIO_Configuration
*
*  ��������: gpio ��ʼ��
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//GPIO_Mode_IN_FLOATING;   
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);*/

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);*/
	//��ʱ�oarm�ϵ�
	DelayMS(1000);
	//can_pwron
	GPIO_SetBits(GPIOC, GPIO_Pin_7);
	//uart_en
	GPIO_SetBits(GPIOC, GPIO_Pin_6);
	//reset
	//GPIO_SetBits(GPIOC, GPIO_Pin_9);
	//acc_on
	GPIO_SetBits(GPIOA, GPIO_Pin_11);
	DelayMS(100);
	//PMU_N_OE_CTL
	GPIO_SetBits(GPIOC, GPIO_Pin_8);
	DelayMS(1000);
	GPIO_ResetBits(GPIOC, GPIO_Pin_8);
	DelayMS(200);
	GPIO_SetBits(GPIOC, GPIO_Pin_8);
}

/*
********************************************************************************
*  ��������: EXTI_Configuration
*
*  ��������: �жϳ�ʼ��
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void EXTI0_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	EXTI_ClearITPendingBit(EXTI_Line0);
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0A;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
#if 0
void EXTI0_IRQHandler(void)
{
	u8 rdata;
	RTU_DEBUG("EXTI0_IRQHandler\r\n");
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line0); /* ����жϱ�־λ */
		//do something
		rdata = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
		if (1 == rdata) {
			acconoff = ACC_ON;
			RTU_DEBUG("EXTI0_IRQHandler:acc on\r\n");
		}
		else {
			acconoff = ACC_OFF;
			RTU_DEBUG("EXTI0_IRQHandler:acc off\r\n");
		}
	}
}
#endif

/*
********************************************************************************
*  ��������: ADC_Configuration
*
*  ��������:ADC ��ʼ��
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //ADC ʱ��12M hz
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ͨ��ɨ��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//2//�������ⲿ����ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //����������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;//ֻ��һ��ͨ����Ҫ����
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);//���ò���ͨ��������Ͳ���ʱ��
	ADC_Cmd(ADC1, ENABLE);//ʹ��ADC
	ADC_ResetCalibration(ADC1); //����У׼
	while (ADC_GetResetCalibrationStatus(ADC1));//�ȴ�����У׼���
	ADC_StartCalibration(ADC1);//����У׼
	while (ADC_GetCalibrationStatus(ADC1)); //�ȴ�У׼���
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); //��ʼ��������ת��
}
void ADC1_SAMPLING(void)
{
	u16 rdata;
	int i, j;
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);//ͨ���ж�EOC�Ƿ�����
	ADC_ClearITPendingBit(ADC1, ADC_FLAG_EOC);//���ת����־λ
	rdata = ADC_GetConversionValue(ADC1);
	batt_lvl_in_milli_volts = ADC_RESULT_IN_MILLI_VOLTS(rdata);
	if (battery_voltage_arrary[0] == 0) {
		battery_voltage_arrary_index = 0;
		for (i = 0; i < VOLTAGE_AVG_NUM; i++) {
			battery_voltage_arrary[i] = batt_lvl_in_milli_volts;
		}
	}
	else {
		battery_voltage_arrary[battery_voltage_arrary_index] = batt_lvl_in_milli_volts;
		battery_voltage_arrary_index = (battery_voltage_arrary_index + 1) % VOLTAGE_AVG_NUM;
		batt_lvl_in_milli_volts = 0;
		for (j = 0; j < VOLTAGE_AVG_NUM; j++) {
			batt_lvl_in_milli_volts += battery_voltage_arrary[j];
		}
		batt_lvl_in_milli_volts /= VOLTAGE_AVG_NUM;
	}
	//RTU_DEBUG("ADC1_SAMPLING<%d>\r\n", batt_lvl_in_milli_volts);
}

/*
********************************************************************************
*  ��������: TIMER_Configuration
*
*  ��������: ��ʱ����ʼ��
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void TIMER_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); /* ʱ��ʹ��*/
	TIM_DeInit(TIM2); /*���½�Timer����Ϊȱʡֵ*/
	TIM_TimeBaseStructure.TIM_Period = 20000 - 1; /*�Զ���װ�ؼĴ������ڵ�ֵ(����  ֵ) 1s*/
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)(SystemCoreClock / 20000) - 1;  /* ʱ��Ԥ��Ƶ�� 72M/20000 -1 */

	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  /* ������Ƶ */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; /* ���ϼ���ģʽ */
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);  /* �������жϱ�־ */

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM2_IRQHandler(void)
{
	u16 PeriodNumber = power_off_time;
	static u16 acc_on_cnt = 0;
	static u16 acc_off_cnt = 0;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ  
	{
		/*if (Counter >= PeriodNumber)
			{
				 if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){
					Counter=0;
					GPIO_SetBits(GPIOA, GPIO_Pin_11);
					}
			}
		 Counter++;
		 */
		 //RTU_DEBUG("TIM2_IRQHandler\r\n");	
		ADC1_SAMPLING();
		if (batt_lvl_in_milli_volts > THRESHOLD) {
			acc_on_cnt++;
			acc_off_cnt = 0;
		}
		else {
			acc_off_cnt++;
			acc_on_cnt = 0;
		}

		if (acc_off_cnt > THRESHOLD_CNT) {
			acc_off_cnt = 0;
			acconoff = ACC_OFF;
		}
		if (acc_on_cnt > THRESHOLD_CNT) {
			acc_on_cnt = 0;
			acconoff = ACC_ON;
		}

		if (acconoff == ACC_ON) {
			if (0 == GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_11)) {
				GPIO_SetBits(GPIOA, GPIO_Pin_11);
				RTU_DEBUG("TIM2_IRQHandler:acc on\r\n");
			}
		}
		else if (acconoff == ACC_OFF) {
			GPIO_ResetBits(GPIOA, GPIO_Pin_11);
			RTU_DEBUG("TIM2_IRQHandler:acc off\r\n");
		}
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ    
}

u8 MemWriteData(u16 *data, u16 num)
{
	FLASH_Status temp_stat;
	u32 temp_addr = PAGE_ADDR;
	u16 i;

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	temp_stat = FLASH_ErasePage(temp_addr);
	if (temp_stat != FLASH_COMPLETE)
	{
		FLASH_Lock();
		return 0;
	}
	for (i = 0; i < num; i++)
	{
		temp_stat = FLASH_ProgramHalfWord((temp_addr + i * 2), data[i]);  //flash  Ϊһ���ֽڴ洢��16λ���ݱ����ַ��2 
		if (temp_stat != FLASH_COMPLETE)
		{
			FLASH_Lock();
			return 0;
		}

	}
	FLASH_Lock();
	return 1;
}

u8 MemReadData(u16 *data, u16 num)
{
	u32 *temp_addr = (u32 *)PAGE_ADDR;
	u16 i;

	for (i = 0; i < num; i++)
	{
		data[i] = *(temp_addr + i * 2);
	}
	return 1;
}


/*
********************************************************************************
*  ��������: main
*
*  ��������: ��������Ӧ�ó������
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
int main(void)
{
	u32 localtime;
	u8 tmp;
	u16 bandrate;
	int i;

	BSP_Init();

	// GPIO��ʼ��
	GpioInit();

	// shell��ʼ��
	ShellInit();

	// Ӳ��GPIO��ʼ��
	GPIO_Configuration();

	//�жϳ�ʼ��
	//EXTI0_Configuration();

	  //ADC��ʼ��
	ADC_Configuration();

	//��ʱ����ʼ��
	TIMER_Configuration();
	//ϵͳ��ʱ����ʼ��
	systick_init();

	Com3Init(CANCOM, 115200, COM_PARA_8N1);

	// ����У�麯����JTAG����ʱҪ���˺�������
#if VECTTAB_FLASH_OFFSET == 0x4000
	CmpCipher();
#endif

	//��ȡcanbandrate ����
	MemReadData(&bandrate, 1);
	if (0x1234 == bandrate)
	{
		CAN1Init(&Can1, CANX_BAUD_500K);
		CAN2Init(&Can2, CANX_BAUD_500K);
		GPIO_SetBits(GPIOC, GPIO_Pin_0);
		GPIO_ResetBits(GPIOC, GPIO_Pin_1);
	}
	else
	{
		CAN1Init(&Can1, CANX_BAUD_250K);
		CAN2Init(&Can2, CANX_BAUD_250K);
		GPIO_SetBits(GPIOC, GPIO_Pin_1);
		GPIO_ResetBits(GPIOC, GPIO_Pin_0);
	}

	WinceCmdInit();

	RTU_DEBUG("application start!\r\n");

	// ���Ź���ʼ��
	//WdgStart(10);

	// Tractor CAN
	memset(s_DA_LIST, 0, sizeof(s_DA_LIST));
	s_inWait = 1;
	RequestAddressClaim();
	//SendAddressClaim(1, s_SA);
	//SendAddressClaim(2, s_SA);
	s_AddressClaimTime = systick_getcnt();

	while (1)
	{
		localtime = systick_getcnt();

		// �û������������
		while (ComxGetChar(SHELLCOM, &tmp))
		{
			ShellMain(SHELLCOM, tmp);
		}

		while (ComxGetChar(CANCOM, &tmp))
		{
			WinceCmdEntry(tmp, CANCOM);
		}

		CanToUtcp(1, &Can1, CANCOM);
		CanToUtcp(2, &Can2, CANCOM);
		
		if (s_inWait && localtime-s_AddressClaimTime>250)
		{
			i = SelectAddress(s_SA);
			if (i < 0)
			{
				SendAddressClaim(1, 0xFE);
				SendAddressClaim(2, 0xFE);
			}
			else
			{
				s_SA = i;
				SendAddressClaim(1, s_SA);
				SendAddressClaim(2, s_SA);
			}
					
			s_inWait = 0;
		}

		if ((localtime + 5) < systick_getcnt())
		{
			RTU_DEBUG("main loop ticks <%d>\r\n", systick_getcnt() - localtime);
		}

		//WdgTick();
	}
}

/*
********************************************************************************
*  ��������: UserCmd
*
*  ��������: �û������������
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void UserCmd(void)
{
	u8 tmp;

	while (ComxGetChar(SHELLCOM, &tmp))
	{
		ShellMain(SHELLCOM, tmp);
	}
}

/*
********************************************************************************
*  ��������: WinceCmdInit
*
*  ��������: ���������ʼ��
*
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void WinceCmdInit(void)
{
	WinceCmdDec.Len = 0;
	WinceCmdDec.State = 0;
}

/*
********************************************************************************
*  ��������: WinceCmdEntry
*
*  ��������: ��CPU�����������
*
*
*  �������: dat   �ֽ�����
*            pcom  ����
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
u8 WinceCmdEntry(u8 dat, COMX * pcom)
{
	u8 dle;
	u32 i, k;
	u8 rescmd[7] = { BINTRANS_DLE, BINTRANS_STX, 0x86, 0x86, 0x86, BINTRANS_DLE, BINTRANS_ETX };
	u8 version[8] = { BINTRANS_DLE,BINTRANS_STX,'v','0','.','9',BINTRANS_DLE,BINTRANS_ETX };
	u8 ack500cmd[7] = { BINTRANS_DLE, BINTRANS_STX, 0x87, 0x87, 0x87, BINTRANS_DLE, BINTRANS_ETX };
	u8 ack250cmd[7] = { BINTRANS_DLE, BINTRANS_STX, 0x88, 0x88, 0x88, BINTRANS_DLE, BINTRANS_ETX };
	u16 wdata;

	// RTU_DEBUG("enter WinceCmdEntry!\r\n");

	switch (WinceCmdDec.State)
	{
	case 0:
		if (WinceCmdDec.Len < 2)
		{
			WinceCmdDec.Dat[WinceCmdDec.Len] = dat;
			WinceCmdDec.Len++;
		}
		else
		{
			if (WinceCmdDec.Dat[0] == BINTRANS_DLE && WinceCmdDec.Dat[1] == BINTRANS_STX)
			{
				WinceCmdDec.Dat[2] = dat;
				WinceCmdDec.Len = 3;
				WinceCmdDec.State = 1;
			}
			else
			{
				WinceCmdDec.Dat[0] = WinceCmdDec.Dat[1];
				WinceCmdDec.Dat[1] = dat;
				WinceCmdDec.Len = 2;
			}
		}
		break;
	case 1:
		WinceCmdDec.Dat[WinceCmdDec.Len++] = dat;

		// ��С���� 2�ֽ�ͷ + 2�ֽ�У�� + 2�ֽڽ�β
		if (WinceCmdDec.Len < 4)
		{
			break;
		}

		// �ҽ�β�ֽ�
		if (WinceCmdDec.Dat[WinceCmdDec.Len - 2] == BINTRANS_DLE &&
			WinceCmdDec.Dat[WinceCmdDec.Len - 1] == BINTRANS_ETX)
		{
			// ת���ֽ�
			for (dle = 0, k = 0, i = 2; i < WinceCmdDec.Len - 2; i++)
			{
				if (WinceCmdDec.Dat[i] == BINTRANS_DLE)
				{
					if (dle == 1)
					{
						dle = 0;
						continue;
					}
					else
					{
						dle = 1;
					}
				}
				else
				{
					dle = 0;
				}

				WinceCmdDec.Esc[k++] = WinceCmdDec.Dat[i];
			}

			WinceCmdDec.Len = k;

			if (WinceCmdDec.Len == 3)//�����ʽ����ֻ��3
			{

				// ����������Ӧ
				if (WinceCmdDec.Esc[0] == 0x85)
				{
					ComxPutData(pcom, rescmd, 7);
				}
				else if (WinceCmdDec.Esc[0] == 0x87)
				{
#if 1
					CAN1Init(&Can1, CANX_BAUD_500K);
					CAN2Init(&Can2, CANX_BAUD_500K);
					GPIO_SetBits(GPIOC, GPIO_Pin_0);
					GPIO_ResetBits(GPIOC, GPIO_Pin_1);
					wdata = 0x1234;
					MemWriteData(&wdata, 1);
					ComxPutData(pcom, ack500cmd, 7);
#else
					wdata = 0x1234;
					MemWriteData(&wdata, 1);
					ComxPutData(pcom, ack500cmd, 7);
					DelayMS(10);
					GPIO_ResetBits(GPIOC, GPIO_Pin_9);
					SysSoftRst();
#endif
				}
				else if (WinceCmdDec.Esc[0] == 0x88)
				{
#if 1
					CAN1Init(&Can1, CANX_BAUD_250K);
					CAN2Init(&Can2, CANX_BAUD_250K);
					GPIO_SetBits(GPIOC, GPIO_Pin_1);
					GPIO_ResetBits(GPIOC, GPIO_Pin_0);
					wdata = 0xffff;
					MemWriteData(&wdata, 1);
					ComxPutData(pcom, ack250cmd, 7);
#else
					wdata = 0xffff;
					MemWriteData(&wdata, 1);
					ComxPutData(pcom, ack250cmd, 7);
					DelayMS(10);
					GPIO_ResetBits(GPIOC, GPIO_Pin_9);
					SysSoftRst();
#endif
				}
				else if (WinceCmdDec.Esc[0] == 0x89)
				{
					ComxPutData(pcom, version, 8);
				}

				// �ָ���ʼ����״̬
				WinceCmdDec.Len = 0;
				WinceCmdDec.State = 0;
			}
			else if (WinceCmdDec.Len >= 8)//�������͵�����
			{
				//�Ƴ�У��A B ��������
				UartDataToCanData(WinceCmdDec.Len - 2, WinceCmdDec.Esc);

			}

		}
		else if (WinceCmdDec.Dat[WinceCmdDec.Len - 2] == BINTRANS_DLE &&
			WinceCmdDec.Dat[WinceCmdDec.Len - 1] == BINTRANS_STX)
		{
			WinceCmdDec.Dat[0] = BINTRANS_DLE;
			WinceCmdDec.Dat[1] = BINTRANS_STX;
			WinceCmdDec.Len = 2;
		}

		// ������󳤶�
		if (WinceCmdDec.Len >= BINTRANS_MAXLEN)
		{
			WinceCmdDec.Len = 0;
			WinceCmdDec.State = 0;
		}
		break;
	default:
		WinceCmdDec.Len = 0;
		WinceCmdDec.State = 0;
		break;
	}

	return 0;
}

/*
********************************************************************************
*  ��������: CanToUtcp
*
*  ��������: ��CAN��������ֱ�Ӵ����UTCP���������ݴ���Э�鷢�͡�
*
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void CanToUtcp(u8 channel, CANX * pcan, COMX * pcom)
{
	u8 i, cha, chb, buf[20];
	CANQUEDATA data;

	if (channel != 1 && channel != 2)
	{
		return;
	}

	else if (pcan == NULL || pcom == NULL)
	{
		return;
	}

	while (CanReceive(pcan, &data))
	{
		if (CheckReceive(channel, &data) > 0)
		{
			continue;
		}

		buf[0] = channel;
		buf[1] = (u8)(data.Id >> 24);
		buf[2] = (u8)(data.Id >> 16);
		buf[3] = (u8)(data.Id >> 8);
		buf[4] = (u8)(data.Id >> 0);
		buf[5] = data.Dlc;

		for (i = 0; i < data.Dlc; i++)
		{
			buf[6 + i] = data.Data[i];
		}

		if (data.Dlc < 8)
		{
			memset(buf + 6 + data.Dlc, 0, 8 - data.Dlc);
		}

		// ����У��ֵ
		cha = 0;
		chb = 0;
		for (i = 0; i < 14; i++)
		{
			cha = cha + buf[i];
			chb = chb + cha;
		}
		buf[14] = cha;
		buf[15] = chb;

		// �򴮿ڷ���CAN����
		RTU_DEBUG("CAN%d recieve data: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\r\n",
			channel, buf[6], buf[7], buf[8], buf[9], buf[10], buf[11], buf[12], buf[13]);

		ComxPutChar(pcom, BINTRANS_DLE);
		ComxPutChar(pcom, BINTRANS_STX);
		for (i = 0; i < 16; i++)
		{
			if (buf[i] == BINTRANS_DLE)
			{
				ComxPutChar(pcom, BINTRANS_DLE);
			}
			ComxPutChar(pcom, buf[i]);
		}

		ComxPutChar(pcom, BINTRANS_DLE);
		ComxPutChar(pcom, BINTRANS_ETX);
	}
}

/*
********************************************************************************
*  ��������: UartDataToCanData
*
*  ��������: ���յ���uart���ݽ��д��
*
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/

void UartDataToCanData(u8 len, u8 *Data)
{
	CANX* pCAN = NULL;
	CANQUEDATA data;
	u8 i;

	switch (Data[0])
	{
	case 1:
		pCAN = &Can1;
		break;
	case 2:
		pCAN = &Can2;
		break;
	}

	if (NULL == pCAN)
	{
		return;
	}

	memset(&data, 0, sizeof(CANQUEDATA));

	data.Id = ((Data[1] << 24) | (Data[2] << 16) | (Data[3] << 8) | Data[4]);
	data.Dlc = Data[5];

	for (i = 0; i < Data[5]; i++)
	{
		data.Data[i] = Data[i + 6];
	}

	CanTransmit(pCAN, &data);
}

/*
********************************************************************************
*  ��������: SendAddressClaim
*
*  ��������: ���͵�ַ������Ϣ
*
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void SendAddressClaim(u8 channel, u8 sa)
{
	CANQUEDATA data;
	u32 ID;
	CANX* pCAN = NULL;

	switch (channel)
	{
	case 1:
		pCAN = &Can1;
		break;
	case 2:
		pCAN = &Can2;
		break;
	default:
		return;
	}

	ID = (0x18EEFF00 | sa) << 3;
	ID |= 0x04;

	data.Id = ID;
	data.Dlc = 8;

	GetNameInfo(0x123456);
	memcpy(data.Data, MyName, 8);

	CanTransmit(pCAN, &data);
}

/*
********************************************************************************
*  ��������: GetNameInfo
*
*  ��������: ��ȡ������NAME��Ϣ
*
*
*  �������: id : serial number of the Tractor ECU
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void GetNameInfo(u32 id)
{
	MyName[0] = id & 0xFF;
	MyName[1] = (id>>8) & 0xFF;
	MyName[2] = (id >> 16) & 0x1F;
	MyName[3] = 0x34;
	MyName[4] = 0x00;
	MyName[5] = 0x86;
	MyName[6] = 0x00;
	MyName[7] = 0x20;
}

/*
********************************************************************************
*  ��������: RequestAddressClaim
*
*  ��������: �����ַ������Ϣ
*
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void RequestAddressClaim(void)
{
	CANQUEDATA data;

	data.Id = (0x18EAFFFE << 3) | 0x04;
	data.Dlc = 3;

	data.Data[0] = 0x00;
	data.Data[1] = 0xEE;
	data.Data[2] = 0x00;

	CanTransmit(&Can1, &data);
	CanTransmit(&Can2, &data);
}

int CheckReceive(u8 channel, CANQUEDATA* pData)
{
	int ID = 0;
	u8 address = 0;
	int i;

	// ���Ա�׼֡��Զ��֡
	if ((pData->Id & 0x04)==0 || (pData->Id & 0x02)!=0)
	{
		return 0;
	}

	ID = pData->Id >> 3;

	if (ID==0x18EAFFFE && pData->Dlc==3)
	{
		if (pData->Data[0]==0x00 && pData->Data[1]==0xEE && pData->Data[2]==0x00)
		{
			SendAddressClaim(channel, s_SA);
			return 1;
		}
	}
	else
	{
		if (0x18EEFF == (ID >> 8))
		{
			address = ID & 0xFF;
			s_DA_LIST[address] = 1;
			
			if (s_inWait)
			{
				return 1;
			}

			if (address == s_SA)
			{
				if (CompareName(MyName, pData->Data) < 0)
				{
					SendAddressClaim(channel, s_SA);
				}
				else
				{
					i = SelectAddress(s_SA - 1);
					if (i < 0)
					{
						SendAddressClaim(channel, 0xFE);
					}
					else
					{
						s_SA = i;
						SendAddressClaim(channel, s_SA);
					}
				}
			}

			return 1;
		}
	}

	return 0;
}

int CompareName(u8* name1, u8* name2)
{
	int i;
	int value;

	for (i=7; i>=0; --i)
	{
		value = name1[i] - name2[i];

		if (0 != value)
		{
			return value;
		}
	}

	return 0;
}

int SelectAddress(int maxAddr)
{
	const int MIN_ADDR = 160;
	int i;

	for (i = maxAddr; i >= MIN_ADDR; --i)
	{
		if (s_DA_LIST[i] == 0)
		{
			s_DA_LIST[i] = 1;
			break;
		}
	}
	
	return (i >= MIN_ADDR) ? i : -1;
}