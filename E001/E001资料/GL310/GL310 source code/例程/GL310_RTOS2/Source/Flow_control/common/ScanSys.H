#if !defined(_ScanSys_H_)
#define _ScanSys_H_
#include "dtype.h"
#include "os_api.h"

#define MAX_MSG_TASK_NUM        16	//8	// 140731

#define TASK_ID_USB_EVENT       0x1000
#define TASK_ID_USB_CMD         0x1001
#define TASK_ID_EXTDEV          0x1002
#define TASK_ID_UART_CMD        0x1003
#define TASK_ID_SYSCTRL         0x1004
#define TASK_ID_SCANCTRL        0x1005
#define TASK_ID_KEYSENSE        0x1006
#define TASK_ID_JOB             0x1007

#define TASK_STACK_SIZE         8192
#define RTOS_BYTE_POOL_SIZE     0x40000
#define TASK_QUEUE_ELEMENTS     16//16 * 4 //4

#define SYS_CTRL_QUEUE_ID       1
#define SCAN_CTRL_QUEUE_ID      6
#define USB_DEV_CTRL_QUEUE_ID   3
#define USB_HOST_CTRL_QUEUE_ID  4
#define AUX_CMDHOST_QUEUE_ID    5
#define JOB_QUEUE_ID            7

#define MSG_CMD_BASE_CODE               0x1000
#define MSG_CMD_LOCK_SCANNER            (MSG_CMD_BASE_CODE + 1)
#define MSG_CMD_GET_SCANNER_CONFIG      (MSG_CMD_BASE_CODE + 2)
#define MSG_CMD_GET_ADF_STATUS          (MSG_CMD_BASE_CODE + 3)
#define MSG_CMD_SET_SCAN_INFO           (MSG_CMD_BASE_CODE + 4)
#define MSG_CMD_ABORT_SCAN              (MSG_CMD_BASE_CODE + 5)
#define MSG_CMD_UNLOCK_SCANNER          (MSG_CMD_BASE_CODE + 6)
#define MSG_CMD_SET_CHIP_DATA           (MSG_CMD_BASE_CODE + 7)
#define MSG_CMD_DIAGNOSTIC              (MSG_CMD_BASE_CODE + 8)
#define MSG_CMD_PSCANNERINFO            (MSG_CMD_BASE_CODE + 9)
#define MSG_CMD_PSTATUS                 (MSG_CMD_BASE_CODE + 10)
#define MSG_CMD_PSCANDATA               (MSG_CMD_BASE_CODE + 11)
#define MSG_CMD_PAGE_STATUS             (MSG_CMD_BASE_CODE + 12)

#define MSG_USBCMD_GETSTATUS 

#define MSG_SCANCMD_BASE_CODE           0x2000
#define MSG_SCAN_SET_PARAMETER          (MSG_SCANCMD_BASE_CODE + 1)


typedef __packed struct {
           UINT32       spScanBeginPixel;
           UINT32       spScanBeginLine;
           UINT32       spScanLinePixels;
           UINT32       spTotalScanLines;
           UINT16       spPixelResolution;
           UINT16       spMotorResolution;
           UINT8        spImageType;
           UINT8        spMonoChannel;
           UINT8        spPixelDepth;
           float        spExposureTime;
           float        spShutterTime[3];
           float        spShutterTime_B[3];
           UINT8        spTimingIdx;
           UINT8        spDummyLines;
           UINT8        spStillMode;
           UINT8        spAutoGoHome;
           UINT8        spDisBackTrack;
           UINT8        spShading;
           UINT8        spGammaMapping;
           UINT8        spCCDLMT;
           UINT8        spTestImage;
           UINT8        spADFscan;
           UINT8        spDualscan;
           UINT8        spJPEGENC;
           UINT8        spMirror;
           UINT8        spCCDPacking;
} SCAN_REQUEST_DATA;

typedef __packed struct {
           DWORD        dwScanSource;           // DWord:  0
           BYTE         nCalibrate;             // DWord:  1
           BYTE         nCarriageDirection;
           BYTE         nCarriageGoHome;
           BYTE         nScanFromHome;
           DWORD        dwReserved;             // DWord:  2
           WORD         wYear;                  // DWord:  3
           BYTE         nMonth;
           BYTE         nDay;
           BYTE         nHour;                  // DWord:  4
           BYTE         nMinute;
           WORD         wBias;
           DWORD        dwBedWidth;             // DWord:  5
           DWORD        dwBedHeight;            // DWord:  6
           DWORD        dwIntensityMin;         // DWord:  7 
           DWORD        dwIntensityMax;         // DWord:  8 
           DWORD        dwIntensityStep;        // DWord:  9 
           DWORD        dwContrastMin;          // DWord: 10  
           DWORD        dwContrastMax;          // DWord: 11
           DWORD        dwContrastStep;         // DWord: 12
           DWORD        dwCompressCapability;   // DWord: 13
           DWORD        dwColorCapability;      // DWord: 14
           DWORD        dwPixelWidth;           // DWord: 15
           DWORD        dwWidthInBytes;         // DWord: 16
           DWORD        dwPixelHeight;          // DWord: 17
           DWORD        dwDataTypes;            // DWord: 18
           DWORD        dwPixelDepth;           // DWord: 19
           DWORD        dwIntensity;            // DWord: 20
           DWORD        dwContrast;             // DWord: 21
           DWORD        dwX_Resolution;         // DWord: 22
           DWORD        dwY_Resolution;         // DWord: 23
           DWORD        dwX_Position;           // DWord: 24
           DWORD        dwY_Position;           // DWord: 25
           DWORD        dwX_Witdh;              // DWord: 26
           DWORD        dwY_Height;             // DWord: 27
           DWORD        dwDither;               // DWord: 28
           DWORD        dwNegative;             // DWord: 29
           DWORD        dwMirror;               // DWord: 30
           DWORD        dwAutoBack;             // DWord: 31
           DWORD        dwColorDither;          // DWord: 32
           DWORD        dwToneMap;              // DWord: 33
           DWORD        dwCompressEnable;       // DWord: 34
           DWORD        dwRawDataFormat;        // DWord: 35
           DWORD        dwRawPixelOrder;        // DWord: 36
           DWORD        dwDataAlignment;        // DWord: 37
           DWORD        dwIO_Interval;          // DWord: 38
           DWORD        dwMaxBufferSize;        // DWord: 39
} SCAN_REQUEST_INFO;


typedef __packed struct {
 
   UINT16       TaskID;
   UINT16       Message;
   UINT32       Parameter[3];
}SyncMessage;


typedef struct _msg_parameter{
    
    UINT8 *Buf;    
    UINT32 Len;
    OS_EventGroup Notify_Event;
    
}msg_parameter;
 


#define ID_SYS_CTRL             0x0001
#define ID_USB_EVENT            0x0002
#define ID_CMD_HANDLE           0x0003
#define ID_EXTDEV               0x0004
#define ID_UART_CMD             0x0005
#define ID_SCAN_CTRL            0x0006
#define ID_KEY_SENSE            0x0007
#define ID_SPIF_CTRL            0x0008



#define PRIO_ID_SYS_CTRL        10
#define PRIO_ID_USB_EVENT       11
#define PRIO_ID_CMD_HANDLE      11
#define PRIO_ID_EXTDEV          11
#define PRIO_ID_FAKESCAN        14
#define PRIO_ID_CMDUART         9
#define PRIO_ID_KEYSENSE        15
#define PRIO_ID_SPIF_CTRL       12
#define PRIO_ID_JOB             15	// Scan thread
#define PRIO_ID_SCAN_CTRL       14

//------------------------- SCTRL_MSG_BASE_CODE----------------------------------------
#define SCTRL_MSG_BASE_CODE             (ID_SYS_CTRL << 12)
#define SCTRL_MSG_FNSH                  (SCTRL_MSG_BASE_CODE + 1)



//------------------------- UEVT_MSG_BASE_CODE -----------------------------------------
#define UEVT_MSG_BASE_CODE              (ID_USB_EVENT << 12)
#define UEVT_MSG_BULK_OUT_DMA_START     (UEVT_MSG_BASE_CODE + 1)
#define UEVT_MSG_BULK_OUT_DMA_WAIT_FNSH (UEVT_MSG_BASE_CODE + 2)
#define UEVT_MSG_BULK_IN_DMA_START      (UEVT_MSG_BASE_CODE + 3)
#define UEVT_MSG_BULK_IN_DMA_WAIT_FNSH  (UEVT_MSG_BASE_CODE + 4)

#define UEVT_MSG_BULK_OUT_PIO_START     (UEVT_MSG_BASE_CODE + 5)
#define UEVT_MSG_BULK_OUT_PIO_WAIT_FNSH (UEVT_MSG_BASE_CODE + 6)
#define UEVT_MSG_BULK_IN_PIO_START      (UEVT_MSG_BASE_CODE + 7)
#define UEVT_MSG_BULK_IN_PIO_WAIT_FNSH  (UEVT_MSG_BASE_CODE + 8)
#define UEVT_MSG_INTERRUPT_PIO_START    (UEVT_MSG_BASE_CODE + 9)
#define UEVT_MSG_INTERRUPT_PIO_FNSH     (UEVT_MSG_BASE_CODE + 10)
#define UEVT_MSG_WAIT_RESET             (UEVT_MSG_BASE_CODE + 11)

#define UEVT_MSG_VDATA_RDY              (UEVT_MSG_BASE_CODE + 21)
#define UEVT_MSG_VCMD_RDY               (UEVT_MSG_BASE_CODE + 22)
#define UEVT_MSG_VINT_RDY               (UEVT_MSG_BASE_CODE + 23)

#define UEVT_MSG_VBUS_ON                (UEVT_MSG_BASE_CODE + 37)
#define UEVT_MSG_VBUS_OFF               (UEVT_MSG_BASE_CODE + 38)
#define UEVT_MSG_CONFIG_ON              (UEVT_MSG_BASE_CODE + 39)
#define UEVT_MSG_CONFIG_OFF             (UEVT_MSG_BASE_CODE + 40)



//----------------------- CHDL_MSG_BASE_CODE -------------------------------------------
#define CHDL_MSG_BASE_CODE              (ID_CMD_HANDLE << 12)
#define CHDL_MSG_LOCK_SCANNER           (CHDL_MSG_BASE_CODE + 1)
#define CHDL_MSG_GET_SCANNER_CONFIG     (CHDL_MSG_BASE_CODE + 2)
#define CHDL_MSG_GET_ADF_STATUS         (CHDL_MSG_BASE_CODE + 3)
#define CHDL_MSG_SET_SCAN_INFO          (CHDL_MSG_BASE_CODE + 4)
#define CHDL_MSG_ABORT_SCAN             (CHDL_MSG_BASE_CODE + 5)
#define CHDL_MSG_UNLOCK_SCANNER         (CHDL_MSG_BASE_CODE + 6)
#define CHDL_MSG_SET_CHIP_DATA          (CHDL_MSG_BASE_CODE + 7)
#define CHDL_MSG_DIAGNOSTIC             (CHDL_MSG_BASE_CODE + 8)
#define CHDL_MSG_PAGE_STATUS            (CHDL_MSG_BASE_CODE + 9)  

#define CHDL_MSG_JOB                    (CHDL_MSG_BASE_CODE + 1)
#define CHDL_MSG_INFO                   (CHDL_MSG_BASE_CODE + 2)  
#define CHDL_MSG_SET_PAR                (CHDL_MSG_BASE_CODE + 4)
#define CHDL_MSG_CMD					(CHDL_MSG_BASE_CODE + 8)

#define CHDL_MSG_START_SCAN             (CHDL_MSG_BASE_CODE + 10)  
#define CHDL_MSG_GET_SCAN_IMAGE         (CHDL_MSG_BASE_CODE + 11)  
#define CHDL_MSG_CANL_SCAN              (CHDL_MSG_BASE_CODE + 12)  
#define CHDL_MSG_STOP_SCAN              (CHDL_MSG_BASE_CODE + 13)  

#define CHDL_MSG_FWUP_PAR               (CHDL_MSG_BASE_CODE + 15)
#define CHDL_MSG_FWUP_IMG               (CHDL_MSG_BASE_CODE + 16)
#define CHDL_MSG_FWUP_REBOOT            (CHDL_MSG_BASE_CODE + 17)

#define CHDL_MSG_DOWNLOAD_SHADING       (CHDL_MSG_BASE_CODE + 18)
#define CHDL_MSG_SET_AFE                (CHDL_MSG_BASE_CODE + 19)
#define CHDL_MSG_MOTOR_MOVE             (CHDL_MSG_BASE_CODE + 20)

#define CHDL_MSG_SINGLE_BUS_ACCESS      (CHDL_MSG_BASE_CODE + 21)
#define CHDL_MSG_MULTI_BUS_ACCESS       (CHDL_MSG_BASE_CODE + 22)

#define CHDL_MSG_GET_VERSION            (CHDL_MSG_BASE_CODE + 30)    
   
//-------------------- SPIF_MSG_BASE_CODE ----------------------------------------------
#define SPIF_MSG_BASE_CODE              (ID_SPIF_CTRL << 12)
#define SPIF_MSG_FWUP                   (SPIF_MSG_BASE_CODE + 1)
#define SPIF_MSG_SET_BOOT_RECOVERY      (SPIF_MSG_BASE_CODE + 2)
#define SPIF_MSG_CHK_ID_TAG             (SPIF_MSG_BASE_CODE + 3)
#define SPIF_MSG_ALLOCATE_ID_TAG        (SPIF_MSG_BASE_CODE + 4)



//--------------------- FSCN_MSG_BASE_CODE ---------------------------------------------
#define FSCN_MSG_BASE_CODE              (ID_SCAN_CTRL << 12)
#define FSCN_MSG_SCAN_START             (FSCN_MSG_BASE_CODE + 1)
#define FSCN_MSG_SCAN_OFF               (FSCN_MSG_BASE_CODE + 2)
#define FSCN_MSG_SCAN_DATA_READY        (FSCN_MSG_BASE_CODE + 3)

//--------------------- UEXT_MSG_BASE_CODE ---------------------------------------------
#define UEXT_MSG_BASE_CODE              (ID_EXTDEV << 12)
#define UEXT_MSG_PORTA_PLUG             (UEXT_MSG_BASE_CODE + 1)
#define UEXT_MSG_PORTA_UNPLUG           (UEXT_MSG_BASE_CODE + 2)
#define UEXT_MSG_PORTB_PLUG             (UEXT_MSG_BASE_CODE + 3)
#define UEXT_MSG_PORTB_UNPLUG           (UEXT_MSG_BASE_CODE + 4)

//-------------------- CUART_MSG_BASE_CODE ----------------------------------------------
#define CUART_MSG_BASE_CODE             (ID_UART_CMD << 12)
#define CUART_MSG_WRITE                 (CUART_MSG_BASE_CODE + 1)
#define CUART_MSG_READ                  (CUART_MSG_BASE_CODE + 2)

//-------------------- KSNS_MSG_BASE_CODE ----------------------------------------------
#define KSNS_MSG_BASE_CODE              (ID_KEY_SENSE << 12)
#define KSNS_MSG_FNSH                   (KSNS_MSG_BASE_CODE + 1)
#define KSNS_MSG_KEYPRESS               (KSNS_MSG_BASE_CODE + 2)   

//-------------------- SCN_MSG_BASE_CODE -------------------------------------------------
#define SCAN_MSG_BASE_CODE              (ID_SCAN_CTRL << 12) 
#define SCAN_MSG_START_SCAN_ADF         (SCAN_MSG_BASE_CODE + 1)
#define SCAN_MSG_START_SCAN_FLATBED     (SCAN_MSG_BASE_CODE + 4)
#define SCAN_MSG_START_SCAN_TEST        (SCAN_MSG_BASE_CODE + 5)

#define SCAN_MSG_SCAN_FINISH            (SCAN_MSG_BASE_CODE + 2)
#define SCAN_MSG_SCAN_DATA_READY        (SCAN_MSG_BASE_CODE + 3)
#define SCAN_MSG_INFO_PAGE_START        (SCAN_MSG_BASE_CODE + 16)
#define SCAN_MSG_INFO_PAGE_END          (SCAN_MSG_BASE_CODE + 17)
#define SCAN_MSG_INFO_DATA_READY        (SCAN_MSG_BASE_CODE + 18)
#define SCAN_MSG_INFO_DATA_FINISH       (SCAN_MSG_BASE_CODE + 19)
#define SCAN_MSG_INFO_PAGE_FINISH       (SCAN_MSG_BASE_CODE + 20) 
#define SCAN_MSG_INFO_OPEN_FILE         (SCAN_MSG_BASE_CODE + 21)  
#define SCAN_MSG_INFO_CLOSE_FILE        (SCAN_MSG_BASE_CODE + 22)

#define SCAN_MSG_JOB					(SCAN_MSG_BASE_CODE + 32)
#define SCAN_MSG_PAR					(SCAN_MSG_BASE_CODE + 33)
#define SCAN_MSG_STAR					(SCAN_MSG_BASE_CODE + 34)
#define SCAN_MSG_INFO					(SCAN_MSG_BASE_CODE + 35)
#define SCAN_MSG_IMG					(SCAN_MSG_BASE_CODE + 36)
#define SCAN_MSG_ABRT					(SCAN_MSG_BASE_CODE + 37)
#define SCAN_MSG_STOP					(SCAN_MSG_BASE_CODE + 38)
#define SCAN_MSG_FIN					(SCAN_MSG_BASE_CODE + 39)
#define SCAN_MSG_CMD					(SCAN_MSG_BASE_CODE + 40)
#define SCAN_MSG_SHADING				(SCAN_MSG_BASE_CODE + 41)
#define SCAN_MSG_AFE					(SCAN_MSG_BASE_CODE + 42)
#define SCAN_MSG_MOVE					(SCAN_MSG_BASE_CODE + 43)

//----------------------- CHDL_MSG_BASE_CODE -------------------------------------------
#define MSG_CHDL_BASE_CODE              (ID_CMD_HANDLE << 12)
#define MSG_CHDL_USB_GROUP              (0x00 << 6)
#define MSG_CHDL_EXT_GROUP              (0x01 << 6)  
#define MSG_CHDL_LOCK_SCANNER           (CHDL_MSG_BASE_CODE + 1)
#define MSG_CHDL_GET_SCANNER_CONFIG     (CHDL_MSG_BASE_CODE + 2)
#define MSG_CHDL_GET_ADF_STATUS         (CHDL_MSG_BASE_CODE + 3)
#define MSG_CHDL_SET_SCAN_INFO          (CHDL_MSG_BASE_CODE + 4)
#define MSG_CHDL_ABORT_SCAN             (CHDL_MSG_BASE_CODE + 5)
#define MSG_CHDL_UNLOCK_SCANNER         (CHDL_MSG_BASE_CODE + 6)
#define MSG_CHDL_SET_CHIP_DATA          (CHDL_MSG_BASE_CODE + 7)
#define MSG_CHDL_DIAGNOSTIC             (CHDL_MSG_BASE_CODE + 8)
#define MSG_CHDL_PAGE_STATUS            (CHDL_MSG_BASE_CODE + 9)  

#define MSG_CHDL_U_LOCK_SCANNER         (CHDL_MSG_BASE_CODE + MSG_CHDL_USB_GROUP + 1)
#define MSG_CHDL_U_GET_SCANNER_CONFIG   (CHDL_MSG_BASE_CODE + MSG_CHDL_USB_GROUP + 2)
#define MSG_CHDL_U_GET_ADF_STATUS       (CHDL_MSG_BASE_CODE + MSG_CHDL_USB_GROUP + 3)
#define MSG_CHDL_U_SET_SCAN_INFO        (CHDL_MSG_BASE_CODE + MSG_CHDL_USB_GROUP + 4)
#define MSG_CHDL_U_ABORT_SCAN           (CHDL_MSG_BASE_CODE + MSG_CHDL_USB_GROUP + 5)
#define MSG_CHDL_U_UNLOCK_SCANNER       (CHDL_MSG_BASE_CODE + MSG_CHDL_USB_GROUP + 6)
#define MSG_CHDL_U_SET_CHIP_DATA        (CHDL_MSG_BASE_CODE + MSG_CHDL_USB_GROUP + 7)
#define MSG_CHDL_U_DIAGNOSTIC           (CHDL_MSG_BASE_CODE + MSG_CHDL_USB_GROUP + 8)
#define MSG_CHDL_U_PAGE_STATUS          (CHDL_MSG_BASE_CODE + MSG_CHDL_USB_GROUP + 9)  

#define MSG_CHDL_X_LOCK_SCANNER         (CHDL_MSG_BASE_CODE + MSG_CHDL_EXT_GROUP + 1)
#define MSG_CHDL_X_GET_SCANNER_CONFIG   (CHDL_MSG_BASE_CODE + MSG_CHDL_EXT_GROUP + 2)
#define MSG_CHDL_X_GET_ADF_STATUS       (CHDL_MSG_BASE_CODE + MSG_CHDL_EXT_GROUP + 3)
#define MSG_CHDL_X_SET_SCAN_INFO        (CHDL_MSG_BASE_CODE + MSG_CHDL_EXT_GROUP + 4)
#define MSG_CHDL_X_ABORT_SCAN           (CHDL_MSG_BASE_CODE + MSG_CHDL_EXT_GROUP + 5)
#define MSG_CHDL_X_UNLOCK_SCANNER       (CHDL_MSG_BASE_CODE + MSG_CHDL_EXT_GROUP + 6)
#define MSG_CHDL_X_SET_CHIP_DATA        (CHDL_MSG_BASE_CODE + MSG_CHDL_EXT_GROUP + 7)
#define MSG_CHDL_X_DIAGNOSTIC           (CHDL_MSG_BASE_CODE + MSG_CHDL_EXT_GROUP + 8)
#define MSG_CHDL_X_PAGE_STATUS          (CHDL_MSG_BASE_CODE + MSG_CHDL_EXT_GROUP + 9)  

//-------------------- SCN_MSG_BASE_CODE -------------------------------------------------   
#define MSG_SCAN_BASE_CODE              (ID_SCAN_CTRL << 12)
#define MSG_SCAN_USBCMD_GROUP           (0x00<<6)
#define MSG_SCAN_EXTCMD_GROUP           (0x01<<6)
#define MSG_SCAN_START_SCAN_ADF         (MSG_SCAN_BASE_CODE + 1)  
#define MSG_SCAN_START_SCAN_FLATBED     (MSG_SCAN_BASE_CODE + 4)  
#define MSG_SCAN_SCAN_FINISH            (MSG_SCAN_BASE_CODE + 2)  
#define MSG_SCAN_SCAN_DATA_READY        (MSG_SCAN_BASE_CODE + 3)

#define MSG_SCAN_U_START_SCAN_ADF       (MSG_SCAN_BASE_CODE + MSG_SCAN_USBCMD_GROUP + 1)
#define MSG_SCAN_U_START_SCAN_FLATBED   (MSG_SCAN_BASE_CODE + MSG_SCAN_USBCMD_GROUP + 4)  
#define MSG_SCAN_U_SCAN_FINISH          (MSG_SCAN_BASE_CODE + MSG_SCAN_USBCMD_GROUP + 2)  
#define MSG_SCAN_U_SCAN_DATA_READY      (MSG_SCAN_BASE_CODE + MSG_SCAN_USBCMD_GROUP + 3)

#define MSG_SCAN_X_START_SCAN_ADF       (MSG_SCAN_BASE_CODE + MSG_SCAN_EXTCMD_GROUP + 1)
#define MSG_SCAN_X_START_SCAN_FLATBED   (MSG_SCAN_BASE_CODE + MSG_SCAN_EXTCMD_GROUP + 4)  
#define MSG_SCAN_X_SCAN_FINISH          (MSG_SCAN_BASE_CODE + MSG_SCAN_EXTCMD_GROUP + 2)  
#define MSG_SCAN_X_SCAN_DATA_READY      (MSG_SCAN_BASE_CODE + MSG_SCAN_EXTCMD_GROUP + 3)
#define MSG_SCAN_X_SCAN_START_FAIL      (MSG_SCAN_BASE_CODE + MSG_SCAN_EXTCMD_GROUP + 5)

#define MSG_SCAN_INFO_PAGE_START        (MSG_SCAN_BASE_CODE + 16) 
#define MSG_SCAN_INFO_PAGE_END          (MSG_SCAN_BASE_CODE + 17) 
#define MSG_SCAN_INFO_DATA_READY        (MSG_SCAN_BASE_CODE + 18) 
#define MSG_SCAN_INFO_DATA_FINISH       (MSG_SCAN_BASE_CODE + 19) 
#define MSG_SCAN_INFO_PAGE_FINISH       (MSG_SCAN_BASE_CODE + 20) 
#define MSG_SCAN_INFO_OPEN_FILE         (MSG_SCAN_BASE_CODE + 21) 
#define MSG_SCAN_INFO_CLOSE_FILE        (MSG_SCAN_BASE_CODE + 22) 

#define MSG_SCAN_X_INFO_PAGE_START      (MSG_SCAN_BASE_CODE + MSG_SCAN_EXTCMD_GROUP + 16) 
#define MSG_SCAN_X_INFO_PAGE_END        (MSG_SCAN_BASE_CODE + MSG_SCAN_EXTCMD_GROUP + 17) 
#define MSG_SCAN_X_INFO_DATA_READY      (MSG_SCAN_BASE_CODE + MSG_SCAN_EXTCMD_GROUP + 18) 
#define MSG_SCAN_X_INFO_DATA_FINISH     (MSG_SCAN_BASE_CODE + MSG_SCAN_EXTCMD_GROUP + 19) 
#define MSG_SCAN_X_INFO_PAGE_FINISH     (MSG_SCAN_BASE_CODE + MSG_SCAN_EXTCMD_GROUP + 20) 
#define MSG_SCAN_X_INFO_OPEN_FILE       (MSG_SCAN_BASE_CODE + MSG_SCAN_EXTCMD_GROUP + 21) 
#define MSG_SCAN_X_INFO_CLOSE_FILE      (MSG_SCAN_BASE_CODE + MSG_SCAN_EXTCMD_GROUP + 22) 




#define CHDL_STAGE_IDLE                  0x0
#define CHDL_STAGE_CMD                   0x01
#define CHDL_STAGE_DATA                  0x02
 


#define CHDL_REP_INIT                    0x00
#define CHDL_REP_WAIT_USB_CONN           0x01
#define CHDL_REP_WAIT_CMD                0x02
#define CHDL_REP_BUSY_DMA_READ           0x03
#define CHDL_REP_BUSY_DMA_WRITE          0x04
#define CHDL_REP_WAIT_RESET              0x05
#define CHDL_REP_SYS_HANGON              0x88




#define CHDL_ACT_FREE                    0x00
#define CHDL_ACT_CANCEL_FLOW             0x01



typedef struct event_flow_structure{
        
    UINT8 stage;
    UINT8 status;    
    UINT8 acton;
    UINT8 usb_plug_flag; 
    UINT8 usb_reset_flag;
    UINT8 flow_restart_flag;
    UINT16 RESERVED;
    
    
}event_flow_structure;

extern event_flow_structure event_flow; 
 










void error_handle(INT8 *str, UINT8 msg);

#endif

