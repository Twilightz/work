/*
**********************************************************************************************************
*  Copyright (C), 2009-2013, 合众思壮深圳研发中心
*
*  项目名称： E7759A
*  
*  文件名称:  Obd.h
*
*  文件描述： OBD II应用诊断程序类型定义
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
**********************************************************************************************************
*/
#ifndef __OBD_H
#define __OBD_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "stm32f10x.h"


/*
 * 为Service $01 和$02 的PID定义,具体含义请参考ISO 15031-5的PID definiions
 */
#define  PID_1															0x00   //PIDs support[01-20]
#define  MONITOR_STATUS											0x01   //Monitor status since DTCs cleared -故障码清除之后的监视器状态
#define  DTC_FREEZE													0x02   //DTC that caused required freeze frame data storage -DTC导致所需的冻结帧数据存储
#define  FUEL_SYSTEM_STATUS      						0x03   //Fuel system 1 and 2 status -燃油系统1和2状态
#define  LOAD_VALUE													0x04   //Calculated LOAD Value -计算载荷值(单位:%)
#define  ECT																0x05   //Engine Coolant Temperature -发动机冷却液温度(单位:度)
#define  STFT_BANK1			       							0x06   //Short Term Fuel Trim - Bank 1 and 3 -短期燃油修正-BANK 1和3（单位:%)
#define  LTFT_BANK1			       							0x07   //Long Term Fuel Trim - Bank 1 and 3 -长期燃油修正-BANK 1和3（单位:%)
#define  STFT_BANK2		              				0x08   //Short Term Fuel Trim - Bank 1 and 3 -短期燃油修正-BANK 2和4（单位:%)
#define  LTFT_BANK2			       							0x09   //Long Term Fuel Trim - Bank 1 and 3 -长期燃油修正-BANK 2和4（单位:%)
#define  FRP																0x0A   //Fuel Rail Pressure -燃油导轨压力(单位:kPa)
#define  IMAP																0x0B   //Intake Manifold Absolute Pressure -进气歧管绝对压力（单位kPa)
#define  ENGINE_RPM													0x0C   //Engine RPM -发动机转速(单位:rpm)
#define  VEHICLE_SPEED											0x0D   //Vehicle Speed Sensor -车速传感器(单位:km/h)
#define  LTA																0x0E   //Ignition Timing Advance for #1 Cylinder -1号气缸点火提前角
#define  IAT																0x0F   //Intake Air Temperature -进气温度(单位:度)
#define  AFR																0x10   //Air Flow Rate from Mass Air Flow Sensor -大气流量传感器的空气流量(单位g/s)
#define  ATP																0x11   //Absolute Throttle Position -节气门绝对位置(单位:%)
#define  CSAS																0x12   //Commanded Secondary Air Status  -指令的二次空气状态
#define  LOS																0x13   //Location of Oxygen Sensors -氧传感器的位置
/*
 * Bank 1-传感器1
 * -氧传感器输出电压  单位: V
 * -短时燃油修正     单位:%
 */
#define  BANK1_SENSOR1                			0x14   //Bank 1 -Sensor 1
#define  BANK1_SENSOR2                			0x15   //Bank 1 -Sensor 2
#define  BANK1_SENSOR3                			0x16   //Bank 1 -Sensor 3
#define  BANK1_SENSOR4                			0x17   //Bank 1 -Sensor 4
#define  BANK2_SENSOR1                			0x18   //Bank 2 -Sensor 1
#define  BANK2_SENSOR2                			0x19   //Bank 2 -Sensor 2
#define  BANK2_SENSOR3                			0x1A   //Bank 2 -Sensor 3
#define  BANK2_SENSOR4                			0x1B   //Bank 2 -Sensor 4
#define  OBD_REQUIRE                     		0x1C   //OBD requirements to which vehicle is designed -对车辆设计的OBD要求
#define  LOS_1                     					0x1D   //Location of oxygen sensors
#define  AIS                     		       	0x1E   //Auxiliary Input Status -(辅助输入状态)动力输出状态
#define  TSES                     					0x1F   //Time Since Engine Start -引擎启动后的时间或发动机点火后运行时间

#define  PID_2                              0x20   //PIDs support[21-40]
#define  DISTANCE_MIL                   		0x21   //Distance Travelled While MIL is Activated -MIL亮起后的行驶距离(单位:km)
#define  FRP_1                              0x22   //Fuel Rail Pressure relative to manifold vacuum -燃油轨相对于歧管真空的压力(单位:kPa)
#define  FRP_2                              0x23   //Fuel Rail Pressure -燃油轨压力(单位:kPa)
/*
 * Bank 1 -传感器 1 (宽量程O2S)
 * -当量比(表达式)
 * -氧传感器电压
 */
#define  BANK1_SENSOR1_O2S        					0x24   //Bank 1 -Sensor 1(wide range O2S)
#define  BANK1_SENSOR2_O2S        					0x25   //Bank 1 -Sensor 2(wide range O2S)
#define  BANK1_SENSOR3_O2S        					0x26   //Bank 1 -Sensor 3(wide range O2S)
#define  BANK1_SENSOR4_O2S        					0x27   //Bank 1 -Sensor 4(wide range O2S)
#define  BANK2_SENSOR1_O2S        					0x28   //Bank 2 -Sensor 1(wide range O2S)
#define  BANK2_SENSOR2_O2S        					0x29   //Bank 2 -Sensor 2(wide range O2S)
#define  BANK2_SENSOR3_O2S        					0x2A   //Bank 2 -Sensor 3(wide range O2S)
#define  BANK2_SENSOR4_O2S        					0x2B   //Bank 2 -Sensor 4(wide range O2S)
#define  EGR                                0x2C   //Commanded EGR  -指令
#define  EGR_ERROR                         	0x2D   //EGR Error   -EGR错误(单位:%)
#define  EP                                 0x2E   //Commanded Evaporative Purge  -指令的燃油汽排出(单位:%)
#define  FLI                                0x2F   //Fuel Level Input  -燃油量输入(单位:%)
#define  WARM_UPS                         	0x30   //Number of warm-ups since diagnostic trouble codes cleared   -故障代码清空后热车次数
#define  DISTANCE_DTC_CLEAR       					0x31   //Distance since diagnostic trouble codes cleared  -故障代码清空后行驶距离  (单位:km)
#define  ESVP                               0x32   //Evap System Vapour Pressure  -EVAP蒸汽压力  (单位:Pa)
#define  BP                                 0x33   //Barometric Pressure  -气压   (单位:kPa)
/*
 * Bank 1-传感器1(宽量程O2S)
 * -当量比(表达式)
 * -氧传感器 单位:mA
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
 * 催化器温度Bank1,传感器1   单位:度
 */
#define  CT_BANK1_SENSOR_1            			0x3C    //Catalyst Temperature Bank 1,Sensor 1
#define  CT_BANK2_SENSOR_1            			0x3D    //Catalyst Temperature Bank 2,Sensor 1
#define  CT_BANK1_SENSOR_2            			0x3E    //Catalyst Temperature Bank 1,Sensor 2
#define  CT_BANK2_SENSOR_2            			0x3F    //Catalyst Temperature Bank 2,Sensor 2

#define  PID_3                              0x40    //PIDs support[41-60]
#define  MONITOR_STATUS_CYCLE      					0x41    //Monitor status this driving cycle   -监测此次驾驶循环状态
#define  MODULE_VOLTAGE                  		0x42    //Control module voltage  -控制模块电压(单位:V）
#define  ABSOLUTE_LOAD                      0x43    //Absolute Load Value -绝对载荷值(单位:%）
#define  EQUIVALENCE_RATIO                  0x45    //Relative Throttle Position - 节气门相对位置(单位:%)
#define  AMBIENT_AIR_TEMPERATURE   					0x46    //Ambient air temperature -环境气温(单位:度)
#define  ATPB                  			     		0x47    //Absolute Throttle Position B   -油门踏板位置B   (单位:%)
#define  ATPC                  			     		0x48    //Absolute Throttle Position C   -油门踏板位置C   (单位:%)
#define  ATPD                  			     		0x49    //Accelerator Pedal Position D   -油门踏板位置D  (单位:%)
#define  ATPE                  			     		0x4A   	//Accelerator Pedal Position E   -油门踏板位置E   (单位:%)
#define  TAC                  			     		0x4C   	//Commanded Throttle Actuator Control  -指令的节气门传动装置控制(单位:%) 
#define  TIME_RUN_ENGINE                   	0x4D   	//Time run by the engine while MIL is activated  -MIL灯亮起后发动机运行分钟数(单位:min)
#define  TIME_DTC_CLEAR                   	0x4E   	//Time since diagnostic trouble codes cleared  -故障代码清空后时间 (单位:min)
#define  MAX_EQUIVALENCE_RATIO       				0x4F   	//Maximum value for Equivalence Ratio
#define  MAX_AFR                            0x50    //Maximum value for Air Flow Rate from Mass Air Flow Sensor  
#define  FUEL_TYPE                          0x51   	//Type of fuel currently being utilized by the vehicle  -本车正在使用的燃油类型
#define  AFP                                0x52    //Alcohol Fuel Percentage  -酒精燃料百分比 (单位:%)
#define  AESVP                              0x53    //Absolute Evap System Vapour Pressure  -EVAP蒸汽压力(绝对值)  (单位:KPa)
#define  ESVP_1                             0x54    //Evap System Vapour Pressure  -EVAP系统蒸汽压力(单位:Pa)
#define  STSO2_BANK                         0x55    //Short Term Secondary O2 Sensor Fuel Trim Bank 1 and 3  -短时第二氧传感器燃油修正(单位:%)
#define  LTSO2_BANK                         0x56    //Long Term Secondary O2 Sensor Fuel Trim Bank 1 and 3  -长时第二氧传感器燃油修正(单位:%)
#define  STSO2_BANK_2                       0x57    //Song Term Secondary O2 Sensor Fuel Trim Bank 2 and 4  (单位:%)
#define  LTSO2_BANK_2                       0x58    //Long Term Secondary O2 Sensor Fuel Trim Bank 2 and 4  (单位:%)
#define  FUEL_RAIL_PRESSURE              		0x59    //Fuel Rail Pressure(absolute) -燃油导轨压力(绝对值)  (单位:KPa)
#define  RAPP                               0x5A   	//Relative Accelerator Pedal Position -相对加速踏板位置或油门踏板相对位置(单位:%)

#define  FUNCTION_BROADCAST_ID			0x7DE
#define  PHYSICAL_ECU1_ID           0x7E0
#define  PHYSICAL_ECU1_RESPONSE_ID  0x7E8
#define  PHYSICAL_ECU2_ID           0x7E1
#define  PHYSICAL_ECU2_RESPONSE_ID  0x7E9

/*
*********************************************************************
*  类、结构体类型定义
*********************************************************************
*/
typedef enum{
    DIAGNOSTIC_CAN_ID_STANDARD, //标准ID
    DIAGNOSTIC_CAN_ID_EXTENDED = 0x04 //扩展ID
}DiagnoticIdType;

typedef enum{
    DIAGNOSTIC_DATA_FRAME,   //数据帧
    DIAGNOSTIC_ROMOTE_FRAME  //远程帧
}DiagnoticFrameType;



typedef enum{
    DIAGNOSTIC_VIN =0x02 ,  			     //车辆识别号码
    DIAGNOSTIC_CALIBRATIN_ID = 0x04,
    DIAGNOSTIC_CALIBRATIN_VERIFICATION_NUMBER = 0x06
}DiagnosticVehicleInfoType;

/*
 *  N_PUD名（或类型）：单帧，首帧，连续帧，流控
 *
 */

/* 单帧(SF):N_PCItype = 0(7-4位)    SF_DL(3-0位)
 * -------------------------------------------------
 * 首帧(FF):N_PCItype = 1(7-4位)    FF_DL(3-0位)
 *-------------------------------------------------
 * 连续帧(CF):N_PCItype = 2(7-4位)   SN(3-0位)
 * ------------------------------------------------
 * 流控(FC):N_PCItype = 3(7-4位)    FS(3-0位)    
 */
typedef enum{
    DIAGNOSTIC_SINGLE_FRAME =0x00,  		//单帧SF			     
    DIAGNOSTIC_FIRST_FRAME = 0x10,   		//首帧FF
    DIAGNOSTIC_CONTINUOUS_FRAME = 0x20, //连续帧CF
    DIAGNOSTIC_FLOW_CONTROL = 0x30  		//流控FC
}DiagnostizFrameType;

/*
 *  OBD 9种标准诊断模式，具体含义请参考 ISO 15031-5 文档
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
 * Negative Response Code(NRC): 否定应答码,具体含义
 * 请参照ISO 15031-5 文档
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
 *   Id         							- 表示仲裁ID
 *   BitField     						- 表示位域
 *   DataField     						- 表示数据域,包括PCI+Mode+parameter.....等
 *   Length         					- 有效数据长度
 *   buf           					  - 表示发送缓存
 *   Ide           					  - 表示ID类型
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
 *   Id         							- 表示仲裁ID
 *   DataField     						- 表示数据域
 *   Length         					- 有效数据长度
 *   Pid                      - 参数ID
 *   Negative_response_code 	- 表示否定应答码
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
 *   Request         	- 表示诊断请求
 *   appcallback     	- 表示应用层诊断响应处理
 *   networkcallback  - 表示网络层诊断响应处理
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

