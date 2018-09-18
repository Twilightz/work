/*
********************************************************************************
*  Copyright (C), 2009-2013, ����˼׳�����з�����
*
*  ��Ŀ���ƣ�E7759A
*  
*  �ļ�����: UartProcess.c
*
*  �ļ��������������ݴ������
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


#include "Shell.h"
#include "Obd.h"
#include <stdlib.h>
#include "Public.h"
#include "Can.h"
#include "Flash.h"
#include "Adc.h"
#include "Delay.h"
#include "string.h"
#include "Delay.h"
#include "UartProcess.h"
#include "J1939.h"

/*
********************************************************************************                                                                  
*  �ⲿ��������                                                                                                       
********************************************************************************
*/
extern u16 RxCounter;
extern bool is_RxFrame;
extern u8 batteryvolt[10];
/*
********************************************************************************                                                                 
*  �ڲ���������                                                                                                         
********************************************************************************
*/
bool is_uart1echo = FALSE;
bool timer3onoff = FALSE; //FALSE��ʾ��ʱ��3�ǹرյ�
bool is_accon_ack  = FALSE; //FALSE��ʾû���յ�acconӦ��
bool is_accoff_ack  = FALSE;//FALSE��ʾû���յ�accoffӦ��
bool cancel_shutdown  = FALSE;//FALSE��ʾû���յ�ȡ���ػ�����
bool diagnosticmode = FALSE; //TRUE��ʾ���ģʽ
bool is_diagnostic_live = FALSE; //TRUE��ʾ��ϼ���

CANPACKET sendcanpacket;
PACKET UART3Recvpacket; //����UART3���յ��ı���
UART1DATASTRUCT  UART1Data;//���崦����������ڣ�UART1�����ݽṹ��
UART3DATASTRUCT  UART3Data;//�������UART3ԭʼ����

//���
u8 turnoncmd[6] = {0x30, 0x00, 0x01,0x01};
//Ϩ��
u8 turnoffcmd[6] = {0x30, 0x00, 0x01, 0x00};
//�汾
u8 version[10] = {0x21, 0x00, 0x04,'v','3','.','1'};
//UART������
u8 ackbaudrate[6] = {0x31,0x00, 0x01};


DiagnosticRequestHandle RequestHandle; 
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
extern void DiagnosticResponsePro(DiagnosticResponse* response);
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
	  UART3Recvpacket.data = NULL;
	  UART3Recvpacket.sendthird = NULL;
	  UART3Recvpacket.revthird = NULL;
	
	  UART1Data.Dat = NULL;
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
						//
						if(k == FIXED_LENGTH+CurrentPacketLen+CHECKSUM)
						{
								UART3Data.Len = k;
								UART3Recvpacket.cha = UART3Data.Esc[UART3Data.Len-2];
								UART3Recvpacket.chb = UART3Data.Esc[UART3Data.Len-1];	
						
								CalculateChecksum(UART3Data.Esc,UART3Data.Len-2,&cha,&chb);
										 
								if(UART3Recvpacket.cha == cha && UART3Recvpacket.chb == chb)
								{
									 //��ǰ���ݰ�����(������У��)
									 UART3Recvpacket.CurrentLen = UART3Data.Len-2;
									 UART3Recvpacket.data = (u8*)malloc(UART3Recvpacket.CurrentLen);
									 if(UART3Recvpacket.data == NULL)
									 {
										  return;
									 }
									 //�����ݰ�������recvpacket.data
									 memcpy(UART3Recvpacket.data,UART3Data.Esc,UART3Recvpacket.CurrentLen);
									 UART3Recvpacket.index = 0;	
									 UART3Recvpacket.codecState = CODEC_STATE_PACKET_TYPE;
									 //��UART3���Ľ��д���
									 recvpacketparse(&UART3Recvpacket);
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
*  ��������: UART1_Data_Pro
*
*  ��������: �����յ���������UART1���ݽ��д���
*            
*
*  �������: data��length
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
void UART1_Data_Pro(u8* data,u16 length)
{
		u16 CurrentPacketLen;
			
	  if(data == NULL)
		{
			 return;
		}
		data[0] = 0x50;
		CurrentPacketLen = length;
		c16toa(&CurrentPacketLen,&data[1]);
		senddatapacket(&COM3,data,FIXED_LENGTH+CurrentPacketLen+CHECKSUM);
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
*  ��������: RevUart1Data
*
*  ��������: ����UART1����
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
void RevUart1Data(void)
{
	  u16 Uart1RxLength = 0;

	  if(is_RxFrame == TRUE)
	  {
				if(RxCounter > 0 && RxCounter <= 1000)
				{
					 UART1Data.Dat = (u8*)malloc(FIXED_LENGTH+RxCounter+CHECKSUM);
					 if(UART1Data.Dat == NULL)
					 {
							 return;
					 }
					 Uart1RxLength = ComxGetData(&COM1,&UART1Data.Dat[3],RxCounter);
					 ComxClrRcv(&COM1);
					 RxCounter = 0;
					 //����UART1����
					 if(is_uart1echo == TRUE)
					 {
						  ComxPutData(&COM3,&UART1Data.Dat[3],Uart1RxLength);
						  is_uart1echo = FALSE;
					 }
					 else
					 {
							UART1_Data_Pro(UART1Data.Dat,Uart1RxLength);
					 }
					 free(UART1Data.Dat);
				}
				else if(RxCounter > 1000)
        {
					 ComxClrRcv(&COM1);
				}
        RxCounter = 0;				
				is_RxFrame = FALSE;
				Uart1RxLength = 0;
		}	
}


/*
********************************************************************************
*  ��������: recvpacketparse
*
*  ��������: �������ݰ���������
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
void recvpacketparse(PACKET* Precvpacket)
{
	  u8 i = 0;
	  u8 iterator = 0;
	  u16 index = 0;
	  u16 canbandrate = 0;
	  u16 CurrentPacketLen = 0;
	  u32 canid = 0;
	  u8 channel = 0;
	  static u32 count = 0;
	
		
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
									
									  if(Precvpacket->PacketType == 0x40 || Precvpacket->PacketType == 0x60 || Precvpacket->PacketType == 0x70)
										{
												Precvpacket->candata = (u8*)malloc(CurrentPacketLen);
											  if(Precvpacket->candata == NULL)
													return;
										}
										else if(Precvpacket->PacketType == 0x51 || Precvpacket->PacketType == 0x10)
										{
												Precvpacket->revthird = (u8*)malloc(CurrentPacketLen);
											  if(Precvpacket->revthird == NULL)
												  return;
										}
										else if(Precvpacket->PacketType == 0x50)
										{
												Precvpacket->sendthird = (u8*)malloc(CurrentPacketLen);
											  if(Precvpacket->sendthird == NULL)
												  return;
										}
										Precvpacket->codecState = CODEC_STATE_PAYLOAD;								 
							  }			
							break;
					//��Ч�غɣ�Ӧ�����ݣ�
					case CODEC_STATE_PAYLOAD:
						 //����E7759��CAN����,����CAN���ݷ��͸�CAN������
					  if(Precvpacket->PacketType == 0x40)
						{		
								Precvpacket->candata[Precvpacket->index++] = iterator;
								if(Precvpacket->index == CurrentPacketLen && CurrentPacketLen >= 0)
							  {
									  diagnosticmode = FALSE;
									  count++;
									  //RTU_DEBUG("count=%d\r\n",count);
										UartDataToCanData(CurrentPacketLen,Precvpacket->candata);
										free(Precvpacket->candata);
										free(Precvpacket->data);

										Precvpacket->index = 0;  
								}

						}
					  //����E7759���������,����: 0x55 0x03 0x60 0x00 0x08 0x01 0x07 0xDF 0x00 0x00 0x02 0x01 0x00 Check A Check B 0x55 0x03
						//��ȡVIN��
						//0x55 0x03 0x60 0x00 0x08 0x01 0x07 0xDF 0x00 0x00 0x02 0x09 0x02 Check A Check B 0x55 0x03
					  else if(Precvpacket->PacketType == 0x60)
						{		
								Precvpacket->candata[Precvpacket->index++] = iterator;
								if(Precvpacket->index == CurrentPacketLen && CurrentPacketLen >= 0)
							  {
									  diagnosticmode = TRUE;
									  RequestHandle.Request.buf = (u8*)malloc(CurrentPacketLen);
									  if(RequestHandle.Request.buf == NULL)
											 return;
									  memcpy(RequestHandle.Request.buf,Precvpacket->candata,CurrentPacketLen);
									  diagnostic_request(&RequestHandle,DiagnosticResponsePro);
									  free(Precvpacket->candata);
										free(Precvpacket->data);	
										Precvpacket->index = 0;  
								}
						}
						//J1939 ��������
						else if(Precvpacket->PacketType == 0x70)
						{
							  Precvpacket->candata[Precvpacket->index++] = iterator;
							  if(Precvpacket->index == CurrentPacketLen && CurrentPacketLen >= 0)
							  {
									 RequestHandle.Request.buf = (u8*)malloc(CurrentPacketLen);
									 if(RequestHandle.Request.buf == NULL)
											 return;
									 memcpy(RequestHandle.Request.buf,Precvpacket->candata,CurrentPacketLen);
									 RTU_DEBUG("CurrentPacketLen=%d\r\n",CurrentPacketLen);
									 for(i=0;i<CurrentPacketLen;i++)
									    RTU_DEBUG("%x ",RequestHandle.Request.buf[i]);
							
									 J1939_request(&RequestHandle,J1939_ResponsePro);
									 
									 free(Precvpacket->candata);
									 free(Precvpacket->data);
									 Precvpacket->index = 0;  
								}
						}
						//���յ�E7759�����ĵ���������,��ͨ��COM1�����ݷ��͸��������豸
						else if(Precvpacket->PacketType == 0x51 || Precvpacket->PacketType == 0x10)
						{
							  
								Precvpacket->revthird[FIXED_LENGTH+Precvpacket->index++] = iterator;
							 
								if(Precvpacket->index == CurrentPacketLen && CurrentPacketLen >= 0)
								{
									 Precvpacket->index = 0;
									 //UART1���ڻ���
									 if(Precvpacket->PacketType == 0x10)
									 {
										  is_uart1echo = TRUE;
										  Precvpacket->revthird[0] = 0x11;
										  memcpy(&Precvpacket->revthird[1],Precvpacket->CurrentPacketLen,2);
										  senddatapacket(&COM1,&Precvpacket->revthird[0],FIXED_LENGTH+CurrentPacketLen+CHECKSUM);
									 }
									 else if(Precvpacket->PacketType == 0x51)
									 {
											Precvpacket->revthird[0] = Precvpacket->PacketType;
											memcpy(&Precvpacket->revthird[1],Precvpacket->CurrentPacketLen,2);
											ComxPutData(&COM1,&Precvpacket->revthird[FIXED_LENGTH],CurrentPacketLen);
									 }
									 free(Precvpacket->revthird);
									 free(Precvpacket->data);
								}						
						}
						//COM1���յ��������豸�����ݣ�Ȼ��ͨ��COM3�����ݷ���E7759
						else if(Precvpacket->PacketType == 0x50)
						{
								Precvpacket->sendthird[FIXED_LENGTH+CHECKSUM+Precvpacket->index++] = iterator;
							 
							  if(Precvpacket->index == CurrentPacketLen && CurrentPacketLen >= 0)
								{	
									 Precvpacket->index = 0;									 
                   Precvpacket->sendthird[0] = Precvpacket->PacketType;
									 memcpy(&Precvpacket->sendthird[1],Precvpacket->CurrentPacketLen,2);
									 senddatapacket(&COM3,Precvpacket->sendthird,FIXED_LENGTH+CurrentPacketLen+CHECKSUM);
									 free(Precvpacket->sendthird);
									 free(Precvpacket->data);
								}
					
						}
						else
						{ 
					      Precvpacket->command = iterator;
								switch(Precvpacket->PacketType)
								{
									  //���յ�ACC ON��OFF��Ӧ��
										case 0x31:
												switch(Precvpacket->command)
												{
													//���յ���ACC ONӦ��
													case 0x01:
                              if(timer3onoff == TRUE)
                              {
                                TIM_Cmd(TIM3, DISABLE);																
																timer3onoff = FALSE;
                              }																
															is_accon_ack = TRUE;
															is_accoff_ack =	FALSE;	
															break;
													//���յ�ACC OFFӦ��
													case 0x00:
															is_accon_ack = FALSE;
															is_accoff_ack = TRUE;
	                            break;													
											}
											break;
										//����E7759�Ŀ�������
									case 0x20:
										  switch(Precvpacket->command)
											{
												//��ȡ����汾����
												case 0x33:														
												   	 senddatapacket(&COM3,version,FIXED_LENGTH+4+CHECKSUM);
												     break;
												//��ȡ��ص�ѹ
												case 0x34:	
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
                             cancel_shutdown = TRUE;												
														 break;
												//�ػ�
												case 0x03:
									           GPIO_ResetBits(GPIOA, GPIO_Pin_11);							
														 break;
												//Զ������
												case 0x04:
														 TIM_Cmd(TIM5, ENABLE);
														 break;
												//�л���500K����
												case 0x50:
													   //RTU_DEBUG("Setting 500K!\r\n");
												     CAN1Init(&Can1, CANX_BAUD_500K,1);
												     canbandrate = 0x500;
														 MemWriteData(PAGE_ADDR+8,&canbandrate,1);
												     DelayMS(1000);
												     ackbaudrate[3] = 0x50;
														 senddatapacket(&COM3,ackbaudrate,FIXED_LENGTH+1+CHECKSUM);
														 break;
												//�л���250K����
												case 0x25:
														 //RTU_DEBUG("Setting 250K!\r\n");
												     CAN1Init(&Can1, CANX_BAUD_250K,1);
												     canbandrate = 0x250;
												     MemWriteData(PAGE_ADDR+8,&canbandrate,1);
												     DelayMS(1000);
												     ackbaudrate[3] = 0x25;
												     senddatapacket(&COM3,ackbaudrate,FIXED_LENGTH+1+CHECKSUM);
														 break;
												//9600������
												case 0x9:
														 //RTU_DEBUG("Setting 9600Baudrate!\r\n");
												     SetUartBaudrate(0x09);
														 break;
											 //19200������
											 case 0x19:
														 //RTU_DEBUG("Setting 19200Baudrate!\r\n");
												     SetUartBaudrate(0x19);
														 break;
											//57600������
										 	case 0x57:
														 //RTU_DEBUG("Setting 57600Baudrate!\r\n");
												     SetUartBaudrate(0x57);
														 break;
											//115200������
											case 0x11:
														 //RTU_DEBUG("Setting 115200Baudrate!\r\n");
												     SetUartBaudrate(0x11);
														 break;
											//230400������
											case 0x23:
														 //RTU_DEBUG("Setting 230400Baudrate!\r\n");
												     SetUartBaudrate(0x23);
														 break;
											//460800������
											case 0x46:
														 //RTU_DEBUG("Setting 460800Baudrate!\r\n");
												     SetUartBaudrate(0x46);
														 break;
											//��ʼ��J1939Э��(CAN)
											case 0x71:
												//CAN1Init(&Can1, CANX_BAUD_250K,0);
											  //ackbaudrate[3] = 0x71;
											  //senddatapacket(&COM3,ackbaudrate,FIXED_LENGTH+1+CHECKSUM);
												break;
										}               								
								    break;
							
							 }
							 free(Precvpacket->data);
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
	  Com1Init(&COM1, Baudrate, COM_PARA_8N1);
		Com3Init(&COM3, Baudrate, COM_PARA_8N1);
		
		MemWriteData(PAGE_ADDR,&Baud,1);
		DelayMS(1000);
	  senddatapacket(&COM3,ackbaudrate,FIXED_LENGTH+1+CHECKSUM);
}



