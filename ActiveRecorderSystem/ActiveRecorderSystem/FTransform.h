#pragma once

#include "Basic.h"
#include "FTransformFunctions.h"
#include <vector>
#include <memory>

/************************************************************************/
/*  File:		FTransform.h
	Writer:		Fuookami
	Version:	v1.0
	Date:		2016-3-25
	Needed:		Windows SDK 7.0 or Windows KIT 8.0 ~ 10.0
	Description:
			Declare the port for getting the transform function to transform 
		frame buffer to RGB24 buffer.

*/
/************************************************************************/

//Port for get transform function to RGB24
class FTransform
{
	friend class singleTon<FTransform>;

public:
	~FTransform(void);
	static FTransform *getPointer(void);
	bool hasTramsFun(const GUID &subtype) const;
	pTransFun_t getFunBySubGuid(const GUID &subtype);
	std::vector<GUID> listAvailableSubtype(void) const;

private:
	FTransform(void);
	FTransform(const FTransform &);

private:
	const std::vector<FTransFun> funCheckTable =
	{
		FTransFun(MFVideoFormat_YUY2, FTransformFunctions::YUV_16Bit::YUY2_RGB)
	};
};