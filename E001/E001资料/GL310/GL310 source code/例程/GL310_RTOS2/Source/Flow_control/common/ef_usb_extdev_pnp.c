/***************************************************************************
* Name: ef_usb_pnp.c                                                       *
* Description: event flow usb external dev host downport plug and play     *
*																		   *
*																		   *
* Author: John KE      				                                       *
* Date: 2013/10/25	                                                       *
****************************************************************************/
#include "tx_api.h"
#include "hostlib.h"
#include "hostusb.h"
#include "usblib.h"

#include "scansys.h"
#include "ef_usb_extdev.h"



void usbext_PortA_Connection(hDev_node *hDev)
{
    printf("Port A Connection. Device VID=[%x] PID=[%x]\n",hDev->VID, hDev->PID);	    
    usbext_set_msg(ID_EXTDEV, UEXT_MSG_PORTA_PLUG, 0, 0);
}

void usbext_PortA_DisConnection(hDev_node *hDev)
{    
	printf("Port A disconnection\n");
	usbext_set_msg(ID_EXTDEV, UEXT_MSG_PORTA_UNPLUG, 0, 0);
}

void usbext_PortB_Connection(hDev_node *hDev)
{       
	printf("Port B Connection. Device VID=[%x] PID=[%x]\n",hDev->VID, hDev->PID);	
	usbext_set_msg(ID_EXTDEV, UEXT_MSG_PORTB_PLUG, 0, 0);
}

void usbext_PortB_DisConnection(hDev_node *hDev)
{     
	printf("Port B disconnection\n");	
	usbext_set_msg(ID_EXTDEV, UEXT_MSG_PORTB_UNPLUG, 0, 0);
}


void usbext_regist_usb_host_notification()
{
	usbHost_HostEventReg(HostPA_Conn, usbext_PortA_Connection);
	usbHost_HostEventReg(HostPA_Disconn, usbext_PortA_DisConnection);
	
	usbHost_HostEventReg(HostPB_Conn, usbext_PortB_Connection);
	usbHost_HostEventReg(HostPB_Disconn, usbext_PortB_DisConnection);
}

