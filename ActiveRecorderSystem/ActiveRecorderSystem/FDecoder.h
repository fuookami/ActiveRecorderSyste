#pragma once

#include "Basic.h"
#include <memory>
#include <vector>

/************************************************************************/
/*  File:		FDecoder.h
	Writer:		Fuookami
	Version:	v1.0
	Date:		2016-3-25
	Needed:		Windows SDK 7.0 or Windows KIT 8.0 ~ 10.0
	Description:
			Now it is no avail and I do not have enough capability to 
		complete it.
			Declare the port for getting the decoder in Microsoft Media 
		Foundation to decode the compressed video stream such as H264.

*/
/************************************************************************/

//port for get decoder
class FDecoder
{
	friend class singleTon<FDecoder>;

public:
	~FDecoder(void);
	static FDecoder *getPointer(void);
	bool hasDecoder(const GUID &subGUID) const;
	result setDecoder(const GUID &inputSubtype, const GUID &outputSubtype, unsigned int frameRate, 
		unsigned int frameWidth, unsigned int frameHeight);
	IMFTransform *getDecoder(void);
	std::vector<GUID> listAvailableInputSubtype(void) const;
	std::vector<GUID> listAvailableOutputSubtype(const GUID &inputSubtype);

private:
	FDecoder(void);
	FDecoder(const FDecoder &);

private:
	spMFType<IMFTransform> m_Decoder;
};

//port for checking decoders' information
namespace FDecoderCheck
{
	using decoderLine = struct decoderLineStr
	{
		decoderLineStr(const CLSID &_rclsid, const GUID &_inputSubtype) :rclsid(_rclsid), inputSubtype(_inputSubtype) {};
		CLSID rclsid;
		GUID inputSubtype;
	};

	static const std::vector<decoderLine> decoderCheckTable =
	{
		decoderLine(CLSID_CMSH264DecoderMFT, MFVideoFormat_H264)
	};

	static enum
	{
		OK,
		UnsupportedInputSub,
		UnsucceedCreate,
		UnsupportedOutputSub
	};
}