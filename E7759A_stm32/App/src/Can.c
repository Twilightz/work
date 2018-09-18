/*
********************************************************************************
*  Copyright (C), 2009-2013, 合众思壮西安研发中心
*
*  项目名称：xxxx
*  
*  文件名称: xxxx.c
*
*  文件描述：xxxx
*             
*             
*  创 建 者: 皇海明
*
*  创建日期：2013-03-18 
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
#include "Can.h"

#include "Public.h"
#include "string.h"
#include "shell.h"
#include "Exti.h"
#include "bsp_iwdg.h"
#include "Obd.h"
#include "UartProcess.h"
#include "Delay.h"

extern u8 sendstatus;
/*
外部变量声明
*/
extern bool diagnosticmode;
extern CANPACKET sendcanpacket;
extern DiagnosticRequestHandle* PrequestHandle;
/*
********************************************************************************
*  函数名称: CanToUtcp
*
*  功能描述: 将CAN总线数据直接打包成UTCP二进制数据传输协议发送。
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
void CanToUtcp(u8 channel, CANX * pcan, COMX * pcom)
{

 u32 StdId;     // 标准ID
    u32 ExtId;     // 扩展ID
    u8  IDE;       //判断是标准ID还是扩展ID
    
    u8 i, buf[20];
    CANQUEDATA data;
	  u16 CurrentPacketLen = 0;
    DiagnosticResponse response;
    if(channel != 1 && channel != 2)
    {
        return;
    }
    
    else if(pcan == NULL || pcom == NULL)
    {
        return;
    }
    while(CanReceive(pcan, &data))
    {
        buf[0] = channel;
		#if 0
        buf[1] = (u8)(data.Id >> 24);
        buf[2] = (u8)(data.Id >> 16);
        buf[3] = (u8)(data.Id >> 8);
        buf[4] = (u8)(data.Id >> 0);
dat.IDE = (u8)0x04 & CAN1->sFIFOMailBox[CAN_FIFO0].RIR;
        if(dat.IDE == CAN_ID_STD)
        {
			dat.StdId = (u32)0x000007FF & (CAN1->sFIFOMailBox[CAN_FIFO0].RIR >> 21);
        }
		else if(dat.IDE == CAN_ID_EXT)
		{
			dat.ExtId = (u32)0x1FFFFFFF & (CAN1->sFIFOMailBox[CAN_FIFO0].RIR >> 3);
		}
		#else
		buf[1] = (u8)(data.Id >> 24);
        buf[2] = (u8)(data.Id >> 16);
        buf[3] = (u8)(data.Id >> 8);
        buf[4] = (u8)(data.Id >> 0);
		#endif
        buf[5] = data.Dlc;
				for (i = 0; i < data.Dlc; i++)
        {
            buf[6 + i] = data.Data[i];
        }
				if (data.Dlc < 8)
				{
						memset(buf + 6 + data.Dlc, 0, 8 - data.Dlc);
				}
				//向串口发送CAN数据
				IWDG_Feed();

				IDE = (u8)0x04 & data.Id;

				RTU_DEBUG("receive:IDE=%x\r\n",IDE);
				if(IDE == CAN_ID_STD)
				{
					data.Id = (u32)0x000007FF & (data.Id >> 21);
				}
				else if(IDE == CAN_ID_EXT)
				{
					data.Id = (u32)0x1FFFFFFF & (data.Id >> 3);
				}
			
				
				RTU_DEBUG("receive:data.Id=%x\r\n",data.Id);
				
				//response.Id = data.Id;
				//response.length = data.Dlc;
				//memcpy(response.DataField,data.Data,data.Dlc);
				
				//for(i=0;i<data.Dlc;i++)
				//			RTU_DEBUG("%x ",response.DataField[i]);
				//PrequestHandle->appcallback(&response);
				#if 0
				//J1939_ResponsePro(&response);
				#endif
				//if(diagnosticmode == TRUE)
				//{
				   // RTU_DEBUG("buf[0]=%x\r\n",buf[0]);
					//	RTU_DEBUG("buf[1]=%x\r\n",buf[1]);
				//RTU_DEBUG("buf[2]=%x\r\n",buf[2]);
				//RTU_DEBUG("buf[3]=%x\r\n",buf[3]);
				//RTU_DEBUG("buf[4]=%x\r\n",buf[4]);
				//RTU_DEBUG("buf[5]=%x\r\n",buf[5]);
				  //  RTU_DEBUG("data.Id=%x\r\n",data.Id);
						//response.Id = data.Id >> 21;
						//RTU_DEBUG("response.Id =%x\r\n",response.Id );
						//response.length = data.Dlc;
						//for(i=0;i<data.Dlc;i++)
						//	RTU_DEBUG("%x ",data.Data[i]);
				    //RTU_DEBUG("\r\n");
						//memcpy(response.DataField,data.Data,data.Dlc);
				#if 0
						//PrequestHandle->networkcallback(&response);
        #endif				
				//}
				//else
				//{
						//远程帧
						if((buf[4]&0x02) == 0x02)
						{
								sendcanpacket.candata[0] = 0x41;
								CurrentPacketLen = CANCHANNEL+CANID;
								c16toa(&CurrentPacketLen,&sendcanpacket.candata[1]);
								memcpy(&sendcanpacket.candata[FIXED_LENGTH],buf,CurrentPacketLen);
								senddatapacket(&COM3,sendcanpacket.candata,FIXED_LENGTH+CurrentPacketLen+CHECKSUM);
						}
						//数据帧
					else
						{
								sendcanpacket.candata[0] = 0x41;
								CurrentPacketLen = data.Dlc+6;
								c16toa(&CurrentPacketLen,&sendcanpacket.candata[1]);
								memcpy(&sendcanpacket.candata[FIXED_LENGTH],buf,14);
								senddatapacket(&COM3,sendcanpacket.candata,FIXED_LENGTH+14+CHECKSUM);
					  }

			  //}				

    }
}

/*
********************************************************************************
*  函数名称: UartDataToCanData
*
*  功能描述: 将收到的uart数据进行打包
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

void UartDataToCanData(u8 len, u8 *Data)
{
	CANQUEDATA data;
	u8 i;
	
	if(Data == NULL && len == 0)
	{
		return;
	}
	memset(&data, 0, sizeof(CANQUEDATA));

	data.Id = ((Data[1] << 24) | (Data[2] << 16) | (Data[3] << 8) | Data[4]);
	data.Dlc = Data[5];
	 
	for(i = 0; i < Data[5]; i++)
  {
		data.Data[i] = Data[i+6];
  }
	 
	CanTransmit(&Can1, &data);
}

/*
一、过滤器完全无效  接收到的标示符全部通过
0x1314 二进制码：0000 0000 0000 0000 0001 0011 0001 0100 
CAN_Filter xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
CAN_FilterMask  0000 0000 0000 0000 0000 0000 0000 0000


*/
void CAN_SetFilterStd()
{
	 u8 i,num;
	 u16 tmp,mask;
	 u16 StdIdArray[2] = {0x7E8,0x7E9};

   CAN_FilterInitTypeDef  CAN_FilterInitStructure;
  
   RTU_DEBUG("CAN_SetFilterStd\r\n");
	 CAN_FilterInitStructure.CAN_FilterNumber = 0;
   CAN_FilterInitStructure.CAN_FilterMode   = CAN_FilterMode_IdMask;
   CAN_FilterInitStructure.CAN_FilterScale  = CAN_FilterScale_32bit;
	 CAN_FilterInitStructure.CAN_FilterIdHigh         = (StdIdArray[0] << 5)&0xffff;
	 RTU_DEBUG("CAN_FilterInitStructure.CAN_FilterIdHigh=%x\r\n",CAN_FilterInitStructure.CAN_FilterIdHigh);
	 CAN_FilterInitStructure.CAN_FilterIdLow          = 0;
	
	 num = sizeof(StdIdArray)/sizeof(StdIdArray[0]);
   for(i=0;i<num;i++)
   {
			tmp = (StdIdArray[0] << 5)&(StdIdArray[i] << 5);
			mask = tmp;
   }
	 RTU_DEBUG("mask=%x\r\n",mask);
				
	 CAN_FilterInitStructure.CAN_FilterMaskIdHigh     = mask;
   CAN_FilterInitStructure.CAN_FilterMaskIdLow      = 0xffff; 
   CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
   CAN_FilterInitStructure.CAN_FilterActivation     = ENABLE;
   CAN_FilterInit(&CAN_FilterInitStructure);
}

void CAN_SetFilterExt()
{
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   
	CAN_FilterInitStructure.CAN_FilterNumber=0;
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	//CAN_FilterInitStructure.CAN_FilterIdHigh=(((u32)ID<<3)&0xffff0000)>>16;
	//CAN_FilterInitStructure.CAN_FilterIdLow=(((u32)ID<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xffff;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xffff;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xffff;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
}


/*
 * bit2: 0为标准ID   1为扩展ID
 *
 */
DiagnoticIdType Get_Id_type(u32 id)
{
	  
	  return id&0x04;
}

DiagnoticFrameType Get_Frame_type(u32 id)
{
	  return id&0x02;
}

void Send_singleframe(void *handle)
{
	  bool Is_Filtered = FALSE;
		u8 i = 0;
		CANQUEDATA candata;
	
	  if(handle == NULL)
			 return;
		
		RTU_DEBUG("Send_singleframe\r\n");
		PrequestHandle = (DiagnosticRequestHandle*)handle;
		candata.Dlc = 0x08;
		candata.Id = PrequestHandle->Request.Id;
    memcpy(&candata.Data[0], PrequestHandle->Request.DataField,0x08);
		
		Is_Filtered = PrequestHandle->Request.BitField&0x01;
		if(Is_Filtered == TRUE)
		{
			if(PrequestHandle->Request.Ide == DIAGNOSTIC_CAN_ID_STANDARD)
			{
				 CAN_SetFilterStd();
			}
			else if(PrequestHandle->Request.Ide == DIAGNOSTIC_CAN_ID_EXTENDED)
			{
				 //CAN_SetFilterExt();
			}	
		}
		CanTransmit(&Can1, &candata);
}

void J1939_ReceiveMessages(u8 channel, CANX * pcan, COMX * pcom)
{

    u8  IDE;       //判断是标准ID还是扩展ID    
    u8 i, buf[20];
    CANQUEDATA data;
    DiagnosticResponse response;
    if(channel != 1 && channel != 2)
    {
        return;
    }
    
    else if(pcan == NULL || pcom == NULL)
    {
        return;
    }
    while(CanReceive(pcan, &data))
    {
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
				//向串口发送CAN数据
				IDE = (u8)0x04 & data.Id;
				if(IDE == CAN_ID_STD)
				{
					data.Id = (u32)0x000007FF & (data.Id >> 21);
				}
				else if(IDE == CAN_ID_EXT)
				{
					data.Id = (u32)0x1FFFFFFF & (data.Id >> 3);
				}		
				response.Id = data.Id;
				RTU_DEBUG("response.Id=%x\r\n",response.Id);
				response.length = data.Dlc;
				for(i=0;i<data.Dlc;i++)
				  RTU_DEBUG("%x ",data.Data[i]);
				memcpy(response.DataField,data.Data,data.Dlc);
				//回调函数
				//PrequestHandle->appcallback(&response);
    }
}

void J1939_CAN_SetFilterExt(u32 ExtId)
{
  //u8 num,i;
	u32 tmp,mask=0;
	//u32 ExtIdArrary[2] = {0x18FEE900,0x18F00900};
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   
  RTU_DEBUG("J1939_CAN_SetFilterExt\r\n");	
	//RTU_DEBUG("ID=%x\r\n",ID);	
	
	CAN_FilterInitStructure.CAN_FilterNumber=0;
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	
	CAN_FilterInitStructure.CAN_FilterIdHigh=((ExtId<<3)>>16)&0xffff;
	CAN_FilterInitStructure.CAN_FilterIdLow=((ExtId<<3)&0xffff)|CAN_ID_EXT;
	
	//CAN_FilterInitStructure.CAN_FilterIdHigh=((ExtIdArrary[0]<<3)>>16)&0xffff;
	//CAN_FilterInitStructure.CAN_FilterIdLow=((ExtIdArrary[0]<<3)&0xffff)|CAN_ID_EXT;
	
	mask = 0x1fffffff;
	
	//num = sizeof(ExtIdArrary)/sizeof(ExtIdArrary[0]);
	//RTU_DEBUG("num=%x\r\n",num);
	//for(i=0;i<num;i++)
	//{
		  //RTU_DEBUG("num=%x\r\n",num);
	//		tmp = ExtIdArrary[i]^(~ExtIdArrary[0]);
	//	  mask &=tmp;
	//}
	tmp = ExtId^(~ExtId);
	mask &=tmp;
	mask <<= 3;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=(mask>>16)&0xffff;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=(mask&0xffff)|0x02;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
}

static u32 J1939_Translate_IdExt (u8 Priority,u8 Pdu_PF,u8 Pdu_Ps,u8 Pdu_Sa)
{
	u32 Id_Ext = 	0x00000000;
	Id_Ext     =    Id_Ext|Priority<<26;
	Id_Ext     =    ((Id_Ext>>16)|Pdu_PF)<<16;
	Id_Ext     =    ((Id_Ext>>8)|Pdu_Ps)<<8;
	Id_Ext     =    Id_Ext|Pdu_Sa;
	return     Id_Ext;
}

void J1939_CAN_Transmit(J1939_MESSAGE_T *MsgPtr,void *handle)
{
	  u32 i,j;
	  u8 SendStatus;
	  CANQUEDATA candata;
	
	  //发送CAN帧之前，需要确定是数据帧、远程帧、标准ID还是扩展ID？
	  candata.RTR = CAN_RTR_Data;
	  candata.IDE = CAN_Id_Extended;
	
	  PrequestHandle = (DiagnosticRequestHandle*)handle;
	
	  candata.Id  = J1939_Translate_IdExt(MsgPtr->Priority,MsgPtr->PDUFormat,MsgPtr->PDUSpecific,MsgPtr->SourceAddress);
	  candata.Dlc = MsgPtr->DataLength;
	  memcpy(&candata.Data[0], &MsgPtr->Data[0],candata.Dlc);
		RTU_DEBUG("candata.Id=%x\r\n",candata.Id);
		RTU_DEBUG("candata.Dlc=%d\r\n",candata.Dlc);
	  for(i=0;i<candata.Dlc;i++)
	    RTU_DEBUG("%x ",candata.Data[i]);
		for(j=0;j<3;j++)
		{
				CanTransmit(&Can1, &candata);
			  i = 0;
				while(sendstatus != CAN_TxStatus_Ok && i<0x800)
				{
					i++;
				}
				if(sendstatus == CAN_TxStatus_Ok)
				{
					break;
				}
				if(i==0x800)
				{
					if(j==2)
					{
							RTU_DEBUG("respone send fail");//输出应答
					}
				}
		}
}




