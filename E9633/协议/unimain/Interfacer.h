#pragma once

class Interfacer
{
public:

	/*
	* FullName:	终端登录
	* @return:  0正确，1消息格式错误，2校验错误，3不存在的命令，4其它不确定错误
	*/
	static int Login();

	/*
	* FullName:	终端登出
	* @return:  true：成功  false：失败
	*/
	static bool Logout();

	/*
	* FullName:	GPS定位信息报告
	* @return:  true：成功  false：失败
	*/
	static bool LocationReport();

	/*
	* FullName:	点火低压提醒
	* @return:  true：成功  false：失败
	*/
	static bool IgnitionLowVoltageReminding();

	/*
	* FullName:	点火低压提醒
	* @return:  true：成功  false：失败
	*/
	static bool ColdCarStartReminding();

	/*
	* FullName:	低压报警
	* @return:  true：成功  false：失败
	*/
	static bool LowVoltageAlarm();

	/*
	* FullName:	超速报警
	* @return:  true：成功  false：失败
	*/
	static bool OverSpeedAlarm();

	/*
	* FullName:	RPM报警
	* @return:  true：成功  false：失败
	*/
	static bool RPM_Alarm();

	/*
	* FullName:	ETC低温报警
	* @return:  true：成功  false：失败
	*/
	static bool ETC_LowTempAlarm();

	/*
	* FullName:	ETC高温报警
	* @return:  true：成功  false：失败
	*/
	static bool ETC_HighTempAlarm();

	/*
	* FullName:	碰撞报警
	* @return:  true：成功  false：失败
	*/
	static bool CollideAlarm();

	/*
	* FullName:	历史故障码报警
	* @return:  true：成功  false：失败
	*/
	static bool HistoryFaultCodeAlarm();

	/*
	* FullName:	当前故障码报警
	* @return:  true：成功  false：失败
	*/
	static bool CurrentFaultCodeAlarm();

	/*
	* FullName:	插拔报警
	* @return:  true：成功  false：失败
	*/
	static bool PlugAlarm();

	/*
	* FullName:	急加速报警
	* @return:  true：成功  false：失败
	*/
	static bool RapidAccelerationAlarm();

	/*
	* FullName:	急减速报警
	* @return:  true：成功  false：失败
	*/
	static bool RapidDecelerationAlarm();

	/*
	* FullName:	急转弯报警
	* @return:  true：成功  false：失败
	*/
	static bool SharpTurnAlarm();

	/*
	* FullName:	转速失调报警
	* @return:  true：成功  false：失败
	*/
	static bool RotationRateDisorderAlarm();

	/*
	* FullName:	超怠速报警
	* @return:  true：成功  false：失败
	*/
	static bool OverIdlingAlarm();

	/*
	* FullName:	滑行报警
	* @return:  true：成功  false：失败
	*/
	static bool SlideAlarm();
};
