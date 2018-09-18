/*
********************************************************************************
*  Copyright (C), 2009-2013, 合众思壮深圳研发中心
*
*  项目名称：E7759A
*  
*  文件名称: Obd.c
*
*  文件描述：OBD II应用诊断程序
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
#include "Obd.h"
#include "CanDrv.h"
#include <string.h>
#include <stdlib.h>
#include "Shell.h"
#include "Public.h"
#include "Obd_Network.h"
#include "Can.h"
#include "UartProcess.h"
#include "Delay.h"


extern CHANNEL_TYPE_T channel;
DiagnosticRequestHandle* PrequestHandle = NULL;


void diagnostic_request(DiagnosticRequestHandle* Handle,DiagnosticResponseReceived  callback)
{
	  u8 A_PCI,A_PCItype;
	
	  if(Handle == NULL)
			 return;
		
		PrequestHandle = Handle;
		PrequestHandle->appcallback = callback;
		PrequestHandle->networkcallback = NULL;
		//获取位域
		Handle->Request.BitField = Handle->Request.buf[0];
		
		channel = Handle->Request.BitField>>1;
		//获取CAN ID
		ato32(&Handle->Request.buf[1],&Handle->Request.Id);
		RTU_DEBUG("diagnostic_request->channel=%d,Handle->Request.BitField=%d,Handle->Request.Id=%d\r\n",channel,Handle->Request.BitField,Handle->Request.Id);
		A_PCI = Handle->Request.buf[5];
		//判断诊断请求为单帧还是多帧
		A_PCItype = get_pci_type(A_PCI);
		//单帧
		if(A_PCItype == DIAGNOSTIC_SINGLE_FRAME)
		{
			  Handle->Request.Length = A_PCI;
			  Handle->Request.DataField[0] = A_PCI;
			  memcpy(&Handle->Request.DataField[1],&Handle->Request.buf[6],Handle->Request.Length);
		}//多帧
		else if(A_PCItype == DIAGNOSTIC_FIRST_FRAME || A_PCItype == DIAGNOSTIC_CONTINUOUS_FRAME)
		{
			
		}
		else if(A_PCItype == DIAGNOSTIC_FLOW_CONTROL)
		{
			  Handle->Request.Length = 0x07;
				Handle->Request.DataField[0] = A_PCI;
			  memcpy(&Handle->Request.DataField[1],&Handle->Request.buf[6],Handle->Request.Length);
		}
		free(Handle->Request.buf);
		diagnostic_request_extwork_send_msg(PrequestHandle);

}

u32 diagnostic_decode_pid_bitfield(DiagnosticResponse* response)
{
		if(response == NULL)
			return -1;
}

float diagnostic_decode_pid(DiagnosticResponse* response)
{	
	   u8 pci = 0;
	   int value;
	   float fvalue;
	   u16 CurrentLen = 0;
		 u8 Pidack[21] = {0x25};

     if(response == NULL)
	     return -1;
	   //RTU_DEBUG("diagnostic_decode_pid\r\n");
     switch(response->Pid){
			 
			 case PID_1:
				  break;
			 case MONITOR_STATUS:
				  break;
			 case FUEL_SYSTEM_STATUS:
				  break;
			 case LOAD_VALUE:
				  //RTU_DEBUG("LOAD_VALUE\r\n");
			    //fvalue = (response->DataField[3]*100.0)/255.0;
			    //RTU_DEBUG("fvalue=%f\r\n",fvalue);
				  break;
			 case ECT:
				  //RTU_DEBUG("ECT\r\n");
			    //value = response->DataField[3]-40;
			    //RTU_DEBUG("value=%d\r\n",value);
				  break;
			 case STFT_BANK1:
			 case LTFT_BANK1: 
			    //fvalue = (response->DataField[3]-128)*100.0/128.0;
			    //RTU_DEBUG("fvalue=%f\r\n",fvalue);
				  break;
			 case IMAP:
				  //RTU_DEBUG("IMAP\r\n");
			    //value = response->DataField[3];
			    //RTU_DEBUG("value=%d\r\n",value);
				  break;
			 case ENGINE_RPM:
				  //RTU_DEBUG("ENGINE_RPM\r\n");
			    //fvalue = ((response->DataField[3]*256)+response->DataField[4])/4;
			    //RTU_DEBUG("fvalue=%f\r\n",fvalue);
				  break;
			 case VEHICLE_SPEED:
				 	//RTU_DEBUG("VEHICLE_SPEED\r\n");
			    //value = response->DataField[3];
			    //RTU_DEBUG("value=%d\r\n",value);
				  break;
			 case LTA:
				  //RTU_DEBUG("LTA\r\n");
			    //fvalue = (response->DataField[3]/2.0)-64.0;
			    //RTU_DEBUG("fvalue=%f\r\n",fvalue);
			    break;
			 case IAT:
				 	//RTU_DEBUG("IAT\r\n");
			    //value = response->DataField[3]-40;
			    //RTU_DEBUG("value=%d\r\n",value);
			     break;
			 case AFR:
				  //RTU_DEBUG("AFR\r\n");
			    //fvalue = (response->DataField[3]*256.0)+response->DataField[4]/100.0;
			    //RTU_DEBUG("fvalue=%f\r\n",fvalue);
			     break;
			 case ATP:
				  //RTU_DEBUG("ATP\r\n");
			    //fvalue = response->DataField[3]*100.0/255.0;
			    //RTU_DEBUG("fvalue=%f\r\n",fvalue);
			     break;
			 case OBD_REQUIRE:
				   break;
			 case TSES:
				 	 //RTU_DEBUG("TSES\r\n");
			     //fvalue = (response->DataField[3]*256.0)+response->DataField[4];
			     //RTU_DEBUG("fvalue=%d\r\n",fvalue);
				   break;
			 case PID_2:
				   break;
			 case DISTANCE_MIL:
				 	 //RTU_DEBUG("DISTANCE_MIL\r\n");
			     //fvalue = (response->DataField[3]*256.0)+response->DataField[4];
			     //RTU_DEBUG("fvalue=%d\r\n",fvalue);
			     break;
			 case EGR:
				 	 //RTU_DEBUG("EGR\r\n");
			     //fvalue = response->DataField[3]*100.0/255;
			     //RTU_DEBUG("fvalue=%d\r\n",fvalue);
			     break;
			 case EP:
				   //RTU_DEBUG("EP\r\n");
			     //fvalue = response->DataField[3]*100.0/255;
			     //RTU_DEBUG("fvalue=%d\r\n",fvalue);
			     break;
			 case WARM_UPS:
				 	 //value = response->DataField[3];
			     //RTU_DEBUG("value=%d\r\n",value);
				   break;
			 case DISTANCE_DTC_CLEAR:
				 	 //value = response->DataField[3];
			     //RTU_DEBUG("value=%d\r\n",value);
				   break;
			 case BP:
				 	 //value = response->DataField[3]*256+response->DataField[4];
			     //RTU_DEBUG("value=%d\r\n",value);
				   break;
			 case BANK1_SENSOR1_O2S_1:
				   //value = (response->DataField[3]*256+response->DataField[4])/32768;
			     //RTU_DEBUG("value=%d\r\n",value);
			 		 //fvalue = (response->DataField[5]*256+response->DataField[6])/256-128;
			     //RTU_DEBUG("fvalue=%d\r\n",fvalue);
				   break;
			 case PID_3:
				   break;
			 case MONITOR_STATUS_CYCLE:
				   break;
			 case MODULE_VOLTAGE:
				 	 //RTU_DEBUG("MODULE_VOLTAGE\r\n");
			     //fvalue = (response->DataField[3]*256.0+response->DataField[4])/1000.0;
			     //RTU_DEBUG("fvalue=%d\r\n",fvalue);
			     break;
			 case ABSOLUTE_LOAD:
				 	 //value = (response->DataField[3]*256+response->DataField[4])*100/255;
			     //RTU_DEBUG("value=%d\r\n",value);
				   break;
			 case ATPB:
				   //RTU_DEBUG("ATPB\r\n");
			     //fvalue = response->DataField[3]*100.0/255.0;
			     //RTU_DEBUG("fvalue=%d\r\n",fvalue);
			     break;
			 case ATPD:
				   //RTU_DEBUG("ATPD\r\n");
			     //fvalue = response->DataField[3]*100.0/255.0;
			     //RTU_DEBUG("fvalue=%d\r\n",fvalue);
			     break;
			 case ATPE:
				   //RTU_DEBUG("ATPE\r\n");
			     //fvalue = response->DataField[3]*100.0/255.0;
			     //RTU_DEBUG("fvalue=%d\r\n",fvalue);
			     break;
			 case TAC:
				   //RTU_DEBUG("TAC\r\n");
			     //fvalue = response->DataField[3]*100.0/255.0;
			     //RTU_DEBUG("fvalue=%d\r\n",fvalue);
			     break;
     }
		 pci = response->DataField[0];
		 CurrentLen = pci+4+1+1;
		 c16toa(&CurrentLen,&Pidack[1]);
		 //RTU_DEBUG("response->Id=%d\r\n",response->Id);
		 Pidack[3] = channel;
		 c32toa_1(&response->Id,&Pidack[4]);
		 memcpy(&Pidack[8],response->DataField,pci+1);
		 senddatapacket(&COM2,Pidack,FIXED_LENGTH+CurrentLen+CHECKSUM);
}

void ProcessSingleFrame(DiagnosticResponse* response)
{
	   u8 Mode;
	   u16 CurrentLen = 0;
	   float diagnosticdata;
	   u8 dtcack[21] = {0x25};
		 Mode = response->DataField[1];
	   //RTU_DEBUG("ProcessSingleFrame\r\n");
		 switch(Mode)
		 {
			 case REQUEST_POWERTRAIN_DIAGNOSTIC_DATA+0x40:
				 		 response->Pid = response->DataField[2];
						 diagnosticdata = diagnostic_decode_pid(response);
						 //RTU_DEBUG("diagnosticdata=%f\r\n",diagnosticdata);
				     break;
			 case REQUEST_POWERTRAIN_FREEZE_FRAME_DATA+0x40:
				     break;
			 //解析排放相关的故障应答帧
			 case REQUEST_EMISSIONS_DTC+0x40:
			 case REQUEST_EMISSIONS_DTC_CLEAR+0x40:
				 	  CurrentLen = 8+4+1+1;
						c16toa(&CurrentLen,&dtcack[1]);
						dtcack[3] = channel;
						c32toa_1(&response->Id,&dtcack[4]);
						memcpy(&dtcack[8],response->DataField,8);
						senddatapacket(&COM2,dtcack,FIXED_LENGTH+CurrentLen+CHECKSUM);
				     break;
			 case REQUEST_OXYGEN_SENSOR_TEST_RESULTS+0x40:
				     break;
			 case REQUEST_ON_BOARD_MONITORING_TEST_RESULTS+0x40:
				     break;
			 case REQUEST_DRIVEING_CYCLE_DTC_REQUEST+0x40:
				     break;
			 case REQUEST_CONTROL_OF_ON_BOARD_SYSTEM+0x40:
				     break;
			 case REQUEST_VEHICLE_INFORMATION+0x40:
					  CurrentLen = 8+4+1+1;
						c16toa(&CurrentLen,&dtcack[1]);
				    dtcack[3] = channel;
						c32toa_1(&response->Id,&dtcack[4]);
						memcpy(&dtcack[8],response->DataField,8);
						senddatapacket(&COM2,dtcack,FIXED_LENGTH+CurrentLen+CHECKSUM);
				     break;
     }
}

void ProcessMultipleFrame(u8 Frametype,DiagnosticResponse* response)
{
	   
	   u8  Mode,i;
		 u16 FF_DL;
     static u16 RemainLength;	
	   u8 Vin[17];
	   u8 VehicleInfoType;
     u16 CurrentLen = 0;
	   u8 MFack[21] = {0x25};
    
	   if(Frametype == DIAGNOSTIC_FIRST_FRAME)
		 {
			  FF_DL = (response->DataField[0]&0x0F<<12)|response->DataField[1];
				Mode = response->DataField[2];
				switch(Mode)
				{
						case REQUEST_VEHICLE_INFORMATION+0x40:
							  VehicleInfoType = response->DataField[3];
							  switch(VehicleInfoType)
								{
									case DIAGNOSTIC_VIN:
										  //memcpy(Vin,&response->DataField[4],4);
									case DIAGNOSTIC_CALIBRATIN_ID:
									case DIAGNOSTIC_CALIBRATIN_VERIFICATION_NUMBER:
								      //RemainLength = FF_DL-4;
									    CurrentLen = 8+4+1;
									    c16toa(&CurrentLen,&MFack[1]);
									    MFack[3] = channel;
		                  c32toa_1(&response->Id,&MFack[4]);
											memcpy(&MFack[8],response->DataField,8);
											senddatapacket(&COM2,MFack,FIXED_LENGTH+CurrentLen+CHECKSUM);
								}
								break;
				}
		 }
		 else if(Frametype == DIAGNOSTIC_CONTINUOUS_FRAME)
		 {
			    //RTU_DEBUG("DIAGNOSTIC_CONTINUOUS_FRAME\r\n");
			
				  CurrentLen = 8+4+1;
					c16toa(&CurrentLen,&MFack[1]);
			    MFack[3] = channel;
		      c32toa_1(&response->Id,&MFack[4]);
					memcpy(&MFack[8],response->DataField,8);
					//RTU_DEBUG("response->DataField[0]=%x\r\n",response->DataField[0]);
			    
				  senddatapacket(&COM2,MFack,FIXED_LENGTH+CurrentLen+CHECKSUM);
			    DelayMS(100);
			    //for(i=0;i<17;i++)
					//	RTU_DEBUG("%x ",MFack[i]);
		 }

}

void  DiagnosticResponsePro(DiagnosticResponse* response)
{
	   u8 pci;
	   u8 Frametype;
	  
	   //RTU_DEBUG("DiagnosticResponsePro\r\n");
     if(response == NULL)
			  return;
	     
		 pci = response->DataField[0];
		 //RTU_DEBUG("pci=%x\r\n",pci);
		 Frametype = pci & 0xF0;
		 //RTU_DEBUG("Frametype=%x\r\n",Frametype);
		 switch(Frametype)
		 {
			 //处理单帧
			 case DIAGNOSTIC_SINGLE_FRAME:
				   ProcessSingleFrame(response);
				   break;
			 //处理多帧
			 case DIAGNOSTIC_FIRST_FRAME:
			 case DIAGNOSTIC_CONTINUOUS_FRAME:
				   ProcessMultipleFrame(Frametype,response);
				   break;
		 }
		

}


void OBD_ISO15765_4_Connection(u16 baud,DiagnosticRequestHandle* Handle)
{
    if(baud == 0 || Handle == NULL)
	   	 return;

	 
	  //diagnostic_request(Handle);
	  //msgdata[2] = PID_2;
	  //diagnostic_request(Handle);
	  //msgdata[2] = PID_3;
	  //diagnostic_request(Handle);
	  //msgdata[2] = OBD_REQUIRE;
	  //diagnostic_request(Handle);
	  //msgdata[2] = MONITOR_STATUS;
	  //diagnostic_request(Handle);
}


