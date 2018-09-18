/*
********************************************************************************
*  Copyright (C), 2009-2012, ����˼׳�����з�����
*
*  ��Ŀ���ƣ�VS331
*  
*  �ļ�����: CanDrv.h
*
*  �ļ�������CANӲ������ͷ�ļ�
*             
*             
*  �� �� ��: �ʺ���
*
*  �������ڣ�2014-04-04 
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

#ifndef __CANDRV_H
#define __CANDRV_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef  CANDRV_GLOBALS
#define CANDRV_EXT
#else
#define CANDRV_EXT   extern
#endif

#include "stm32f10x.h"

/*
*********************************************************************
*  ȫ�ֺ궨��
*********************************************************************
*/
// CAN���롢������г��ȶ���
#define    LIST_CAN1_IN_NUM      128
#define    LIST_CAN2_IN_NUM      128

#define    LIST_CAN1_OUT_NUM     128
#define    LIST_CAN2_OUT_NUM     128

/*#define    CANQUE_TO_CANTXMSG(msg, que)  do{\
    (msg)->ExtId = (que)->Id;\
    (msg)->IDE = CAN_Id_Extended;\
    (msg)->RTR = CAN_RTR_Data;\
    (msg)->DLC = (que)->Dlc;\
    (msg)->Data[0] = (que)->Data[0];\
    (msg)->Data[1] = (que)->Data[1];\
    (msg)->Data[2] = (que)->Data[2];\
    (msg)->Data[3] = (que)->Data[3];\
    (msg)->Data[4] = (que)->Data[4];\
    (msg)->Data[5] = (que)->Data[5];\
    (msg)->Data[6] = (que)->Data[6];\
    (msg)->Data[7] = (que)->Data[7];\
}while(0)
*/

/*
*********************************************************************
*  �ࡢ�ṹ�����Ͷ���
*********************************************************************
*/
typedef struct CANQUEDATA_STRUCT
{
    u32 Id;        // ������չID
    u8  Data[8];   // ������
    u8  Dlc;       // ���ݳ���
	 u8  IDE;
	  u8  RTR;
}CANQUEDATA;

// �������ݶ��нṹ��
typedef struct canqueuelist_t
{
    u16       head;              /* ����ǰָ�� */
    u16       trail;             /* ���е�ָ�� */
    u16       lenmax;            /* ���г���   */
    CANQUEDATA  *pdata;          /* ��Ϣ����   */
}CANQUEUELIST;


#ifdef CANQUE_USEOS
#include  <ucos_ii.h>
#endif

// ���ڳ�ʼ���ṹ��
typedef struct canx_t
{
    CAN_TypeDef     * pCanx;
    CANQUEUELIST    * pQueueOut;
    CANQUEUELIST    * pQueueIn;
#ifdef CANQUE_USEOS
    OS_EVENT        * pWrBufSem;
#endif
}CANX;

typedef enum
{
    CANX_BAUD_50K   = 50000,
    CANX_BAUD_100K  = 100000,
    CANX_BAUD_125K  = 125000,
    CANX_BAUD_250K  = 250000,
    CANX_BAUD_500K  = 500000,
    CANX_BAUD_1000K = 1000000,
}CANXBAUD;

/*
*********************************************************************
*  �ⲿ���ñ�������
*********************************************************************
*/
CANDRV_EXT   CANX  Can1;
CANDRV_EXT   CANX  Can2;

/*
*********************************************************************
*  �ⲿ���ú�������
*********************************************************************
*/
void CANFilterInit(u8 isFilter);
CANDRV_EXT    void CAN1Init     (CANX *pcan, CANXBAUD baud, u8 is_Filter);
CANDRV_EXT    void CAN2Init			(CANX *pcan, CANXBAUD baud, u8 is_Filter);
CANDRV_EXT    u8   CanTransmit  (CANX *pcan, CANQUEDATA *pdata);
CANDRV_EXT    u8   CanReceive   (CANX *pcan, CANQUEDATA *pdata);


#ifdef __cplusplus
}
#endif

#endif