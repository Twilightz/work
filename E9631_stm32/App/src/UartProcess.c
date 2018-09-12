/*
********************************************************************************
*  Copyright (C), 2009-2013, ����˼׳�����з�����
*
*  ��Ŀ���ƣ�E7759A
*  
*  �ļ�����: UartProcess.c
*
*  �ļ��������������ݴ�������
*             
*             
*  �� �� ��: Τ����
*
*  �������ڣ�2017-10-9 
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

#include "Obd.h"
#include "Shell.h"
#include <stdlib.h>
#include "Public.h"
#include "Can.h"
#include "CanDrv.h"
#include "Flash.h"
#include "Adc.h"
#include "Delay.h"
#include "string.h"
#include "Delay.h"
#include "UartProcess.h"
//#include "sleep.h"
#include "J1939.h"
/*
********************************************************************************                                                                  
*  �ⲿ��������                                                                                                       
********************************************************************************
*/
extern u8 batteryvolt[10];
extern bool is_shutdown_can;
extern bool cannelfilter;
/*
********************************************************************************                                                                 
*  �ڲ���������                                                                                                         
********************************************************************************
*/

DiagnosticRequestHandle RequestHandle;

CANPACKET sendcanpacket;
PACKET UART3Recvpacket; //����UART3���յ��ı���
UART3DATASTRUCT  UART3Data;//�������UART3ԭʼ����

WORK_MODE_T work_mode = COMMAND_MODE;
CHANNEL_TYPE_T channel = CAN_CHANNEL_1;

u8 accon_ack = 0;

extern bool is_open_timer4;
extern bool is_open_timer6;

bool is_accoff_ack  = FALSE;
bool is_accon_ack  = FALSE;
bool is_E9631_Boot  = FALSE;
bool is_Cancel_shutdown = FALSE;
//�汾
u8 version[10] = {0x21, 0x00, 0x04,'2','.','1','8'};

//UART������
u8 ackbaudrate[6] = {0x31,0x00, 0x01};
//GPIO
u8 gpioack[7] = {0x91,0x00, 0x02};
//ģʽ
u8 modeack[6] = {0x81,0x00, 0x01};
//can ͨ��
u8 channelack[6] = {0x83,0x00, 0x01};  

u8 ackfitter[10] = {0x50,0x00, 0x01};
u8 ackcancelfitter[6] = {0x51,0x00, 0x01};
/*
********************************************************************************                                                                  
*  �ڲ���������                                                                                                        
********************************************************************************
*/
void senddatapacket(COMX * pcom,u8* data,u16 length);
void recvpacketparse(PACKET* Precvpacket);
void SetUartBaudrate(u8 data);
/*
********************************************************************************                                                                  
*  �ⲿ��������                                                                                                        
********************************************************************************
*/
extern u16 Timer6Count;
extern u8 turnoncmd[6];
extern u8 turnoffcmd[6];
/*
********************************************************************************
*  ��������: StructInit
*
*  ��������: �ṹ���ʼ��
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
void StructInit(void)
{
    UART3Data.Len   = 0;
    UART3Data.State = 0;
	
	  UART3Recvpacket.candata = NULL;
	  //UART3Recvpacket.data = NULL;
	
}

void gpiotest(u8 command)
{
	  u8 status;
	
	  switch(command)
    {

				//RADAR_IN  ����OK
				case 0x12:
				      //RTU_DEBUG("RADAR_IN test!\r\n");
			 				status = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12);
					
							gpioack[3] = 0x12;
              gpioack[4] = status;								
							senddatapacket(&COM3,gpioack,FIXED_LENGTH+2+CHECKSUM);	
							break;

			 //MILEAGE_PWE_EN //����OK
			 case 0x16:
							//RTU_DEBUG("MILEAGE_PWE_EN test!\r\n");	
							status = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6);
						
							gpioack[3] = 0x16;	
              gpioack[4] = status;								
							senddatapacket(&COM3,gpioack,FIXED_LENGTH+2+CHECKSUM);
							break;
			//MILEAGE_mcuin
			case 0x22:
						  //RTU_DEBUG("MILEAGE_mcuin test!\r\n");
							status = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15);
			
							gpioack[3] = 0x22;
              gpioack[4] = status;								
						  senddatapacket(&COM3,gpioack,FIXED_LENGTH+2+CHECKSUM);
							break;
		 //ACC status
		 case 0x30:	
							//ACC ON
							//RTU_DEBUG("ACC status test!\r\n");
							status = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
							if(0 == status){							
									senddatapacket(&COM3,turnoncmd,FIXED_LENGTH+1+CHECKSUM);
							}
							//ACC OFF
							else if(0 != status)
							{
									senddatapacket(&COM3,turnoffcmd,FIXED_LENGTH+1+CHECKSUM);
							}
							break;
			//gpio input
			case 0x44:
						  status = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0);
			
							gpioack[3] = 0x44;
              gpioack[4] = status;								
						  senddatapacket(&COM3,gpioack,FIXED_LENGTH+2+CHECKSUM);
							break;
			}               								
}

/*
********************************************************************************
*  ��������: UART3_Data_Pro
*
*  ��������: ����UART3����
*            
*
*  �������: dat
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void UART3_Data_Pro(u8 dat)
{
    u8 dle;
    u16 i, k;
	  u16 index = 0;
	  u8 cha,chb;
	  u16 CurrentPacketLen = 0;
	  static u8 flag = 0;
	 
    switch(UART3Data.State)
    {
			//����BINTRANS_DLE��BINTRANS_STX
			case 0:
		       if(UART3Data.Len<1)
					 {
								UART3Data.Dat[UART3Data.Len++] = dat;
					 }
					 //������BINTRANS_DLE���������ж�
		       if(UART3Data.Dat[0] == BINTRANS_DLE)
					 {			
        			 if(flag==0)
							 {
								  flag=1;
							 }
							 else
							 {
									UART3Data.Dat[UART3Data.Len++] = dat;
									if(UART3Data.Len == 2)
									{
										  //�������ڶ����ֽ���BINTRANS_STX�������������һ֡���ݡ�
											if(UART3Data.Dat[1] == BINTRANS_STX)
											{
													flag = 0;
													UART3Data.State  = 1;
											}
											else
											{
													flag = 0;
													UART3Data.Len = 0;
											}
									}											
							}
					}
					else
					{
							UART3Data.Len = 0;
					}
          break;
	   case 1:
					UART3Data.Dat[UART3Data.Len++] = dat;
					// �ҽ�β�ֽ�
					if(UART3Data.Dat[UART3Data.Len - 2] == BINTRANS_DLE && 
            UART3Data.Dat[UART3Data.Len - 1] == BINTRANS_ETX)
					{  
            // ת���ֽ�
            for(dle = 0, k = 0, i = 2; i < UART3Data.Len - 2; i++)
            {
							  //��<DLE><DLE>ת��Ϊ<DLE>
                if(UART3Data.Dat[i] == BINTRANS_DLE)
                {
                    if(dle == 1)
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
                UART3Data.Esc[k++] = UART3Data.Dat[i];
            }
						
						//ȡ��ǰ����
						ato16(&UART3Data.Esc[1],&CurrentPacketLen);
						if(k == FIXED_LENGTH+CurrentPacketLen+CHECKSUM)
						{
								UART3Data.Len = k;
								UART3Recvpacket.cha = UART3Data.Esc[UART3Data.Len-2];
								UART3Recvpacket.chb = UART3Data.Esc[UART3Data.Len-1];	
						    
								CalculateChecksum(UART3Data.Esc,UART3Data.Len-2,&cha,&chb);
								//���У���һ�£���Ա��Ľ�һ������		 
								if(UART3Recvpacket.cha == cha && UART3Recvpacket.chb == chb)
								{
									 //��ǰ���ݰ�����(������У��)
									 UART3Recvpacket.CurrentLen = UART3Data.Len-2;
									 //�����ݰ�������recvpacket.data
									 memcpy(UART3Recvpacket.data,UART3Data.Esc,UART3Recvpacket.CurrentLen);
									 UART3Recvpacket.index = 0;	
									 UART3Recvpacket.codecState = CODEC_STATE_PACKET_TYPE;
									 //��UART3���Ľ��д���
									 recvpacketparse(&UART3Recvpacket);
									 is_shutdown_can = FALSE;
									 UART3Data.Len = 0;
									 UART3Data.State = 0;
							 }
							 else
							 {
									 UART3Data.Len = 0;
									 UART3Data.State = 0;
							 }
						}
						else
						{
								UART3Data.Len = 0;
								UART3Data.State = 0;
						}
			  }
        // ������󳤶�
        if(UART3Data.Len >= BINTRANS_MAXLEN)
        {
            UART3Data.Len   = 0;
            UART3Data.State = 0;
        }
        break;
    default:
        UART3Data.Len   = 0;
        UART3Data.State = 0;
        break;
    }	
    return;
}


/*
********************************************************************************
*  ��������: senddatapacket
*
*  ��������: �����ڷ���Э�����ݰ�
*            
*
*  �������: pcom��data��length
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
//length=type(һ���ֽڣ�+��ǰ���ȣ������ֽ�) +���� +У��ͣ������ֽڣ�

void senddatapacket(COMX * pcom,u8* data,u16 length)
{
	  u16 i;
	  u8 cha,chb;
  
    if(pcom == NULL && data == NULL && length == 0)
		{
			return;
		}
		
		CalculateChecksum(data,length-2,&cha,&chb);
		data[length-2] = cha;
		data[length-1] = chb;
		
	  ComxPutChar(pcom, BINTRANS_DLE);
		ComxPutChar(pcom, BINTRANS_STX);
		for(i = 0; i < length; i++)
		{
				if(data[i] == BINTRANS_DLE)
				{
						ComxPutChar(pcom, BINTRANS_DLE);
				}
				
				ComxPutChar(pcom, data[i]);
		}				
    ComxPutChar(pcom, BINTRANS_DLE);
	  ComxPutChar(pcom, BINTRANS_ETX);
		
}



/*
********************************************************************************
*  ��������: recvpacketparse
*
*  ��������: �������ݰ�����������
*            
*
*  �������: Precvpacket
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
extern u8 count3;
void recvpacketparse(PACKET* Precvpacket)
{
	  u8 i,temp,Accstatus = 0;
	  u8 iterator = 0;
	  u16 index = 0;
	  u16 canbandrate = 0;
	  u16 CurrentPacketLen = 0;
	  u32 canID,MaskId_t = 0;
	  u8 CanIDTemp[4],MaskIdTemp[4];
	  u32 n=0;
	  u8 MaskID[9];
	  u16 canidstd,MaskId_std;
	
	  //��ָ��Ϊ�գ��ҳ������ݳ���С�ڵ���0ʱ���˳�������
    if(Precvpacket == NULL && Precvpacket->CurrentLen <= 0)
		{
			  return;
		}
	 	for(index = 0; index < Precvpacket->CurrentLen; index++)
		{
				iterator = Precvpacket->data[index];
			
			  switch(Precvpacket->codecState)
		    {
					 //��������
					 case CODEC_STATE_PACKET_TYPE:
								Precvpacket->PacketType = iterator;
					      Precvpacket->codecState = CODEC_STATE_CURRENT_PACKET_LENGTH;
					  break;
					 //���ĵ�ǰ����
					 case CODEC_STATE_CURRENT_PACKET_LENGTH:
							  Precvpacket->CurrentPacketLen[Precvpacket->index++] = iterator;
								if(Precvpacket->index == 2)
								{
										Precvpacket->index = 0;
										CurrentPacketLen = 0;
										ato16(Precvpacket->CurrentPacketLen,&CurrentPacketLen);
									
									  if(Precvpacket->PacketType == 0x60 || Precvpacket->PacketType == 0x70)
										{
												Precvpacket->candata = (u8*)malloc(CurrentPacketLen);
											  if(Precvpacket->candata == NULL)
													return;
										}
										Precvpacket->codecState = CODEC_STATE_PAYLOAD;								 
							  }			
							break;
					//��Ч�غɣ�Ӧ�����ݣ�
					case CODEC_STATE_PAYLOAD:
						 //CAN
					  if(Precvpacket->PacketType == 0x40)
						{		
							  
								Precvpacket->can[Precvpacket->index++] = iterator;
								if(Precvpacket->index == CurrentPacketLen && CurrentPacketLen >= 0)
							  {
									  IWDG_Feed();
									  Accstatus = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
									  if(Accstatus ==0)
										{
											 UartDataToCanData(CurrentPacketLen,Precvpacket->can);
										}
										Precvpacket->index = 0;  
								}

						}
	
						//OBD
						else	if(Precvpacket->PacketType == 0x60)
						{		
								Precvpacket->candata[Precvpacket->index++] = iterator;
								if(Precvpacket->index == CurrentPacketLen && CurrentPacketLen >= 0)
							  {
									  IWDG_Feed();
									  Accstatus = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
									  if(Accstatus ==0)
										{
												RequestHandle.Request.buf = (u8*)malloc(CurrentPacketLen);
												if(RequestHandle.Request.buf == NULL)
													 return;
												memcpy(RequestHandle.Request.buf,Precvpacket->candata,CurrentPacketLen);
												diagnostic_request(&RequestHandle,DiagnosticResponsePro);
												free(Precvpacket->candata);
										}
										Precvpacket->index = 0;  
								}

						}
						//J1939
						else if(Precvpacket->PacketType == 0x70)
						{
								Precvpacket->candata[Precvpacket->index++] = iterator;
							  if(Precvpacket->index == CurrentPacketLen && CurrentPacketLen >= 0)
							  {
									 IWDG_Feed();
									 Accstatus = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
									 if(Accstatus ==0)
									 {
											RequestHandle.Request.buf = (u8*)malloc(CurrentPacketLen);
											if(RequestHandle.Request.buf == NULL)
												return;
											memcpy(RequestHandle.Request.buf,Precvpacket->candata,CurrentPacketLen);
											J1939_request(&RequestHandle,J1939_ResponsePro);
									 
											free(Precvpacket->candata);
											Precvpacket->index = 0;  
										}											
								}
						}
						//����ID����
					  else if(Precvpacket->PacketType == 0x50)
						{		

								Precvpacket->canid[Precvpacket->index++] = iterator;
								if(Precvpacket->index == CurrentPacketLen && CurrentPacketLen >= 0)
							  {
									  for(i=1;i<9;i++)
									  {
											 if(Precvpacket->canid[i]>='0' && Precvpacket->canid[i]<='9')
											 {
													Precvpacket->canid[i] = Precvpacket->canid[i] -48;
											 }
                       if(Precvpacket->canid[i]>='A' && Precvpacket->canid[i]<='F')
											 {
													Precvpacket->canid[i] = Precvpacket->canid[i] -55;
											 }
											 if(Precvpacket->canid[i]=='X')
											 {
													Precvpacket->canid[i] = 0x00;
												  MaskID[i] = 0x0;
											 }
											 else if(Precvpacket->canid[i]!='X')
											 {
													MaskID[i] = 0xF;
											 } 
										}
										CanIDTemp[0] = Precvpacket->canid[1]<<4 | Precvpacket->canid[2];
										CanIDTemp[1] = Precvpacket->canid[3]<<4 | Precvpacket->canid[4];
									  CanIDTemp[2] = Precvpacket->canid[5]<<4 | Precvpacket->canid[6];
									  CanIDTemp[3] = Precvpacket->canid[7]<<4 | Precvpacket->canid[8];
										
										MaskIdTemp[0] = MaskID[1]<<4 | MaskID[2];
										MaskIdTemp[1] = MaskID[3]<<4 | MaskID[4];
									  MaskIdTemp[2] = MaskID[5]<<4 | MaskID[6];
									  MaskIdTemp[3] = MaskID[7]<<4 | MaskID[8];

										canID = ((CanIDTemp[0] << 24) | (CanIDTemp[1] << 16) | (CanIDTemp[2] << 8) | CanIDTemp[3]);
										MaskId_t = ((MaskIdTemp[0] << 24) | (MaskIdTemp[1] << 16) | (MaskIdTemp[2] << 8) | MaskIdTemp[3]);
										ackfitter[3] = CanIDTemp[0];
										ackfitter[4] = CanIDTemp[1];
										ackfitter[5] = CanIDTemp[2];
										ackfitter[6] = CanIDTemp[3];
										//��չID
										if(Precvpacket->canid[0] == 0x04 || Precvpacket->canid[0] == 0x06)
										{
												CAN_SetFilterExt(canID,MaskId_t,Precvpacket->canid[0]);
										}//��׼ID									
										else if(Precvpacket->canid[0] == 0x00)
										{
											  canidstd = (u16)(CanIDTemp[2]<<8)|CanIDTemp[3];
											  MaskId_std = (u16)(MaskIdTemp[2]<<8)|MaskIdTemp[3];
												CAN_SetFilterStd(canidstd,Precvpacket->canid[0],MaskId_std);
										}
										else if(Precvpacket->canid[0] == 0x08)
										{
											   J1939_CAN_SetFilterExt_Mutl(canID);
											   cannelfilter = TRUE;
										}
									  ackfitter[3] = count3;
									  senddatapacket(&COM3,ackfitter,FIXED_LENGTH+1+CHECKSUM);
										Precvpacket->index = 0;  
								}

						}
						//ȡ��ID����
					  else if(Precvpacket->PacketType == 0x51)
						{	
							  if(channel == CAN_CHANNEL_1)
								{
										setFilter(CAN_CHANNEL_1);
								}
								else if(channel == CAN_CHANNEL_2)
								{
										setFilter(CAN_CHANNEL_2);
								}
							  ackcancelfitter[3] = 0x01;
							  senddatapacket(&COM3,ackcancelfitter,FIXED_LENGTH+1+CHECKSUM);
						}
						else
						{ 
					      Precvpacket->command = iterator;
								switch(Precvpacket->PacketType)
								{
									  
										case 0x31:
												switch(Precvpacket->command)
												{
													//���յ���ACC ONӦ��
													case 0x01:										
															 is_accon_ack = TRUE;
															 is_accoff_ack = FALSE;
                    					 break;
													//���յ�ACC OFFӦ��
													case 0x00:
														  is_accoff_ack = TRUE;
													    is_accon_ack = FALSE;
												      break;													
												}
												break;
										//����E7759�Ŀ�������
										case 0x20:
										  switch(Precvpacket->command)
											{
												//��ȡ�����汾����
												case 0x33:		
                             //RTU_DEBUG("get version!\r\n");													
												   	 senddatapacket(&COM3,version,FIXED_LENGTH+4+CHECKSUM);
												     break;
												//��ȡ��ص�ѹ
												case 0x34:	
													   //RTU_DEBUG("batteryvolt!\r\n");
                             ADC1_SAMPLING();
                             if(batteryvolt[6] == 0x00)
                             {															 
																batteryvolt[6] += 0x30;
                             }													 
														 senddatapacket(&COM3,batteryvolt,FIXED_LENGTH+4+CHECKSUM);
														 break;
								
										 }               								
								     break;
								case 0x30:
										  switch(Precvpacket->command)
											{	
												//���յ�AndroidӦ��ȡ���ػ�
												case 0x02:	
                             //RTU_DEBUG("Cancel the shutdown!\r\n");
                             is_Cancel_shutdown = TRUE;		
                 					   break;
												//�ػ�
												case 0x03:
													   //RTU_DEBUG("Shut down immediately commond!\r\n");
												     TIM_Cmd(TIM4, ENABLE);		
                             is_open_timer4 = TRUE;	
														 TIM_Cmd(TIM6, DISABLE);
                             is_open_timer6 = FALSE;	
                             Timer6Count = 0;												
														 break;
												//Զ������
												case 0x04:
													   TIM_Cmd(TIM3, DISABLE);
														 TIM_Cmd(TIM5, ENABLE);
														 break;
												//�л���500K����
												case 0x50:
												     if(work_mode == CAN_MODE)
														 {
															  if(channel == CAN_CHANNEL_1)
																{
																		//RTU_DEBUG("CHANNEL_1  CAN_MODE:Setting 500K!\r\n");
																		canbandrate = 0x1503;  //����0x1503��1��ʾͨ��1,50��ʾ500K��3��ʾCAN_MODE
																		CAN1Init(&Can1, CANX_BAUD_500K,1);
																}
																else if(channel == CAN_CHANNEL_2)
																{
																	  //RTU_DEBUG("CHANNEL_2  CAN_MODE:Setting 500K!\r\n");
																	  canbandrate = 0x2503;
																	  CAN2Init(&Can2, CANX_BAUD_500K,1);
																}
														 }
														 else if(work_mode == OBD_MODE)
														 {
															 if(channel == CAN_CHANNEL_1)
																{
																	  //RTU_DEBUG("CHANNEL_1  OBD_MODE:Setting 500K!\r\n");
																	  canbandrate = 0x1502;
																		CAN1Init(&Can1, CANX_BAUD_500K,0);
															  }
															  else if(channel == CAN_CHANNEL_2)
																{
																	 	//RTU_DEBUG("CHANNEL_2  OBD_MODE:Setting 500K!\r\n");
																	  canbandrate = 0x2502;
																		CAN2Init(&Can2, CANX_BAUD_500K,0); 
																}
														 }
														 MemWriteData(PAGE_ADDR+8,&canbandrate,1);
												     DelayMS(4000);
												     ackbaudrate[3] = 0x50;
														 senddatapacket(&COM3,ackbaudrate,FIXED_LENGTH+1+CHECKSUM);
														 break;
												//�л���250K����
												case 0x25:	 
												     if(work_mode == CAN_MODE)
														 {
															  if(channel == CAN_CHANNEL_1)
																{
																	  //RTU_DEBUG("CHANNEL_1   CAN_MODE:Setting 250K!\r\n");
																	  canbandrate = 0x1253;
																		CAN1Init(&Can1, CANX_BAUD_250K,1);
																}
																else if(channel == CAN_CHANNEL_2)
																{
																	  //RTU_DEBUG("CHANNEL_2   CAN_MODE:Setting 250K!\r\n");
																	  canbandrate = 0x2253;
																		CAN2Init(&Can2, CANX_BAUD_250K,1);
																}
														 }
														 else if(work_mode == OBD_MODE)
														 {
															 	if(channel == CAN_CHANNEL_1)
																{
																	  //RTU_DEBUG("CHANNEL_1  OBD_MODE:Setting 250K!\r\n");
																	  canbandrate = 0x1252;
																		CAN1Init(&Can1, CANX_BAUD_250K,0);
															  }
															  else if(channel == CAN_CHANNEL_2)
																{
																	 	//RTU_DEBUG("CHANNEL_2  OBD_MODE:Setting 250K!\r\n");
																	  canbandrate = 0x2252;
																		CAN2Init(&Can2, CANX_BAUD_250K,0); 
																}
														 }
														 else if(work_mode == J1939_MODE)
														 {
															  if(channel == CAN_CHANNEL_1)
																{
																	  //RTU_DEBUG("CHANNEL_1   J1939_MODE:Setting 250K!\r\n");
																		canbandrate = 0x1251;
																		CAN1Init(&Can1, CANX_BAUD_250K,0);
																}
																else if(channel == CAN_CHANNEL_2)
																{
																	  //RTU_DEBUG("CHANNEL_2   J1939_MODE:Setting 250K!\r\n");
																		canbandrate = 0x2251;
																	  CAN2Init(&Can2, CANX_BAUD_250K,0);
																}
														 }
												     MemWriteData(PAGE_ADDR+8,&canbandrate,1);
												     DelayMS(4000);
												     ackbaudrate[3] = 0x25;
												     senddatapacket(&COM3,ackbaudrate,FIXED_LENGTH+1+CHECKSUM);
														 break;
														 //�л���125K����
												case 0x12:	 
												     if(work_mode == CAN_MODE)
														 {
															  if(channel == CAN_CHANNEL_1)
																{
																	  //RTU_DEBUG("CHANNEL_1   CAN_MODE:Setting 125K!\r\n");
																	  canbandrate = 0x1123;
																		CAN1Init(&Can1, CANX_BAUD_125K,1);
																}
																else if(channel == CAN_CHANNEL_2)
																{
																	  //RTU_DEBUG("CHANNEL_2   CAN_MODE:Setting 125K!\r\n");
																	  canbandrate = 0x2123;
																		CAN2Init(&Can2, CANX_BAUD_125K,1);
																}
														 }

												     MemWriteData(PAGE_ADDR+8,&canbandrate,1);
												     DelayMS(4000);
												     ackbaudrate[3] = 0x12;
												     senddatapacket(&COM3,ackbaudrate,FIXED_LENGTH+1+CHECKSUM);
														 break;
											 //9600������
											 case 0x9:
											 //19200������
											 case 0x19:
											 //57600������
										 	 case 0x57:
											 //115200������
											 case 0x11:
											 //230400������
											 case 0x23:
											 //460800������
											 case 0x46:
												     SetUartBaudrate(Precvpacket->command);
														 break;
										}               								
								    break;
								 case 0x80:
										 switch(Precvpacket->command)
								     {
											  //��ѯģʽ
											  case 0x00:
												    //RTU_DEBUG("Inquiry mode!\r\n");
												    switch(work_mode)
												    {
																case COMMAND_MODE:
																	modeack[3] = 0x00;
																	senddatapacket(&COM3,modeack,FIXED_LENGTH+1+CHECKSUM);
																	break;
																case J1939_MODE:
																	modeack[3] = 0x01;
																	senddatapacket(&COM3,modeack,FIXED_LENGTH+1+CHECKSUM);
																	break;
																case OBD_MODE:
																	modeack[3] = 0x02;
																	senddatapacket(&COM3,modeack,FIXED_LENGTH+1+CHECKSUM);
																	break;
																case CAN_MODE:
																	modeack[3] = 0x03;
																	senddatapacket(&COM3,modeack,FIXED_LENGTH+1+CHECKSUM);
																	break;
														}
														break;
											  //CANģʽ
												case 0x03:
													  //RTU_DEBUG("Entry CAN mode!\r\n");
													  work_mode = CAN_MODE;
														modeack[3] = 0x03;											
														senddatapacket(&COM3,modeack,FIXED_LENGTH+1+CHECKSUM);
														break;	
											  //OBDģʽ
												case 0x02:
													  //RTU_DEBUG("Entry OBD mode!\r\n");
													  work_mode = OBD_MODE;
														modeack[3] = 0x02;											
														senddatapacket(&COM3,modeack,FIXED_LENGTH+1+CHECKSUM);
														break;
											  //J1939ģʽ
												case 0x01:
													  //RTU_DEBUG("Entry J1939 mode!\r\n");
													  work_mode = J1939_MODE;
														modeack[3] = 0x01;												
														senddatapacket(&COM3,modeack,FIXED_LENGTH+1+CHECKSUM);
														break;																
										 }
										 break;
								//��ѯcanͨ��
								case 0x82:
										 switch(Precvpacket->command)
								     {
												case 0x00:
													//RTU_DEBUG("Inquiry can channel!\r\n");
													switch(channel)
													{
															case CAN_CHANNEL_1:
																channelack[3] = CAN_CHANNEL_1;
																senddatapacket(&COM3,channelack,FIXED_LENGTH+1+CHECKSUM);
																break;
															case CAN_CHANNEL_2:
																channelack[3] = CAN_CHANNEL_2;
																senddatapacket(&COM3,channelack,FIXED_LENGTH+1+CHECKSUM);
																break;
													}
													break;
											 case CAN_CHANNEL_1:
													//RTU_DEBUG("Set CAN channel 1!\r\n");
												  channel = CAN_CHANNEL_1;
												  channelack[3] = CAN_CHANNEL_1;												
													senddatapacket(&COM3,channelack,FIXED_LENGTH+1+CHECKSUM);
												  break;
											 case CAN_CHANNEL_2:
													//RTU_DEBUG("Set CAN channel 2!\r\n");
													channel = CAN_CHANNEL_2;
													channelack[3] = CAN_CHANNEL_2;												
													senddatapacket(&COM3,channelack,FIXED_LENGTH+1+CHECKSUM);
													break;
										}
										break;
								//GPIO
								case 0x90:
                     gpiotest(Precvpacket->command);            								
								     break;
							 }
							 //free(Precvpacket->data);
					}			
          break;
				}
		}
}
/*
********************************************************************************
*  ��������: SetUartBaudrate
*
*  ��������: ����UART������
*            
*
*  �������: data
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void SetUartBaudrate(u8 data)
{
	  u16 Baud;
	  u32 Baudrate = 0;
	
    if(data == 0x9)
		{
				Baud = 0x9600;
				Baudrate = 9600;
				ackbaudrate[3] = 0x9;
		}
		else if(data == 0x19)
		{
				Baud = 0x1920;
				Baudrate = 19200;
				ackbaudrate[3] = 0x19;
		}
		else if(data == 0x57)
		{
				Baud = 0x5760;
				Baudrate = 57600;
				ackbaudrate[3] = 0x57;
		}
		else if(data == 0x11)
		{
				Baud = 0x1152;
				Baudrate = 115200;
				ackbaudrate[3] = 0x11;
		}
		else if(data == 0x23)
		{
				Baud = 0x2304;
				Baudrate = 230400;
				ackbaudrate[3] = 0x23;
		}
		else if(data == 0x46)
		{
				Baud = 0x4608;
				Baudrate = 460800;
				ackbaudrate[3] = 0x46;
		}
		Com3Init(&COM3, Baudrate, COM_PARA_8N1);
		
		MemWriteData(PAGE_ADDR,&Baud,1);
		DelayMS(1000);
	  senddatapacket(&COM3,ackbaudrate,FIXED_LENGTH+1+CHECKSUM);
}


