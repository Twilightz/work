/*
**********************************************************************************************************
*  Copyright (C), 2009-2012, ����˼׳�����з�����
*
*  ��Ŀ���ƣ� xxxx
*  
*  �ļ�����:  AppPrintf.h
*
*  �ļ������� xxxx
*             
*             
*  �� �� ��: �ʺ���
*
*  �������ڣ�2013-08-02 
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
#ifndef __APPPRINTF_H
#define __APPPRINTF_H

#ifdef __cplusplus
extern "C" {
#endif
	
#ifdef  APPPRINTF_GLOBALS
#define APPPRINTF_EXT
#else
#define APPPRINTF_EXT   extern
#endif

	


/*
*********************************************************************
*  ȫ�ֺ궨��
*********************************************************************
*/
#define _C_COMPILER_USE    _RVMDK_KEIL


/*
*********************************************************************
*  �ࡢ�ṹ�����Ͷ���
*********************************************************************
*/
enum
{
    _IAR_C_422 = 1,
    _RVMDK_KEIL ,
    _ADS_1_2 ,
    _SOURCERY_GCC, 
    _KEIL_C51,
    _RIDE,
    _AVR_GCC    
};

typedef const char   *APPLIB_ConstStrPtr;
typedef       char   *APPLIB_StrPtr;

typedef struct _applib_print_st
{
    APPLIB_StrPtr s;
    void (*outc) (char, struct _applib_print_st *);
    unsigned int i;
} APPLIB_PRINT_ST;

typedef void (*Printf_CallBack) (char, APPLIB_PRINT_ST *);

/*
*********************************************************************
*  �ⲿ���ñ�������
*********************************************************************
*/

/*
*********************************************************************
*  �ⲿ���ú�������
*********************************************************************
*/
APPPRINTF_EXT int AppLib_vprintf(APPLIB_ConstStrPtr format, va_list args, APPLIB_PRINT_ST *desc);
APPPRINTF_EXT int AppLib_sprintf(char *s, const char *format, ...);
APPPRINTF_EXT int AppLib_vsprintf(char *s, const char *format, va_list args);



#ifdef __cplusplus
}
#endif

#endif /* __APPPRINTF_H */

