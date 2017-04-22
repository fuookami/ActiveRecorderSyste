#pragma once

#include "Basic.h"
#include <string>

/************************************************************************/
/*  File:		FMediaType.h
	Writer:		Fuookami
	Version:	v1.0
	Date:		2016-3-25
	Needed:		Windows SDK 7.0 or Windows KIT 8.0 ~ 10.0
	Description:
			It declare a port for easy handling the video media type in 
		Microsoft Media Foundation.

*/
/************************************************************************/

namespace FMediaType
{
	HRESULT createVideoMediaType(IMFMediaType **ppMT, const GUID &subtype,
		const unsigned int frameRate, const unsigned int frameWidth, const unsigned int frameHeight);
	UINT32 getFrameRate(IMFMediaType *pmt);
	std::string getMediaTypeInfo(IMFMediaType *pmt, const int index = -1);
}