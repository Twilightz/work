#include "def.h"
#include "usrConfig.h"
#include "spi_lib.h" 
#include "clibutil.h"

#include "UDev_CstApi.h"  
//#include "Alias.h"
#include "ef_usb_event.h"
#include "ef_usb_dev_io.h"
#define UDEV_DEVICE_DESC    0x01
#define UDEV_CONFIG_DESC    0x02


 

UINT8 user_abDeviceDescHS[] =
{
    0x12,             // Descriptor length(0x12)
	UDEV_DEVICE_DESC, // Device descriptor
	0x00,	          // USB specification 2.0
	0x02, 
	0xFF,			  // Class
	0xFF,			  // Subclass
	0xFF,			  // Protocol
	0x40,  		      // Maximum packet size of endpoint 0
    0xE3,
    0x05,             // VID 0x05E3
    0x10,
    0xF3,             // PID 0xF310  
    0x01,
	0x00,             // BCD 0x0001  
    0x01,	          // Vendor string
	0x02,	          // Product string
	0,				  // ciSERIAL,		    
	0x01, 		      // Number of configuration
};
 


UINT8 user_abConfigDescHS[] = 
{
	0x9,
	0x2,
	0x27,
	0x00,
	0x01,
	0x01,                       //Configuration value
	0x00,                       //index of string descriptor
	0xC0,//0x80,                       // Attribute = self powered , remote wakeup->0
	0x32,


//first interface
//UINT8 const abIntf0Desc[] =
	0x9,        // Descriptor length
	0x4,			         // Interface descriptor
	0x00,              // Interface number
	0x00,			             // Alternate setting
	3,	     				     // Number of endpoint
	0xFF,                        // Interface Class
	0xFF,                        // Interface SubClass
	0xFF,                        // Interface Protocol
	0x00,			             // Interface string

//UINT8 const abEndp1Desc[] =
    0x7,         // Descriptor length
	0x5,			         // Endpoint descriptor
	0x81,                        // IN endpoint number
	0x2,    //
	cMAXPKTHS1_L_CST,            // Maximum packet size Low byte
	cMAXPKTHS1_H_CST,            // Maximum packet size high byte
	0x00,                        // Polling interval

//UINT8 const abEndp2Desc[] =
//{
    0x7,         // Descriptor length
	0x5,			         // Endpoint descriptor
	0x02,                        // OUT endpoint number
	0x02,    //
	cMAXPKTHS2_L_CST,            // Maximum packet size Low byte
	cMAXPKTHS2_H_CST,            // Maximum packet size high byte
	0x00,                        // Polling interval


	//UINT8 const abEndp3Desc[] =
    0x7,         // Descriptor length
	0x5,			         // Endpoint descriptor
	0x83,                        // IN endpoint number
	0x3,     //
    cMAXPKTHS3_L_CST,            // Maximum packet size Low byte    
	0x00,                        // Maximum packet size high byte
	0x08				// 2^(8-1) /8 = 16ms
};

// 121004 G-mark add for USB FULL speed >>
UINT8 user_abConfigDescFS[] = 
{
	0x09,
	0x02,
	0x27,
	0x00,
	0x01,
	0x01,                       //Configuration value
	0x00,                       //index of string descriptor
	0xC0,//0x80,                       // Attribute = self powered , remote wakeup->0,0x80,                       // Attribute = self powered , remote wakeup->0
	0x32,


//first interface
//UINT8 const abIntf0Desc[] =
	0x9,        // Descriptor length
	0x4,			         // Interface descriptor
	0x00,              // Interface number
	0x00,			             // Alternate setting
	3,	     				     // Number of endpoint
	0xFF,                        // Interface Class
	0xFF,                        // Interface SubClass
	0xFF,                        // Interface Protocol
	0x00,			             // Interface string

//UINT8 const abEndp1Desc[] =
    0x7,         // Descriptor length
	0x5,			         // Endpoint descriptor
	0x81,                        // IN endpoint number
	0x2,    //
	cMAXPKTFS1_L_CST,            // Maximum packet size Low byte
	cMAXPKTFS1_H_CST,            // Maximum packet size high byte
	0x00,                        // Polling interval

//UINT8 const abEndp2Desc[] =
//{
    0x7,         // Descriptor length
	0x5,			         // Endpoint descriptor
	0x02,                        // OUT endpoint number
	0x2,    //
	cMAXPKTFS2_L_CST,            // Maximum packet size Low byte
	cMAXPKTFS2_H_CST,            // Maximum packet size high byte
	0x00,                        // Polling interval


	//UINT8 const abEndp3Desc[] =
    0x7,         // Descriptor length
	0x5,			         // Endpoint descriptor
	0x83,                        // IN endpoint number
	0x3,     //
    cMAXPKTFS3_L_CST,            // Maximum packet size Low byte    
	0x00,                        // Maximum packet size high byte
	0x10,						//16ms

};
// 121004 G-mark add for USB FULL speed << 
 

//-------istring customer define ----------
//-------istring customer define ----------
UINT8 abNewVdrStr1Desc[] =
{
	0x10, 0x3,
	'G',0,
	'E',0,
	'N',0,
	'E',0,
	'S',0,
	'Y',0,	
	'S',0	
};
 
UINT8 abNewPrtStr2Desc[] =
{
	0x10, 0x3,
	'S',0,
	'C',0,
	'A',0,
	'N',0,
	'N',0,	
	'E',0,
	'R',0,
	
}; 

//Customer command table 8 Byte
UINT8 CstSetupCmd_SetRestart[8] = {0x40, 0xFF, 0xFE, 0x00, 0x01, 0x00, 0x01, 0x00};
UINT8 CstSetupCmd_RdStatus[8]   = {0xC0, 0xFF, 0xFE, 0x00, 0x02, 0x00, 0x02, 0x00};
UINT8 CstSetupCmd_CancelFlow[8] = {0x40, 0xFF, 0xFE, 0x00, 0x03, 0x00, 0x01, 0x00};


BOOL usbdev_event_callback_register(void);
/************************************************************************
 *		BOOL user_UpPortConfig()
 ************************************************************************/
BOOL user_UpPortConfig()
{
    
   

   Chg_Dev_Desc(user_abDeviceDescHS); //If we want to change the string, we have to open it.
   Chg_Cfg_Desc(user_abConfigDescHS, HS_CST);
   Chg_Cfg_Desc(user_abConfigDescFS, FS_CST);    
 
   ChgVdrStr(abNewVdrStr1Desc, 0x0E, TRUE); // vendor string
   ChgPdtStr(abNewPrtStr2Desc, 0x10, TRUE); // product string
   
   //ChgSerStr(abNewPrtStr2Desc, 0x10, TRUE); // serial number string

 
   udev_RegEP0CmdHandler(0, CstSetupCmd_SetRestart, usbdev_ctrl_set_restart,  1);          // 1 CW, 0 CR
   udev_RegEP0CmdHandler(0, CstSetupCmd_CancelFlow, usbdev_ctrl_cancel_flow,  1);          // 1 CW, 0 CR
   udev_RegEP0CmdHandler(0, CstSetupCmd_RdStatus  , usbdev_ctrl_read_status,  0);          // 1 CW, 0 CR
 
 

 
   return TRUE;	
}


/************************************************************************
 *		BOOL user_downPortConfig(TX_BYTE_POOL *stack_pool)
 ************************************************************************/
BOOL user_downPortConfig()
{
	return TRUE;
}
 
/************************************************************************
 *		BOOL sysconfig_initialize(TX_BYTE_POOL *stack_pool)
 *
 * 
 ************************************************************************/
BOOL usrConfig_initialize()
{
 
   	
	if( !user_downPortConfig())
		return FALSE;

	if( !user_UpPortConfig())
		return FALSE;	
 
		
	return TRUE;
}

