/*
********************************************************************************
*  Copyright (C), 2009-2012, 合众思壮西安研发中心
*
*  项目名称：E6202
*  
*  文件名称: CipherFunc.c
*
*  文件描述：加密解密源文件
*             
*             
*  创 建 者: 皇海明
*
*  创建日期：2013-03-20 
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

#define CIPHERFUNC_GLB
#include "CipherFunc.h"
#include "Uart.h"
#include "Shell.h"





/*
********************************************************************************                                                                  
*  内部函数声明                                                                                                        
********************************************************************************
*/



/*
********************************************************************************                                                                 
*  内部变量定义                                                                                                         
********************************************************************************
*/
const unsigned long  CRC32Table[256] =
{
    0x00000000L, 0x77073096L, 0xEE0E612CL, 0x990951BAL,
    0x076DC419L, 0x706AF48FL, 0xE963A535L, 0x9E6495A3L,
    0x0EDB8832L, 0x79DCB8A4L, 0xE0D5E91EL, 0x97D2D988L,
    0x09B64C2BL, 0x7EB17CBDL, 0xE7B82D07L, 0x90BF1D91L,
    0x1DB71064L, 0x6AB020F2L, 0xF3B97148L, 0x84BE41DEL,
    0x1ADAD47DL, 0x6DDDE4EBL, 0xF4D4B551L, 0x83D385C7L,
    0x136C9856L, 0x646BA8C0L, 0xFD62F97AL, 0x8A65C9ECL,
    0x14015C4FL, 0x63066CD9L, 0xFA0F3D63L, 0x8D080DF5L,
    0x3B6E20C8L, 0x4C69105EL, 0xD56041E4L, 0xA2677172L,
    0x3C03E4D1L, 0x4B04D447L, 0xD20D85FDL, 0xA50AB56BL,
    0x35B5A8FAL, 0x42B2986CL, 0xDBBBC9D6L, 0xACBCF940L,
    0x32D86CE3L, 0x45DF5C75L, 0xDCD60DCFL, 0xABD13D59L,
    0x26D930ACL, 0x51DE003AL, 0xC8D75180L, 0xBFD06116L,
    0x21B4F4B5L, 0x56B3C423L, 0xCFBA9599L, 0xB8BDA50FL,
    0x2802B89EL, 0x5F058808L, 0xC60CD9B2L, 0xB10BE924L,
    0x2F6F7C87L, 0x58684C11L, 0xC1611DABL, 0xB6662D3DL,
    0x76DC4190L, 0x01DB7106L, 0x98D220BCL, 0xEFD5102AL,
    0x71B18589L, 0x06B6B51FL, 0x9FBFE4A5L, 0xE8B8D433L,
    0x7807C9A2L, 0x0F00F934L, 0x9609A88EL, 0xE10E9818L,
    0x7F6A0DBBL, 0x086D3D2DL, 0x91646C97L, 0xE6635C01L,
    0x6B6B51F4L, 0x1C6C6162L, 0x856530D8L, 0xF262004EL,
    0x6C0695EDL, 0x1B01A57BL, 0x8208F4C1L, 0xF50FC457L,
    0x65B0D9C6L, 0x12B7E950L, 0x8BBEB8EAL, 0xFCB9887CL,
    0x62DD1DDFL, 0x15DA2D49L, 0x8CD37CF3L, 0xFBD44C65L,
    0x4DB26158L, 0x3AB551CEL, 0xA3BC0074L, 0xD4BB30E2L,
    0x4ADFA541L, 0x3DD895D7L, 0xA4D1C46DL, 0xD3D6F4FBL,
    0x4369E96AL, 0x346ED9FCL, 0xAD678846L, 0xDA60B8D0L,
    0x44042D73L, 0x33031DE5L, 0xAA0A4C5FL, 0xDD0D7CC9L,
    0x5005713CL, 0x270241AAL, 0xBE0B1010L, 0xC90C2086L,
    0x5768B525L, 0x206F85B3L, 0xB966D409L, 0xCE61E49FL,
    0x5EDEF90EL, 0x29D9C998L, 0xB0D09822L, 0xC7D7A8B4L,
    0x59B33D17L, 0x2EB40D81L, 0xB7BD5C3BL, 0xC0BA6CADL,
    0xEDB88320L, 0x9ABFB3B6L, 0x03B6E20CL, 0x74B1D29AL,
    0xEAD54739L, 0x9DD277AFL, 0x04DB2615L, 0x73DC1683L,
    0xE3630B12L, 0x94643B84L, 0x0D6D6A3EL, 0x7A6A5AA8L,
    0xE40ECF0BL, 0x9309FF9DL, 0x0A00AE27L, 0x7D079EB1L,
    0xF00F9344L, 0x8708A3D2L, 0x1E01F268L, 0x6906C2FEL,
    0xF762575DL, 0x806567CBL, 0x196C3671L, 0x6E6B06E7L,
    0xFED41B76L, 0x89D32BE0L, 0x10DA7A5AL, 0x67DD4ACCL,
    0xF9B9DF6FL, 0x8EBEEFF9L, 0x17B7BE43L, 0x60B08ED5L,
    0xD6D6A3E8L, 0xA1D1937EL, 0x38D8C2C4L, 0x4FDFF252L,
    0xD1BB67F1L, 0xA6BC5767L, 0x3FB506DDL, 0x48B2364BL,
    0xD80D2BDAL, 0xAF0A1B4CL, 0x36034AF6L, 0x41047A60L,
    0xDF60EFC3L, 0xA867DF55L, 0x316E8EEFL, 0x4669BE79L,
    0xCB61B38CL, 0xBC66831AL, 0x256FD2A0L, 0x5268E236L,
    0xCC0C7795L, 0xBB0B4703L, 0x220216B9L, 0x5505262FL,
    0xC5BA3BBEL, 0xB2BD0B28L, 0x2BB45A92L, 0x5CB36A04L,
    0xC2D7FFA7L, 0xB5D0CF31L, 0x2CD99E8BL, 0x5BDEAE1DL,
    0x9B64C2B0L, 0xEC63F226L, 0x756AA39CL, 0x026D930AL,
    0x9C0906A9L, 0xEB0E363FL, 0x72076785L, 0x05005713L,
    0x95BF4A82L, 0xE2B87A14L, 0x7BB12BAEL, 0x0CB61B38L,
    0x92D28E9BL, 0xE5D5BE0DL, 0x7CDCEFB7L, 0x0BDBDF21L,
    0x86D3D2D4L, 0xF1D4E242L, 0x68DDB3F8L, 0x1FDA836EL,
    0x81BE16CDL, 0xF6B9265BL, 0x6FB077E1L, 0x18B74777L,
    0x88085AE6L, 0xFF0F6A70L, 0x66063BCAL, 0x11010B5CL,
    0x8F659EFFL, 0xF862AE69L, 0x616BFFD3L, 0x166CCF45L,
    0xA00AE278L, 0xD70DD2EEL, 0x4E048354L, 0x3903B3C2L,
    0xA7672661L, 0xD06016F7L, 0x4969474DL, 0x3E6E77DBL,
    0xAED16A4AL, 0xD9D65ADCL, 0x40DF0B66L, 0x37D83BF0L,
    0xA9BCAE53L, 0xDEBB9EC5L, 0x47B2CF7FL, 0x30B5FFE9L,
    0xBDBDF21CL, 0xCABAC28AL, 0x53B39330L, 0x24B4A3A6L,
    0xBAD03605L, 0xCDD70693L, 0x54DE5729L, 0x23D967BFL,
    0xB3667A2EL, 0xC4614AB8L, 0x5D681B02L, 0x2A6F2B94L,
    0xB40BBE37L, 0xC30C8EA1L, 0x5A05DF1BL, 0x2D02EF8DL
};


/*
**********************************************************************************
*
* 函数名称: Calculate_CRC32
*
* 函数描述: 校验码计算
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 校验码计算结果
*
**********************************************************************************
*/
unsigned long Calculate_CRC32(unsigned long CRC32, unsigned char *buff, unsigned long Len)
{
    CRC32 = ~CRC32;
    while(Len--)
    {
        CRC32 = CRC32Table[(CRC32 ^ *buff++) & 0xFFL] ^ (CRC32 >> 8);
    }
    return (CRC32 ^ (~0L));
}

/*
**********************************************************************************
*
* 函数名称: CmpCipher
*
* 函数描述: 加密密码判断
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 与原始密码相等返回0，不相等返回 -1
*
**********************************************************************************
*/
char CmpCipher(void)
{
	unsigned char buf[20], i;
	const unsigned char * pdata;
	unsigned long crc, bootcrc;
	int (*getbootcrc)(void);

	// 读入设备唯一标识符共12个字节
	pdata = (const unsigned char *)(0x1FFFF7E8);
	for(i = 0; i < 12; i++)
	{
		buf[i] = *pdata++;
	}
	//log_print_fmt(SW_COM1, "udid:%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\r\n", 
	//buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8],buf[9],buf[10],buf[11]);
	
	// 读取软件版本号
	pdata = (const unsigned char *)(0x08000000 + 89 * 4);
	for(i = 0; i < 4; i++)
	{
		buf[12 + i] = *pdata++;
	}
	//log_print_fmt(SW_COM1, "vertion:%02X%02X%02X%02X\r\n", buf[12],buf[13],buf[14],buf[15]);
	
	// 计算校验码
	crc = Calculate_CRC32(0x77616C74, buf, 16);
	getbootcrc = (int (*)(void))(*((const unsigned int*)(0x08000000 + 90 * 4)));
	bootcrc = (*getbootcrc)();
	//log_print_fmt(SW_COM1, "crc:%08X, bootcrc:%08X\r\n", crc, bootcrc);
	if(crc != bootcrc)
	{
		u32 *p;
		p = (u32 *)(0x08004000 + 89 * 4);
		ShellPrintf(SHELLCOM, "\r\noooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\r\n");
		ShellPrintf(SHELLCOM, "o SoftWare Version : V%02x.%02x.%02x.%02x\r\n", (u8)((*p) >> 24), (u8)((*p) >> 16), (u8)((*p) >> 8), (u8)((*p) >> 0));
		ShellPrintf(SHELLCOM, "o DeviceType:        E%d\r\n", *(u32 *)(0x08004000+91*4));
		ShellPrintf(SHELLCOM, "o BuidTime:          %s %s\r\n", __DATE__, __TIME__);
		ShellPrintf(SHELLCOM, "o Note: \r\n");
		ShellPrintf(SHELLCOM, "o     Application don't be load, because of crc check error.\r\n");
		ShellPrintf(SHELLCOM, "o If you want to know more, please contact with Unistrong.\r\n");
		ShellPrintf(SHELLCOM, "o Contact method can obtain in the www.Unistrong.com\r\n");
		ShellPrintf(SHELLCOM, "oooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\r\n");
		
		SysSoftRst();
		return 1;
	}
	else
	{
		return 0;
	}
}


