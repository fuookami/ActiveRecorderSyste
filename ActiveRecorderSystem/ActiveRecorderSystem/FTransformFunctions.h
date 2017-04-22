#pragma once

#include "Basic.h"
#include <memory>
#include <vector>

/************************************************************************/
/*  File:		FTransformFunctions.h
	Writer:		Fuookami
	Version:	v1.0
	Date:		2016-3-25
	Needed:		Windows SDK 7.0 or Windows KIT 8.0 ~ 10.0
	Description:
			List functions for transforming buffer to RGB24 buffer.

*/
/************************************************************************/

typedef void(*pTransFun_t)(const byte *_src, byte *_dest, unsigned int frameWidth, unsigned int frameHeight, const unsigned int srcBit);

using FTransFun = struct FTransFunStr
{
	FTransFunStr(const GUID &_subtype, pTransFun_t _pFun) : inputBuffSubtype(_subtype), pFun(_pFun) {};
	GUID inputBuffSubtype;
	pTransFun_t pFun;
};

//Functions for transforming to RGB24
namespace FTransformFunctions
{
	static unsigned int RGB24_Bit = 24;

	namespace YUV_16Bit
	{
		void YUY2_RGB(const byte *YUY2buff, byte *RGBbuff, unsigned int frameWidth, unsigned int frameHeight, const unsigned int bit = 16);
	}
}