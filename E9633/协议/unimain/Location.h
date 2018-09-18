#pragma once

class Location
{
public:
	Location()
	{
		Reset();
	}

	void Reset()
	{
		time		= 0;
		latitude	= 0;
		longitude	= 0;
		altitude	= 0;
		speed		= 0;
		hasAzimuth	= false;
		azimuth		= 0;
		quality		= 0;
		HDOP		= 0;
		satellite	= 0;
	}

public:
	long		time;		// 本定位数据的utc时间
	double		latitude;	// 纬度（单位：度，精确到0.000001，北纬为+，南纬为-）
	double		longitude;	// 经度（单位：度，精确到0.000001，东经为+，西经为-）
	double		altitude;	// 海拔高度（单位：米，-9999.9~99999.9，精确到0.1）
	double		speed;		// 相对于地面的速度（单位：公里每小时）
	bool		hasAzimuth;	// 方位角是否有效
	double		azimuth;	// 方位角（单位：度，相对于正北顺时针角度，0.0~359.9）
	int			quality;	// 定位状态 0-未定位,1-二维定位,2-三维定位,3-Last定位
	double		HDOP;		// HDOP
	int         satellite;	// 卫星个数
};
