/*
**********************************************************************************************************
*  Copyright (C), 2009-2013, ����˼׳�����з�����
*
*  ��Ŀ���ƣ� E7759A
*  
*  �ļ�����:  Obd.h
*
*  �ļ������� OBD IIӦ����ϳ������Ͷ���
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
**********************************************************************************************************
*/
#ifndef __OBD_H
#define __OBD_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "stm32f10x.h"


/*
 * ΪService $01 ��$02 ��PID����,���庬����ο�ISO 15031-5��PID definiions
 */
#define  PID_1															0x00   //PIDs support[01-20]
#define  MONITOR_STATUS											0x01   //Monitor status since DTCs cleared -���������֮��ļ�����״̬
#define  DTC_FREEZE													0x02   //DTC that caused required freeze frame data storage -DTC��������Ķ���֡���ݴ洢
#define  FUEL_SYSTEM_STATUS      						0x03   //Fuel system 1 and 2 status -ȼ��ϵͳ1��2״̬
#define  LOAD_VALUE													0x04   //Calculated LOAD Value -�����غ�ֵ(��λ:%)
#define  ECT																0x05   //Engine Coolant Temperature -��������ȴҺ�¶�(��λ:��)
#define  STFT_BANK1			       							0x06   //Short Term Fuel Trim - Bank 1 and 3 -����ȼ������-BANK 1��3����λ:%)
#define  LTFT_BANK1			       							0x07   //Long Term Fuel Trim - Bank 1 and 3 -����ȼ������-BANK 1��3����λ:%)
#define  STFT_BANK2		              				0x08   //Short Term Fuel Trim - Bank 1 and 3 -����ȼ������-BANK 2��4����λ:%)
#define  LTFT_BANK2			       							0x09   //Long Term Fuel Trim - Bank 1 and 3 -����ȼ������-BANK 2��4����λ:%)
#define  FRP																0x0A   //Fuel Rail Pressure -ȼ�͵���ѹ��(��λ:kPa)
#define  IMAP																0x0B   //Intake Manifold Absolute Pressure -������ܾ���ѹ������λkPa)
#define  ENGINE_RPM													0x0C   //Engine RPM -������ת��(��λ:rpm)
#define  VEHICLE_SPEED											0x0D   //Vehicle Speed Sensor -���ٴ�����(��λ:km/h)
#define  LTA																0x0E   //Ignition Timing Advance for #1 Cylinder -1�����׵����ǰ��
#define  IAT																0x0F   //Intake Air Temperature -�����¶�(��λ:��)
#define  AFR																0x10   //Air Flow Rate from Mass Air Flow Sensor -���������������Ŀ�������(��λg/s)
#define  ATP																0x11   //Absolute Throttle Position -�����ž���λ��(��λ:%)
#define  CSAS																0x12   //Commanded Secondary Air Status  -ָ��Ķ��ο���״̬
#define  LOS																0x13   //Location of Oxygen Sensors -����������λ��
/*
 * Bank 1-������1
 * -�������������ѹ  ��λ: V
 * -��ʱȼ������     ��λ:%
 */
#define  BANK1_SENSOR1                			0x14   //Bank 1 -Sensor 1
#define  BANK1_SENSOR2                			0x15   //Bank 1 -Sensor 2
#define  BANK1_SENSOR3                			0x16   //Bank 1 -Sensor 3
#define  BANK1_SENSOR4                			0x17   //Bank 1 -Sensor 4
#define  BANK2_SENSOR1                			0x18   //Bank 2 -Sensor 1
#define  BANK2_SENSOR2                			0x19   //Bank 2 -Sensor 2
#define  BANK2_SENSOR3                			0x1A   //Bank 2 -Sensor 3
#define  BANK2_SENSOR4                			0x1B   //Bank 2 -Sensor 4
#define  OBD_REQUIRE                     		0x1C   //OBD requirements to which vehicle is designed -�Գ�����Ƶ�OBDҪ��
#define  LOS_1                     					0x1D   //Location of oxygen sensors
#define  AIS                     		       	0x1E   //Auxiliary Input Status -(��������״̬)�������״̬
#define  TSES                     					0x1F   //Time Since Engine Start -�����������ʱ��򷢶�����������ʱ��

#define  PID_2                              0x20   //PIDs support[21-40]
#define  DISTANCE_MIL                   		0x21   //Distance Travelled While MIL is Activated -MIL��������ʻ����(��λ:km)
#define  FRP_1                              0x22   //Fuel Rail Pressure relative to manifold vacuum -ȼ�͹�����������յ�ѹ��(��λ:kPa)
#define  FRP_2                              0x23   //Fuel Rail Pressure -ȼ�͹�ѹ��(��λ:kPa)
/*
 * Bank 1 -������ 1 (������O2S)
 * -������(���ʽ)
 * -����������ѹ
 */
#define  BANK1_SENSOR1_O2S        					0x24   //Bank 1 -Sensor 1(wide range O2S)
#define  BANK1_SENSOR2_O2S        					0x25   //Bank 1 -Sensor 2(wide range O2S)
#define  BANK1_SENSOR3_O2S        					0x26   //Bank 1 -Sensor 3(wide range O2S)
#define  BANK1_SENSOR4_O2S        					0x27   //Bank 1 -Sensor 4(wide range O2S)
#define  BANK2_SENSOR1_O2S        					0x28   //Bank 2 -Sensor 1(wide range O2S)
#define  BANK2_SENSOR2_O2S        					0x29   //Bank 2 -Sensor 2(wide range O2S)
#define  BANK2_SENSOR3_O2S        					0x2A   //Bank 2 -Sensor 3(wide range O2S)
#define  BANK2_SENSOR4_O2S        					0x2B   //Bank 2 -Sensor 4(wide range O2S)
#define  EGR                                0x2C   //Commanded EGR  -ָ��
#define  EGR_ERROR                         	0x2D   //EGR Error   -EGR����(��λ:%)
#define  EP                                 0x2E   //Commanded Evaporative Purge  -ָ���ȼ�����ų�(��λ:%)
#define  FLI                                0x2F   //Fuel Level Input  -ȼ��������(��λ:%)
#define  WARM_UPS                         	0x30   //Number of warm-ups since diagnostic trouble codes cleared   -���ϴ�����պ��ȳ�����
#define  DISTANCE_DTC_CLEAR       					0x31   //Distance since diagnostic trouble codes cleared  -���ϴ�����պ���ʻ����  (��λ:km)
#define  ESVP                               0x32   //Evap System Vapour Pressure  -EVAP����ѹ��  (��λ:Pa)
#define  BP                                 0x33   //Barometric Pressure  -��ѹ   (��λ:kPa)
/*
 * Bank 1-������1(������O2S)
 * -������(���ʽ)
 * -�������� ��λ:mA
 */
#define  BANK1_SENSOR1_O2S_1        				0x34   //Bank 1 -Sensor 1(wide range O2S)
#define  BANK1_SENSOR2_O2S_1        				0x35   //Bank 1 -Sensor 2(wide range O2S)
#define  BANK1_SENSOR3_O2S_1        				0x36   //Bank 1 -Sensor 3(wide range O2S)
#define  BANK1_SENSOR4_O2S_1        				0x37   //Bank 1 -Sensor 4(wide range O2S)
#define  BANK2_SENSOR1_O2S_1        				0x38   //Bank 2 -Sensor 1(wide range O2S)
#define  BANK2_SENSOR2_O2S_1        				0x39   //Bank 2 -Sensor 2(wide range O2S)
#define  BANK2_SENSOR3_O2S_1        				0x3A   //Bank 2 -Sensor 3(wide range O2S)
#define  BANK2_SENSOR4_O2S_1        				0x3B   //Bank 2 -Sensor 4(wide range O2S)
/*
 * �߻����¶�Bank1,������1   ��λ:��
 */
#define  CT_BANK1_SENSOR_1            			0x3C    //Catalyst Temperature Bank 1,Sensor 1
#define  CT_BANK2_SENSOR_1            			0x3D    //Catalyst Temperature Bank 2,Sensor 1
#define  CT_BANK1_SENSOR_2            			0x3E    //Catalyst Temperature Bank 1,Sensor 2
#define  CT_BANK2_SENSOR_2            			0x3F    //Catalyst Temperature Bank 2,Sensor 2

#define  PID_3                              0x40    //PIDs support[41-60]
#define  MONITOR_STATUS_CYCLE      					0x41    //Monitor status this driving cycle   -���˴μ�ʻѭ��״̬
#define  MODULE_VOLTAGE                  		0x42    //Control module voltage  -����ģ���ѹ(��λ:V��
#define  ABSOLUTE_LOAD                      0x43    //Absolute Load Value -�����غ�ֵ(��λ:%��
#define  EQUIVALENCE_RATIO                  0x45    //Relative Throttle Position - ���������λ��(��λ:%)
#define  AMBIENT_AIR_TEMPERATURE   					0x46    //Ambient air temperature -��������(��λ:��)
#define  ATPB                  			     		0x47    //Absolute Throttle Position B   -����̤��λ��B   (��λ:%)
#define  ATPC                  			     		0x48    //Absolute Throttle Position C   -����̤��λ��C   (��λ:%)
#define  ATPD                  			     		0x49    //Accelerator Pedal Position D   -����̤��λ��D  (��λ:%)
#define  ATPE                  			     		0x4A   	//Accelerator Pedal Position E   -����̤��λ��E   (��λ:%)
#define  TAC                  			     		0x4C   	//Commanded Throttle Actuator Control  -ָ��Ľ����Ŵ���װ�ÿ���(��λ:%) 
#define  TIME_RUN_ENGINE                   	0x4D   	//Time run by the engine while MIL is activated  -MIL������󷢶������з�����(��λ:min)
#define  TIME_DTC_CLEAR                   	0x4E   	//Time since diagnostic trouble codes cleared  -���ϴ�����պ�ʱ�� (��λ:min)
#define  MAX_EQUIVALENCE_RATIO       				0x4F   	//Maximum value for Equivalence Ratio
#define  MAX_AFR                            0x50    //Maximum value for Air Flow Rate from Mass Air Flow Sensor  
#define  FUEL_TYPE                          0x51   	//Type of fuel currently being utilized by the vehicle  -��������ʹ�õ�ȼ������
#define  AFP                                0x52    //Alcohol Fuel Percentage  -�ƾ�ȼ�ϰٷֱ� (��λ:%)
#define  AESVP                              0x53    //Absolute Evap System Vapour Pressure  -EVAP����ѹ��(����ֵ)  (��λ:KPa)
#define  ESVP_1                             0x54    //Evap System Vapour Pressure  -EVAPϵͳ����ѹ��(��λ:Pa)
#define  STSO2_BANK                         0x55    //Short Term Secondary O2 Sensor Fuel Trim Bank 1 and 3  -��ʱ�ڶ���������ȼ������(��λ:%)
#define  LTSO2_BANK                         0x56    //Long Term Secondary O2 Sensor Fuel Trim Bank 1 and 3  -��ʱ�ڶ���������ȼ������(��λ:%)
#define  STSO2_BANK_2                       0x57    //Song Term Secondary O2 Sensor Fuel Trim Bank 2 and 4  (��λ:%)
#define  LTSO2_BANK_2                       0x58    //Long Term Secondary O2 Sensor Fuel Trim Bank 2 and 4  (��λ:%)
#define  FUEL_RAIL_PRESSURE              		0x59    //Fuel Rail Pressure(absolute) -ȼ�͵���ѹ��(����ֵ)  (��λ:KPa)
#define  RAPP                               0x5A   	//Relative Accelerator Pedal Position -��Լ���̤��λ�û�����̤�����λ��(��λ:%)

#define  FUNCTION_BROADCAST_ID			0x7DE
#define  PHYSICAL_ECU1_ID           0x7E0
#define  PHYSICAL_ECU1_RESPONSE_ID  0x7E8
#define  PHYSICAL_ECU2_ID           0x7E1
#define  PHYSICAL_ECU2_RESPONSE_ID  0x7E9

/*
*********************************************************************
*  �ࡢ�ṹ�����Ͷ���
*********************************************************************
*/
typedef enum{
    DIAGNOSTIC_CAN_ID_STANDARD, //��׼ID
    DIAGNOSTIC_CAN_ID_EXTENDED = 0x04 //��չID
}DiagnoticIdType;

typedef enum{
    DIAGNOSTIC_DATA_FRAME,   //����֡
    DIAGNOSTIC_ROMOTE_FRAME  //Զ��֡
}DiagnoticFrameType;



typedef enum{
    DIAGNOSTIC_VIN =0x02 ,  			     //����ʶ�����
    DIAGNOSTIC_CALIBRATIN_ID = 0x04,
    DIAGNOSTIC_CALIBRATIN_VERIFICATION_NUMBER = 0x06
}DiagnosticVehicleInfoType;

/*
 *  N_PUD���������ͣ�����֡����֡������֡������
 *
 */

/* ��֡(SF):N_PCItype = 0(7-4λ)    SF_DL(3-0λ)
 * -------------------------------------------------
 * ��֡(FF):N_PCItype = 1(7-4λ)    FF_DL(3-0λ)
 *-------------------------------------------------
 * ����֡(CF):N_PCItype = 2(7-4λ)   SN(3-0λ)
 * ------------------------------------------------
 * ����(FC):N_PCItype = 3(7-4λ)    FS(3-0λ)    
 */
typedef enum{
    DIAGNOSTIC_SINGLE_FRAME =0x00,  		//��֡SF			     
    DIAGNOSTIC_FIRST_FRAME = 0x10,   		//��֡FF
    DIAGNOSTIC_CONTINUOUS_FRAME = 0x20, //����֡CF
    DIAGNOSTIC_FLOW_CONTROL = 0x30  		//����FC
}DiagnostizFrameType;

/*
 *  OBD 9�ֱ�׼���ģʽ�����庬����ο� ISO 15031-5 �ĵ�
 */
typedef enum {
    REQUEST_POWERTRAIN_DIAGNOSTIC_DATA = 0x1,
    REQUEST_POWERTRAIN_FREEZE_FRAME_DATA = 0x2,
    REQUEST_EMISSIONS_DTC = 0x3,
    REQUEST_EMISSIONS_DTC_CLEAR = 0x4,
    REQUEST_OXYGEN_SENSOR_TEST_RESULTS = 0x5,
    REQUEST_ON_BOARD_MONITORING_TEST_RESULTS = 0x6,
    REQUEST_DRIVEING_CYCLE_DTC_REQUEST = 0x7,
    REQUEST_CONTROL_OF_ON_BOARD_SYSTEM = 0x8,
    REQUEST_VEHICLE_INFORMATION = 0x9
} DiagnosticMode;

/*
 * Negative Response Code(NRC): ��Ӧ����,���庬��
 * �����ISO 15031-5 �ĵ�
 */
typedef enum {
    NRC_SUCCESS = 0x0,
    NRC_SERVICE_NOT_SUPPORTED = 0x11,
    NRC_SUB_FUNCTION_NOT_SUPPORTED = 0x12,
    NRC_BUSY_REPEAT_REQUEST = 0x21,
    NRC_CONDITIONS_NOT_CORRECT = 0x22,
    NRC_RESPONSE_PENDING = 0x78
} DiagnosticNegativeResponseCode;

/*
 *   Id         							- ��ʾ�ٲ�ID
 *   BitField     						- ��ʾλ��
 *   DataField     						- ��ʾ������,����PCI+Mode+parameter.....��
 *   Length         					- ��Ч���ݳ���
 *   buf           					  - ��ʾ���ͻ���
 *   Ide           					  - ��ʾID����
 */

typedef struct {
    u32   Id;
	  u8    BitField;
    u8    DataField[8];
    u16   Length;
	  u8*   buf;
	  DiagnoticIdType Ide;
}DiagnosticRequest;

/*
 *   Id         							- ��ʾ�ٲ�ID
 *   DataField     						- ��ʾ������
 *   Length         					- ��Ч���ݳ���
 *   Pid                      - ����ID
 *   Negative_response_code 	- ��ʾ��Ӧ����
 */
typedef struct {
    u32 Id;
    u8  DataField[8];
    u16 length;
	  u8  Pid;
    DiagnosticNegativeResponseCode Negative_response_code;
}DiagnosticResponse;

typedef void (*DiagnosticResponseReceived) (DiagnosticResponse* response);

/*
 *   Request         	- ��ʾ�������
 *   appcallback     	- ��ʾӦ�ò������Ӧ����
 *   networkcallback  - ��ʾ����������Ӧ����
 */
typedef struct {
   DiagnosticRequest Request;
   DiagnosticResponseReceived appcallback;
   DiagnosticResponseReceived networkcallback;
}DiagnosticRequestHandle;  


extern void diagnostic_request(DiagnosticRequestHandle* Handle,DiagnosticResponseReceived callback);
void OBD_ISO15765_4_Connection(u16 baud,DiagnosticRequestHandle* Handle);
extern void DiagnosticResponsePro(DiagnosticResponse* response);


#ifdef __cplusplus
}
#endif

#endif /* __OBD_H */

