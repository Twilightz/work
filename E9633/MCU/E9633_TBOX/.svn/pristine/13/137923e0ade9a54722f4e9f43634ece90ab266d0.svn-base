/******************** (C) COPYRIGHT 2012 STMicroelectronics ********************
* File Name          : lis331dlh_driver.c
* Author             : MSH Application Team
* Author             : Abhishek Anand	
* Version            : $Revision:$
* Date               : $Date:$
* Description        : LIS331DLH driver file
*                      
* HISTORY:
* Date               |	Modification                    |	Author
* 16/05/2012         |	Initial Revision                |	Abhishek Anand
* 17/05/2012         |  Modified to support multiple drivers in the same program                |	Abhishek Anand

********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* THIS SOFTWARE IS SPECIFICALLY DESIGNED FOR EXCLUSIVE USE WITH ST PARTS.
*
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "lis331dlh_driver.h"
#include "I2C.h"
#include "stm32f10x_i2c.h"
#include "Shell.h"
#include "Delay.h"
#include <stdlib.h>
#include <math.h>
#include "UartProcess.h"
#include "shell.h"
#include "bsp_iwdg.h"
uint8_t response;
u8_t i=0,g=0,flag;
u8_t position=0, old_position=0,count;
u16_t number;
u8 Rollover[7] = {0x0D, 0x00, 0x02};
//static u32 memtime;
extern AxesRaw_t data;
extern bool is_rollover_ack;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

int  I2C_BufferRead(u8_t* Data, u8_t deviceAddr,u8_t Reg,u16_t NumByteToRead);
int  I2C_ByteWrite( u8_t deviceAddr,u8_t Reg,u8_t Data);

/*******************************************************************************
* Function Name		: LIS331DLH_ReadReg
* Description		: Generic Reading function. It must be fullfilled with either
*			: I2C or SPI reading functions					
* Input			: Register Address
* Output		: Data Read
* Return		: Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
u8_t LIS331DLH_ReadReg(u8_t deviceAddr, u8_t Reg, u8_t* Data,u16_t Num) {
  
   if(!I2C_BufferRead(Data, deviceAddr, Reg,Num)) 
   return 0;
   else  
   return 1;
	
}


/*******************************************************************************
* Function Name		: LIS331DLH_WriteReg
* Description		: Generic Writing function. It must be fullfilled with either
*			: I2C or SPI writing function
* Input			: Register Address, Data to be written
* Output		: None
* Return		: Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
u8_t LIS331DLH_WriteReg(u8_t deviceAddress, u8_t WriteAddr, u8_t Data) {
  
  //To be completed with either I2c or SPI writing function
  // i.e. SPI_Mems_Write_Reg(Reg, Data);
  // i.e. I2C_ByteWrite(&Data,  deviceAddress,  WriteAddr);  
  // I2C_ByteWrite(&Data,  deviceAddress,  WriteAddr); 
	if(!I2C_ByteWrite(deviceAddress,WriteAddr,Data))
		return MEMS_ERROR;
	else
		return MEMS_SUCCESS;
}


/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : LIS331DLH_GetWHO_AM_I
* Description    : Read identification code from LIS331DLH_WHO_AM_I register
* Input          : char to be filled with the Device identification Value
* Output         : None
* Return         : Status [value of FSS]
*******************************************************************************/
status_t LIS331DLH_GetWHO_AM_I(u8_t* val){
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_WHO_AM_I, val,1) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetODR
* Description    : Sets LIS331DLH Accelerometer Output Data Rate 
* Input          : Output Data Rate
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
//设置Output Data Rate为100Hz,LIS331DLH_CTRL_REG1(0x20)寄存器的值为0xf
status_t LIS331DLH_SetODR(LIS331DLH_ODR_t dr){
  u8_t value;
  
	if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG1, &value,1) )
    return MEMS_ERROR;
  //需要加延时
	DelayMS(10);	
  value &= 0xE7;
  value |= dr<<LIS331DLH_DR;
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG1, value) )
    return MEMS_ERROR;
  return MEMS_SUCCESS;

}


/*******************************************************************************
* Function Name  : LIS331DLH_SetMode
* Description    : Sets LIS331DLH Accelerometer Operating Mode
* Input          : Modality (LIS331DLH_LOW_POWER, LIS331DLH_NORMAL, LIS331DLH_POWER_DOWN...)
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetMode(LIS331DLH_Mode_t pm) {

  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG1, &value,1) )
    return MEMS_ERROR;
	DelayMS(10);
  value &= 0x1F;
  value |= (pm<<LIS331DLH_PM);   
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG1, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetAxis
* Description    : Enable/Disable LIS331DLH Axis
* Input          : LIS331DLH_X_ENABLE/LIS331DLH_X_DISABLE | LIS331DLH_Y_ENABLE/LIS331DLH_Y_DISABLE
                   | LIS331DLH_Z_ENABLE/LIS331DLH_Z_DISABLE
* Output         : None
* Note           : You MUST use all input variable in the argument, as example
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetAxis(LIS331DLH_Axis_t axis) {
  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG1, &value,1) )
    return MEMS_ERROR;
	DelayMS(10);
  value &= 0xF8;
  value |= (0x07 & axis);
	if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG1, value) )
    return MEMS_ERROR;   
 
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetFullScale
* Description    : Sets the LIS331DLH FullScale
* Input          : LIS331DLH_FULLSCALE_2/LIS331DLH_FULLSCALE_4/LIS331DLH_FULLSCALE_8
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetFullScale(LIS331DLH_Fullscale_t fs) {
  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG4, &value,1) )
    return MEMS_ERROR;
	DelayMS(10);
  value &= 0xCF;	
  value |= (fs<<LIS331DLH_FS);
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG4, value) )
    return MEMS_ERROR;
	
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetBDU
* Description    : Enable/Disable Block Data Update Functionality
* Input          : MEMS_ENABLE/MEMS_DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetBDU(State_t bdu) {
  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG4, &value,1) )
    return MEMS_ERROR;
  
  value &= 0x7F;
  value |= (bdu<<LIS331DLH_BDU);
  
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG4, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetBLE
* Description    : Set Endianess (MSB/LSB)
* Input          : LIS331DLH_BLE_LSB / LIS331DLH_BLE_MSB
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetBLE(LIS331DLH_Endianess_t ble) {
  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG4, &value,1) )
    return MEMS_ERROR;
  
  value &= 0xBF;	
  value |= (ble<<LIS331DLH_BLE);
  
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG4, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetFDS
* Description    : Set Filter Data Selection
* Input          : MEMS_ENABLE/MEMS_DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetFDS(State_t fds) {
  u8_t value;
  
 if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG2, &value,1) )
    return MEMS_ERROR;
 
  DelayMS(10);
  value &= 0xEF;	
  value |= (fds<<LIS331DLH_FDS);
  
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG2, value) )
    return MEMS_ERROR;
	 
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetBOOT
* Description    : Rebot memory content
* Input          : MEMS_ENABLE/MEMS_DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetBOOT(State_t boot) {
  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG2, &value,1) )
    return MEMS_ERROR;
  
  value &= 0x7F;	
  value |= (boot<<LIS331DLH_BOOT);
  
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG2, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetSelfTest
* Description    : Set Self Test Modality
* Input          : MEMS_DISABLE/MEMS_ENABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetSelfTest(State_t st) {
  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG4, &value,1) )
    return MEMS_ERROR;
  
  value &= 0xFD;
  value |= (st<<LIS331DLH_ST);
  
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG4, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetSelfTestSign
* Description    : Set Self Test Sign (Disable = st_plus, Enable = st_minus)
* Input          : MEMS_DISABLE/MEMS_ENABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetSelfTestSign(State_t st_sign) {
  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG4, &value,1) )
    return MEMS_ERROR;
  
  value &= 0xF7;
  value |= (st_sign<<LIS331DLH_ST_SIGN);
  
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG4, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetIntHighLow
* Description    : Set Interrupt active state (Disable = active high, Enable = active low)
* Input          : MEMS_DISABLE/MEMS_ENABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetIntHighLow(State_t ihl) {
  u8_t value;
  
	if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG3, &value,1) )
    return MEMS_ERROR;
	
	DelayMS(20);
  value &= 0x7F;
  value |= (ihl<<LIS331DLH_IHL);
  
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG3, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetIntPPOD
* Description    : Set Interrupt Push-Pull/OpenDrain Pad (Disable = Push-Pull, Enable = OpenDrain)
* Input          : MEMS_DISABLE/MEMS_ENABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetIntPPOD(State_t pp_od) {
  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG3, &value,1) )
    return MEMS_ERROR;
  
  value &= 0xBF;
  value |= (pp_od<<LIS331DLH_PP_OD);
  
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG3, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetInt1DataSign
* Description    : Set Data signal Interrupt 1 pad
* Input          : Modality by LIS331DLH_INT_Conf_t Typedef 
                  (LIS331DLH_INT_SOURCE, LIS331DLH_INT_1OR2_SOURCE, LIS331DLH_DATA_READY, LIS331DLH_BOOT_RUNNING)
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetInt1DataSign(LIS331DLH_INT_Conf_t i_cfg) {
  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG3, &value,1) )
    return MEMS_ERROR;
  
  value &= 0xFC;
  value |= (i_cfg<<LIS331DLH_I1_CFG);
  
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG3, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetInt2DataSign
* Description    : Set Data signal Interrupt 2 pad
* Input          : Modality by LIS331DLH_INT_Conf_t Typedef 
                  (LIS331DLH_INT_SOURCE, LIS331DLH_INT_1OR2_SOURCE, LIS331DLH_DATA_READY, LIS331DLH_BOOT_RUNNING)
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetInt2DataSign(LIS331DLH_INT_Conf_t i_cfg) {
  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG3, &value,1) )
    return MEMS_ERROR;
  
  value &= 0xE7;
  value |= (i_cfg<<LIS331DLH_I2_CFG);
  
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG3, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetSPI34Wire
* Description    : Set SPI mode 
* Input          : Modality by LIS331DLH_SPIMode_t Typedef (LIS331DLH_SPI_4_WIRE, LIS331DLH_SPI_3_WIRE)
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetSPI34Wire(LIS331DLH_SPIMode_t sim) {
  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG4, &value,1) )
    return MEMS_ERROR;
  
  value &= 0xFE;
  value |= (sim<<LIS331DLH_SIM);
  
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG4, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_TurnONEnable
* Description    : TurnON Mode selection for sleep to wake function
* Input          : LIS331DLH_SLEEP_TO_WAKE_DIS/LIS331DLH_SLEEP_TO_WAKE_ENA
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_TurnONEnable(LIS331DLH_Sleep_To_Wake_Conf_t stw) {
  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG5, &value,1) )
    return MEMS_ERROR;
  DelayMS(10);
  value &= 0x00;
  value |= (stw<<LIS331DLH_TURN_ON);
  
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG5, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_HPFilterReset
* Description    : Reading register for reset the content of internal HP filter
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_HPFilterReset(void) {
  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_HP_FILTER_RESET, &value,1) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetReference
* Description    : Sets Reference register acceleration value as a reference for HP filter
* Input          : Value of reference acceleration value (0-255)
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetReference(i8_t ref) {
  
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_REFERENCE_REG, ref) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetHPFMode
* Description    : Set High Pass Filter Modality
* Input          : LIS331DLH_HPM_NORMAL_MODE_RES/LIS331DLH_HPM_REF_SIGNAL/LIS331DLH_HPM_NORMAL_MODE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetHPFMode(LIS331DLH_HPFMode_t hpm) {
  u8_t value = 0;
  

	if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG2, &value,1) )
    return MEMS_ERROR;
  value &= 0x9F;
  value |= (hpm<<LIS331DLH_HPM);

  
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG2, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetHPFCutOFF
* Description    : Set High Pass CUT OFF Freq
* Input          : LIS331DLH_HPFCF [0,3]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetHPFCutOFF(LIS331DLH_HPFCutOffFreq_t hpf) {
  u8_t value = 0;
  

  value &= 0xFC;
  value |= (hpf<<LIS331DLH_HPCF);
	
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG2, value) )
    return MEMS_ERROR;
    if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG2, &value,1) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
  
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetInt2HPEnable
* Description    : Set Interrupt2 hp filter enable/disable
* Input          : MEMS_ENABLE/MEMS_DISABLE
* example        : LIS331DLH_SetInt2HPEnable(MEMS_ENABLE)
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetInt2HPEnable(State_t stat) {
  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG2, &value,1) )
    return MEMS_ERROR;
  
  value &= 0xF7;
  value |= stat<<LIS331DLH_HPEN2 ;
  
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG2, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}     


/*******************************************************************************
* Function Name  : LIS331DLH_SetInt1HPEnable
* Description    : Set Interrupt1 hp filter enable/disable
* Input          : MEMS_ENABLE/MEMS_DISABLE
* example        : LIS331DLH_SetInt1HPEnable(MEMS_ENABLE)
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetInt1HPEnable(State_t stat) {
  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG2, &value,1) )
    return MEMS_ERROR;
  
  value &= 0xFB;
  value |= stat<<LIS331DLH_HPEN1 ;
  
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG2, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}  


/*******************************************************************************
* Function Name  : LIS331DLH_Int1LatchEnable
* Description    : Enable Interrupt 1 Latching function
* Input          : ENABLE/DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_Int1LatchEnable(State_t latch) {
  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG3, &value,1) )
    return MEMS_ERROR;
  
  value &= 0xFB;
  value |= latch<<LIS331DLH_LIR1;
  
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG3, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_Int2LatchEnable
* Description    : Enable Interrupt 2 Latching function
* Input          : ENABLE/DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_Int2LatchEnable(State_t latch) {
  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG3, &value,1) )
    return MEMS_ERROR;
  
  value &= 0xDF;
  value |= latch<<LIS331DLH_LIR2;
  
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_CTRL_REG3, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_ResetInt1Latch
* Description    : Reset Interrupt 1 Latching function
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_ResetInt1Latch(void) {
  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_INT1_SRC, &value,1) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_ResetInt2Latch
* Description    : Reset Interrupt 2 Latching function
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_ResetInt2Latch(void) {
  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_INT2_SRC, &value,1) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetInt1Configuration
* Description    : Interrupt 1 Configuration (without 6D_INT)
* Input          : LIS331DLH_INT_AND/OR | LIS331DLH_INT_ZHIE_ENABLE/DISABLE | LIS331DLH_INT_ZLIE_ENABLE/DISABLE...
* Output         : None
* Note           : You MUST use ALL input variable in the argument, as in example above
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetInt1Configuration(LIS331DLH_IntConf_t ic) {
  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_INT1_CFG, &value,1) )
    return MEMS_ERROR;
	DelayMS(20);
  value &= 0x40; 
  value |= ic;
  
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_INT1_CFG, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetInt2Configuration
* Description    : Interrupt 2 Configuration (without 6D_INT)
* Input          : LIS331DLH_INT_AND/OR | LIS331DLH_INT_ZHIE_ENABLE/DISABLE | LIS331DLH_INT_ZLIE_ENABLE/DISABLE...
* Output         : None
* Note           : You MUST use all input variable in the argument, as example
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetInt2Configuration(LIS331DLH_IntConf_t ic) {
  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_INT2_CFG, &value,1) )
    return MEMS_ERROR;
  
  value &= 0x40; 
  value |= ic;
  
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_INT2_CFG, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetInt1Mode
* Description    : Interrupt 1 Configuration mode (OR, 6D Movement, AND, 6D Position)
* Input          : LIS331DLH_INT_MODE_OR, LIS331DLH_INT_MODE_6D_MOVEMENT, LIS331DLH_INT_MODE_AND, LIS331DLH_INT_MODE_6D_POSITION
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetInt1Mode(LIS331DLH_IntMode_t int_mode) {
  u8_t value = 0;
  
  value &= 0x3F; 
  value |= (int_mode<<LIS331DLH_INT_6D);
	
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_INT1_CFG, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetInt2Mode
* Description    : Interrupt 2 Configuration mode (OR, 6D Movement, AND, 6D Position)
* Input          : LIS331DLH_INT_MODE_OR, LIS331DLH_INT_MODE_6D_MOVEMENT, LIS331DLH_INT_MODE_AND, LIS331DLH_INT_MODE_6D_POSITION
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetInt2Mode(LIS331DLH_IntMode_t int_mode) {
  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_INT2_CFG, &value,1) )
    return MEMS_ERROR;
  
  value &= 0x3F; 
  value |= (int_mode<<LIS331DLH_INT_6D);
  
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_INT2_CFG, value) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_Get6DPositionInt1
* Description    : 6D Interrupt 1 Position Detect
* Input          : Byte to be filled with LIS331DLH_POSITION_6D_t Typedef
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
	status_t LIS331DLH_Get6DPositionInt1(u8_t* val){
  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_INT1_SRC, &value,1) )
    return MEMS_ERROR;
  
  value &= 0x7F;
  
  switch (value){
  case LIS331DLH_UP_SX:   
    *val = LIS331DLH_UP_SX;    
    break;
  case LIS331DLH_UP_DX:   
    *val = LIS331DLH_UP_DX;    
    break;
  case LIS331DLH_DW_SX:   
    *val = LIS331DLH_DW_SX;    
    break;
  case LIS331DLH_DW_DX:   
    *val = LIS331DLH_DW_DX;    
    break;
  case LIS331DLH_TOP:     
    *val = LIS331DLH_TOP;      
    break;
  case LIS331DLH_BOTTOM:  
    *val = LIS331DLH_BOTTOM;  
    break;
  }
  
  return MEMS_SUCCESS;  
}


/*******************************************************************************
* Function Name  : LIS331DLH_Get6DPositionInt2
* Description    : 6D Interrupt 2 Position Detect
* Input          : Byte to be filled with LIS331DLH_POSITION_6D_t Typedef
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_Get6DPositionInt2(u8_t* val){
  u8_t value;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_INT2_SRC, &value,1) )
    return MEMS_ERROR;
  
  value &= 0x7F;
  
  switch (value){
  case LIS331DLH_UP_SX:   
    *val = LIS331DLH_UP_SX;    
    break;
  case LIS331DLH_UP_DX:   
    *val = LIS331DLH_UP_DX;    
    break;
  case LIS331DLH_DW_SX:   
    *val = LIS331DLH_DW_SX;    
    break;
  case LIS331DLH_DW_DX:   
    *val = LIS331DLH_DW_DX;    
    break;
  case LIS331DLH_TOP:     
    *val = LIS331DLH_TOP;      
    break;
  case LIS331DLH_BOTTOM:  
    *val = LIS331DLH_BOTTOM;   
    break;
  }
  
  return MEMS_SUCCESS;  
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetInt1Threshold
* Description    : Sets Interrupt 1 Threshold
* Input          : Threshold = [0,127]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetInt1Threshold(u8_t ths) {
	u8_t value;

  if (ths > 127)
    return MEMS_ERROR;

  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_INT1_THS, ths) )
    return MEMS_ERROR;    

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetInt1Duration
* Description    : Sets Interrupt 1 Duration
* Input          : Duration = [0,127]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetInt1Duration(u8_t id) {  
  if (id > 127)
    return MEMS_ERROR;
  
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_INT1_DURATION, id) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetInt2Threshold
* Description    : Sets Interrupt 2 Threshold
* Input          : Threshold = [0,127]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetInt2Threshold(u8_t ths) {
  if (ths > 127)
    return MEMS_ERROR;
  
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_INT2_THS, ths) )
    return MEMS_ERROR;    
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_SetInt2Duration
* Description    : Sets Interrupt 2 Duration
* Input          : Duration = [0,127]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_SetInt2Duration(u8_t id) {  
  if (id > 127)
    return MEMS_ERROR;
  
  if( !LIS331DLH_WriteReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_INT2_DURATION, id) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_GetStatusReg
* Description    : Read the status register
* Input          : char to empty by Status Reg Value
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_GetStatusReg(u8_t* val) {
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_STATUS_REG, val,1) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;  
}

/*******************************************************************************
* Function Name  : LIS331DLH_GetStatusBIT
* Description    : Read the status register BIT
* Input          : LIS331DLH_STATUS_REG_ZYXOR, LIS331DLH_STATUS_REG_ZOR, LIS331DLH_STATUS_REG_YOR, LIS331DLH_STATUS_REG_XOR,
                   LIS331DLH_STATUS_REG_ZYXDA, LIS331DLH_STATUS_REG_ZDA, LIS331DLH_STATUS_REG_YDA, LIS331DLH_STATUS_REG_XDA, 
                   LIS331DLH_DATAREADY_BIT
* Output         : status register BIT
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_GetStatusBit(u8_t statusBIT, u8_t *val) {
  u8_t value;  
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_STATUS_REG, &value,1) )
    return MEMS_ERROR;
  
  switch (statusBIT){
  case LIS331DLH_STATUS_REG_ZYXOR:     
    if(value &= LIS331DLH_STATUS_REG_ZYXOR){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }  
  case LIS331DLH_STATUS_REG_ZOR:       
    if(value &= LIS331DLH_STATUS_REG_ZOR){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }  
  case LIS331DLH_STATUS_REG_YOR:       
    if(value &= LIS331DLH_STATUS_REG_YOR){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }                                 
  case LIS331DLH_STATUS_REG_XOR:       
    if(value &= LIS331DLH_STATUS_REG_XOR){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }  
  case LIS331DLH_STATUS_REG_ZYXDA:     
    if(value &= LIS331DLH_STATUS_REG_ZYXDA){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }   
  case LIS331DLH_STATUS_REG_ZDA:       
    if(value &= LIS331DLH_STATUS_REG_ZDA){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }   
  case LIS331DLH_STATUS_REG_YDA:       
    if(value &= LIS331DLH_STATUS_REG_YDA){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }   
  case LIS331DLH_STATUS_REG_XDA:       
    if(value &= LIS331DLH_STATUS_REG_XDA){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }                                      
  }
  
  return MEMS_ERROR;
}


/*******************************************************************************
* Function Name  : LIS331DLH_GetAccAxesRaw
* Description    : Read the Acceleration Values Output Registers
* Input          : buffer to empity by AccAxesRaw_t Typedef
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_GetAccAxesRaw(AxesRaw_t* buff) {
  i16_t value;
  u8_t *valueL = (u8_t *)(&value);
  u8_t *valueH = ((u8_t *)(&value)+1);
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_OUT_X_L, valueL,1) )
    return MEMS_ERROR;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_OUT_X_H, valueH,1) )
    return MEMS_ERROR;
  
  buff->AXIS_X = value/16;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_OUT_Y_L, valueL,1) )
    return MEMS_ERROR;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_OUT_Y_H, valueH,1) )
    return MEMS_ERROR;
  
  buff->AXIS_Y = value/16;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_OUT_Z_L, valueL,1) )
    return MEMS_ERROR;
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_OUT_Z_H, valueH,1) )
    return MEMS_ERROR;
 
  buff->AXIS_Z = value/16;
  
  return MEMS_SUCCESS;  
}


/*******************************************************************************
* Function Name  : LIS331DLH_GetInt1Src
* Description    : Reset Interrupt 1 Latching function
* Input          : buffer to empty by Int1 Source Value
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_GetInt1Src(u8_t* val) {  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_INT1_SRC, val,1) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_GetInt2Src
* Description    : Reset Interrupt 2 Latching function
* Input          : buffer to empty by Int2 Source Value
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_GetInt2Src(u8_t* val) {  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_INT2_SRC, val,1) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LIS331DLH_GetInt1SrcBit
* Description    : Reset Interrupt 1 Latching function
* Input          : LIS331DLH_INT1_SRC_IA, LIS331DLH_INT1_SRC_ZH, LIS331DLH_INT1_SRC_ZL .....
* Output         : None
* Return         : Status of BIT [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_GetInt1SrcBit(u8_t statusBIT, u8_t *val) {
  u8_t value;  
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_INT1_SRC, &value,1) )
    return MEMS_ERROR;
  
  if(statusBIT == LIS331DLH_INT_SRC_IA){
    if(value &= LIS331DLH_INT_SRC_IA){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }    
  }
  
  if(statusBIT == LIS331DLH_INT_SRC_ZH){
    if(value &= LIS331DLH_INT_SRC_ZH){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }   
  }
  
  if(statusBIT == LIS331DLH_INT_SRC_ZL){
    if(value &= LIS331DLH_INT_SRC_ZL){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }   
  }
  
  if(statusBIT == LIS331DLH_INT_SRC_YH){
    if(value &= LIS331DLH_INT_SRC_YH){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }   
  }
  
  if(statusBIT == LIS331DLH_INT_SRC_YL){
    if(value &= LIS331DLH_INT_SRC_YL){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }  
  }
  
  if(statusBIT == LIS331DLH_INT_SRC_XH){
    if(value &= LIS331DLH_INT_SRC_XH){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }   
  }
  
  if(statusBIT == LIS331DLH_INT_SRC_XL){
    if(value &= LIS331DLH_INT_SRC_XL){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }    
  } 
  return MEMS_ERROR;
}


/*******************************************************************************
* Function Name  : LIS331DLH_GetInt2SrcBit
* Description    : Reset Interrupt 2 Latching function
* Input          : LIS331DLH_INT_SRC_IA, LIS331DLH_INT_SRC_ZH, LIS331DLH_INT_SRC_ZL .....
* Output         : None
* Return         : Status of BIT [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS331DLH_GetInt2SrcBit(u8_t statusBIT, u8_t *val) {
  u8_t value;  
  
  if( !LIS331DLH_ReadReg(LIS331DLH_MEMS_I2C_ADDRESS, LIS331DLH_INT2_SRC, &value,1) )
    return MEMS_ERROR;
  
  if(statusBIT == LIS331DLH_INT_SRC_IA){
    if(value &= LIS331DLH_INT_SRC_IA){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }   
  }
  
  if(statusBIT == LIS331DLH_INT_SRC_ZH){
    if(value &= LIS331DLH_INT_SRC_ZH){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }  
  }
  
  if(statusBIT == LIS331DLH_INT_SRC_ZL){
    if(value &= LIS331DLH_INT_SRC_ZL){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }  
  }
  
  if(statusBIT == LIS331DLH_INT_SRC_YH){
    if(value &= LIS331DLH_INT_SRC_YH){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }    
  }
  
  if(statusBIT == LIS331DLH_INT_SRC_YL){
    if(value &= LIS331DLH_INT_SRC_YL){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }   
  }
  
  if(statusBIT == LIS331DLH_INT_SRC_XH){
    if(value &= LIS331DLH_INT_SRC_XH){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }  
  }
  
  if(statusBIT == LIS331DLH_INT_SRC_XL){
    if(value &= LIS331DLH_INT_SRC_XL){     
      *val = MEMS_SET;
      return MEMS_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return MEMS_SUCCESS;
    }    
  } 
  return MEMS_ERROR;
}


int  I2C_BufferRead(u8_t* Data, u8_t deviceAddr,u8_t Reg,u16_t NumByteToRead) 
{
    unsigned int x = 0x007f;
	
	  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
		{
			  IWDG_Feed();
				x -- ;
        if(x <= 1)
           return 0;
		};
		x = 0x007f;
		/* send a start condition to I2C bus */
	  I2C_GenerateSTART(I2C1,ENABLE);

		/* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		{
			 IWDG_Feed();
			 x -- ;
       if(x <= 1)
           return 0;
		};

		/* send slave address to I2C bus */
		I2C_Send7bitAddress(I2C1,deviceAddr,I2C_Direction_Transmitter);
    x = 0x007f;
		/* Test on EV6 and clear it */
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		{
		   IWDG_Feed();
			 x -- ;
       if(x <= 1)
           return 0;
		};
		/* Clear EV6 by setting again the PE bit */
		I2C_Cmd(I2C1, ENABLE);
	
		I2C_SendData(I2C1,Reg);
	  x = 0x007f;
		/* Test on EV8 and clear it */
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			 IWDG_Feed();
			 x -- ;
       if(x <= 1)
          return 0;
		};
	
	  I2C_GenerateSTART(I2C1,ENABLE);
	  x = 0x007f;
    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		{
			  IWDG_Feed();
				x -- ;
        if(x <= 1)
           return 0;
		};

		/* send slave address to I2C bus */
		I2C_Send7bitAddress(I2C1,deviceAddr,I2C_Direction_Receiver);
		x = 0x007f;
		/* Test on EV6 and clear it */
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		{		
			  IWDG_Feed();
			  x -- ;
        if(x <= 1)
           return 0;
		};
 
		/* While there is data to be read */
		while(NumByteToRead)  
		{
			if(NumByteToRead == 1)
			{
				/* Disable Acknowledgement */
				I2C_AcknowledgeConfig(I2C1, DISABLE);
      
				/* Send STOP Condition */
				I2C_GenerateSTOP(I2C1, ENABLE);
			}
			/* Test on EV7 and clear it */
			if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  
			{      
				/* Read a byte from the EEPROM */
				*Data = I2C_ReceiveData(I2C1);

        /* Point to the next location where the byte read will be saved */
				Data++; 
      
        /* Decrement the read bytes counter */
        NumByteToRead--;        
			}   
		}
		/* Enable Acknowledgement to be ready for another reception */
		I2C_AcknowledgeConfig(I2C1, ENABLE);
		return 1;
}


int  I2C_ByteWrite( u8_t deviceAddr,u8_t Reg,u8_t Data)
{
		int  falg;
    unsigned int x = 0x007f;
	
		falg =0;  //I2C_FLAG_SB

	
		/* Send STRAT condition */
		I2C_GenerateSTART(I2C1, ENABLE);
	
	  /* Test on EV5 and clear it */
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		{
			 IWDG_Feed();
			 x -- ;
       if(x <= 1)
           return 0;
		}
	  /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C1, deviceAddr, I2C_Direction_Transmitter);
   
		x = 0x007f;
	  /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		{
			 IWDG_Feed();
			 x -- ;
       if(x <= 1)
           return 0;
		}

	  /* Send the EEPROM's internal address to write to */
		I2C_SendData(I2C1, Reg);
	
		x = 0x007f;
	  /* Test on EV8 and clear it */
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			 IWDG_Feed();
			 x -- ;
       if(x <= 1)
           return 0;
		}
    /* Send the byte to be written */
    I2C_SendData(I2C1,Data); 
		
		x = 0x007f;
    /*!&lt; Wait till all data have been physically transferred on the bus */
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			 IWDG_Feed();
			 x -- ;
       if(x <= 1)
           return 0;
		}
		/* Send STOP condition */
		I2C_GenerateSTOP(I2C1, ENABLE);
		falg=1;
	  return falg ;
}

status_t LIS331DLH_Init(void)
{
	 status_t response;
	 //set ODR (turn ON device)
   response = LIS331DLH_SetODR( LIS331DLH_ODR_100Hz);

   response = LIS331DLH_SetMode( LIS331DLH_NORMAL);

	 //set Fullscale
	 response = LIS331DLH_SetFullScale( LIS331DLH_FULLSCALE_2);
	 
	 //set axis Enable
	 response = LIS331DLH_SetAxis( LIS331DLH_X_ENABLE | LIS331DLH_Y_ENABLE |  LIS331DLH_Z_ENABLE);
}

void LIS331DLH_Sleep_Mode(void)
{
	 LIS331DLH_SetMode(LIS331DLH_LOW_POWER_10);
}

void LIS331DLH_Config_Interrupt(void)
{
	 //write 00h into CTRL_REG2
	 LIS331DLH_SetFDS(MEMS_DISABLE);
	 //write 00h into CTRL_REG3
	 LIS331DLH_SetIntHighLow(MEMS_DISABLE);
	 //write 00h into CTRL_REG4
	 LIS331DLH_SetFullScale( LIS331DLH_FULLSCALE_2);
	 //write 00h into CTRL_REG5
	 LIS331DLH_TurnONEnable(LIS331DLH_SLEEP_TO_WAKE_DIS);
	 //write 10h into INT1_THS
	 LIS331DLH_SetInt1Threshold(0x8);
	 //write 00h into INT1_DURATION
	 LIS331DLH_SetInt1Duration(0);
	 //
	 LIS331DLH_SetInt1Configuration(LIS331DLH_INT_XHIE_ENABLE|LIS331DLH_INT_YHIE_ENABLE);
}
void LIS33DH_Test(void)
{
	 response = LIS331DLH_Get6DPositionInt1(&position);
	 if((response==1) && (old_position!=position)){
	   switch (position){
				case  LIS331DLH_UP_SX:	
						//printf("\r\n position = UP_SX_ !");
						break;
				case  LIS331DLH_UP_DX:
						//printf("\r\n position = UP_DX_ !");
						break;
				case  LIS331DLH_DW_SX:
						//printf("\r\n position = dw_SX_ !");
						break; 			 
				case  LIS331DLH_DW_DX:	
						//printf("\r\n position = dw_DX_ !");
						break; 
				case  LIS331DLH_TOP: 	
						//printf("\r\n position = h_TOP_ !");
						break; 
				case  LIS331DLH_BOTTOM:	
						//printf("\r\n position = h_BOTTOM_ !");
						break; 
				default: 	 
						//printf("\r\n position = UP_unkown_ !");
						break;    
	   } 
	 }
}

signed char lis331dlh_Get_Angle(AxesRaw_t* data)
{
	  double angle;
    signed char Rollovertemp;
	  //计算侧翻
		angle =atan2((double)data->AXIS_X,(double)data->AXIS_Z)*(180/3.14159265);//180/3.14159265
		Rollovertemp = (signed char)angle;
	  if(Rollovertemp >= (signed char)166)
		{
				Rollovertemp = Rollovertemp - 256;
		}
	
	  if(Rollovertemp != 0)
		{
			  //左侧翻
			  if(-45>=Rollovertemp && is_rollover_ack == FALSE)
				{
            Rollovertemp = ~Rollovertemp + 1;
					  is_rollover_ack = TRUE;
						Rollover[3] = LEFT_TURN;
					  Rollover[4] = Rollovertemp;
						senddatapacket(&COM2,Rollover,FIXED_LENGTH+2+CHECKSUM);
				}
				//右侧翻
				if(45<=Rollovertemp && is_rollover_ack == FALSE)
				{
					  is_rollover_ack = TRUE;
						Rollover[3] = TURN_RIGHT;
					  Rollover[4] = Rollovertemp;
						senddatapacket(&COM2,Rollover,FIXED_LENGTH+2+CHECKSUM);
				}
				if(Rollovertemp>-45 && Rollovertemp<45 && is_rollover_ack == TRUE)
				{
					  is_rollover_ack = FALSE;
				}
		}
		return Rollovertemp;
}

//threhold数组的参数决定了震动的幅度，需要根据自己的要求设定 

//150->轻微路段(限速带） 
static u16_t threhold[5] = {0,800};//value 10000 means turn off collision check
//static short threhold[4] = {300,600,1200,10000};//

//若返回1，则表示震动。若返回0，则表示不震动
char lis331dlh_check_collision(u8_t level,AxesRaw_t* data)//检测是否震动
{		
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	 static i16_t prev_x = 0,prev_y = 0,prev_z = 0;		
	 static u8_t b_is_first = 0;		
	 int8_t is_collision = 0;	

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	 if(b_is_first == 0)		
	 {		
		  prev_x = data->AXIS_X;				
		  prev_y = data->AXIS_Y;				
		  prev_z = data->AXIS_Z;								
		  b_is_first = 1;								
		  return 0;							
	 }					
	 if((abs(data->AXIS_X - prev_x) > threhold[level])||(abs(data->AXIS_Y - prev_y) > threhold[level])||(abs(data->AXIS_Z - prev_z) > threhold[level]))		
	 {		
		  is_collision = 1;	
   }
	 prev_x = data->AXIS_X;		
	 prev_y = data->AXIS_Y;		
	 prev_z = data->AXIS_Z;	
	 return is_collision;
}



/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/
