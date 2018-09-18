/*
********************************************************************************
*  Copyright (C), 2009-2013, 合众思壮深圳研发中心
*
*  项目名称：E7759A
*  
*  文件名称: UartProcess.c
*
*  文件描述：串口数据处理程序
*             
*             
*  创 建 者: 韦哲轩
*
*  创建日期：2017-10-9 
*
*  版 本 号：V1.0
*
*  修改记录： 
*             
*      1. 日    期： 
*         修 改 人： 
*         所作修改： 
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
*  外部变量声明                                                                                                       
********************************************************************************
*/
extern u16 RxCounter;
extern bool is_RxFrame;
extern u8 batteryvolt[10];
/*
********************************************************************************                                                                 
*  内部变量定义                                                                                                         
********************************************************************************
*/
bool is_uart1echo = FALSE;
bool timer3onoff = FALSE; //FALSE表示定时器3是关闭的
bool is_accon_ack  = FALSE; //FALSE表示没有收到accon应答
bool is_accoff_ack  = FALSE;//FALSE表示没有收到accoff应答
bool cancel_shutdown  = FALSE;//FALSE表示没有收到取消关机命令
bool diagnosticmode = FALSE; //TRUE表示诊断模式
bool is_diagnostic_live = FALSE; //TRUE表示诊断激活

CANPACKET sendcanpacket;
PACKET UART3Recvpacket; //定义UART3接收到的报文
UART1DATASTRUCT  UART1Data;//定义处理第三方串口（UART1）数据结构体
UART3DATASTRUCT  UART3Data;//定义接收UART3原始数据

//点火
u8 turnoncmd[6] = {0x30, 0x00, 0x01,0x01};
//熄火
u8 turnoffcmd[6] = {0x30, 0x00, 0x01, 0x00};
//版本
u8 version[10] = {0x21, 0x00, 0x04,'v','3','.','1'};
//UART波特率
u8 ackbaudrate[6] = {0x31,0x00, 0x01};


DiagnosticRequestHandle RequestHandle; 
/*
********************************************************************************                                                                  
*  内部函数声明                                                                                                        
********************************************************************************
*/
void senddatapacket(COMX * pcom,u8* data,u16 length);
void recvpacketparse(PACKET* Precvpacket);
void SetUartBaudrate(u8 data);
/*
********************************************************************************                                                                  
*  外部函数声明                                                                                                        
********************************************************************************
*/
extern void DiagnosticResponsePro(DiagnosticResponse* response);
/*
********************************************************************************
*  函数名称: StructInit
*
*  功能描述: 结构体初始化
*            
*
*  输入参数: 无
*
*  输出参数: 无
*
*  返 回 值: 无
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
*  函数名称: UART3_Data_Pro
*
*  功能描述: 处理UART3数据
*            
*
*  输入参数: dat
*
*  输出参数: 无
*
*  返 回 值: 无
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
			//处理BINTRANS_DLE和BINTRANS_STX
			case 0:
		       if(UART3Data.Len<1)
					 {
								UART3Data.Dat[UART3Data.Len++] = dat;
					 }
					 //若遇到BINTRANS_DLE，则往下判断
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
										  //若遇到第二个字节是BINTRANS_STX，则表明可能是一帧数据。
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
					// 找结尾字节
					if(UART3Data.Dat[UART3Data.Len - 2] == BINTRANS_DLE && 
            UART3Data.Dat[UART3Data.Len - 1] == BINTRANS_ETX)
					{  
            // 转义字节
            for(dle = 0, k = 0, i = 2; i < UART3Data.Len - 2; i++)
            {
							  //将<DLE><DLE>转换为<DLE>
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
						
						//取当前长度
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
									 //当前数据包长度(不包括校验)
									 UART3Recvpacket.CurrentLen = UART3Data.Len-2;
									 UART3Recvpacket.data = (u8*)malloc(UART3Recvpacket.CurrentLen);
									 if(UART3Recvpacket.data == NULL)
									 {
										  return;
									 }
									 //将数据包拷贝到recvpacket.data
									 memcpy(UART3Recvpacket.data,UART3Data.Esc,UART3Recvpacket.CurrentLen);
									 UART3Recvpacket.index = 0;	
									 UART3Recvpacket.codecState = CODEC_STATE_PACKET_TYPE;
									 //对UART3报文进行处理
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
        // 超出最大长度
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
*  函数名称: UART1_Data_Pro
*
*  功能描述: 将接收到不定长的UART1数据进行处理
*            
*
*  输入参数: data，length
*
*  输出参数: 无
*
*  返 回 值: 无
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
*  函数名称: senddatapacket
*
*  功能描述: 往串口发送协议数据包
*            
*
*  输入参数: pcom，data，length
*
*  输出参数: 无
*
*  返 回 值: 无
*
********************************************************************************
*/
//length=type(一个字节）+当前长度（两个字节) +数据 +校检和（两个字节）

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
*  函数名称: RevUart1Data
*
*  功能描述: 接收UART1数据
*            
*
*  输入参数: 无
*
*  输出参数: 无
*
*  返 回 值: 无
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
					 //处理UART1数据
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
*  函数名称: recvpacketparse
*
*  功能描述: 接收数据包，并处理。
*            
*
*  输入参数: Precvpacket
*
*  输出参数: 无
*
*  返 回 值: 无
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
	
		
	  //当指针为空，且长度数据长度小于等于0时，退出函数。
    if(Precvpacket == NULL && Precvpacket->CurrentLen <= 0)
		{
			  return;
		}
	 	for(index = 0; index < Precvpacket->CurrentLen; index++)
		{
				iterator = Precvpacket->data[index];
			
			  switch(Precvpacket->codecState)
		    {
					 //报文类型
					 case CODEC_STATE_PACKET_TYPE:
								Precvpacket->PacketType = iterator;
					      Precvpacket->codecState = CODEC_STATE_CURRENT_PACKET_LENGTH;
					  break;
					 //报文当前长度
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
					//有效载荷（应用数据）
					case CODEC_STATE_PAYLOAD:
						 //接收E7759的CAN数据,并将CAN数据发送给CAN分析仪
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
					  //接收E7759的诊断请求,例如: 0x55 0x03 0x60 0x00 0x08 0x01 0x07 0xDF 0x00 0x00 0x02 0x01 0x00 Check A Check B 0x55 0x03
						//读取VIN码
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
						//J1939 报文请求
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
						//接收到E7759发来的第三方数据,并通过COM1将数据发送给第三方设备
						else if(Precvpacket->PacketType == 0x51 || Precvpacket->PacketType == 0x10)
						{
							  
								Precvpacket->revthird[FIXED_LENGTH+Precvpacket->index++] = iterator;
							 
								if(Precvpacket->index == CurrentPacketLen && CurrentPacketLen >= 0)
								{
									 Precvpacket->index = 0;
									 //UART1串口回显
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
						//COM1接收到第三方设备的数据，然后通过COM3将数据发给E7759
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
									  //接收到ACC ON或OFF的应答
										case 0x31:
												switch(Precvpacket->command)
												{
													//接收到到ACC ON应答
													case 0x01:
                              if(timer3onoff == TRUE)
                              {
                                TIM_Cmd(TIM3, DISABLE);																
																timer3onoff = FALSE;
                              }																
															is_accon_ack = TRUE;
															is_accoff_ack =	FALSE;	
															break;
													//接收到ACC OFF应答
													case 0x00:
															is_accon_ack = FALSE;
															is_accoff_ack = TRUE;
	                            break;													
											}
											break;
										//接收E7759的控制命令
									case 0x20:
										  switch(Precvpacket->command)
											{
												//获取软件版本命令
												case 0x33:														
												   	 senddatapacket(&COM3,version,FIXED_LENGTH+4+CHECKSUM);
												     break;
												//获取电池电压
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
												//接收到Android应用取消关机
												case 0x02:
                             cancel_shutdown = TRUE;												
														 break;
												//关机
												case 0x03:
									           GPIO_ResetBits(GPIOA, GPIO_Pin_11);							
														 break;
												//远程升级
												case 0x04:
														 TIM_Cmd(TIM5, ENABLE);
														 break;
												//切换到500K命令
												case 0x50:
													   //RTU_DEBUG("Setting 500K!\r\n");
												     CAN1Init(&Can1, CANX_BAUD_500K,1);
												     canbandrate = 0x500;
														 MemWriteData(PAGE_ADDR+8,&canbandrate,1);
												     DelayMS(1000);
												     ackbaudrate[3] = 0x50;
														 senddatapacket(&COM3,ackbaudrate,FIXED_LENGTH+1+CHECKSUM);
														 break;
												//切换到250K命令
												case 0x25:
														 //RTU_DEBUG("Setting 250K!\r\n");
												     CAN1Init(&Can1, CANX_BAUD_250K,1);
												     canbandrate = 0x250;
												     MemWriteData(PAGE_ADDR+8,&canbandrate,1);
												     DelayMS(1000);
												     ackbaudrate[3] = 0x25;
												     senddatapacket(&COM3,ackbaudrate,FIXED_LENGTH+1+CHECKSUM);
														 break;
												//9600波特率
												case 0x9:
														 //RTU_DEBUG("Setting 9600Baudrate!\r\n");
												     SetUartBaudrate(0x09);
														 break;
											 //19200波特率
											 case 0x19:
														 //RTU_DEBUG("Setting 19200Baudrate!\r\n");
												     SetUartBaudrate(0x19);
														 break;
											//57600波特率
										 	case 0x57:
														 //RTU_DEBUG("Setting 57600Baudrate!\r\n");
												     SetUartBaudrate(0x57);
														 break;
											//115200波特率
											case 0x11:
														 //RTU_DEBUG("Setting 115200Baudrate!\r\n");
												     SetUartBaudrate(0x11);
														 break;
											//230400波特率
											case 0x23:
														 //RTU_DEBUG("Setting 230400Baudrate!\r\n");
												     SetUartBaudrate(0x23);
														 break;
											//460800波特率
											case 0x46:
														 //RTU_DEBUG("Setting 460800Baudrate!\r\n");
												     SetUartBaudrate(0x46);
														 break;
											//初始化J1939协议(CAN)
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
*  函数名称: SetUartBaudrate
*
*  功能描述: 设置UART波特率
*            
*
*  输入参数: data
*
*  输出参数: 无
*
*  返 回 值: 无
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



