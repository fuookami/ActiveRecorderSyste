#pragma once

/************************************************************************/
/*  File:		FVideoFormat.h
	Writer:		Fuookami
	Version:	v1.0
	Date:		2016-3-25
	Needed:		Windows SDK 7.0 or Windows KIT 8.0 ~ 10.0
	Description:
			It declare a port with some information to distinguish the 
		video subtype format GUID.

*/
/************************************************************************/

#include "Basic.h"
#include <array>
#include <string>

//port for checking the video subtype format
namespace FVideoFormat
{
	std::string subtypeGuidToString(const GUID &subtype);
	int subtypeIsEncoded(const GUID &subtype);
}

namespace FVideoFormatCheckTable
{
	using mtvf = struct mtvfstr
	{
		mtvfstr(const GUID &_guid, const char *_name) :guid(_guid), name(_name) {};
		GUID guid;
		std::string name;
	};

	static enum
	{
		UnknownSubtypeGUID = -1,
		Uncompressed = 0,
		Compressed = 1
	};

	static const std::array<mtvf, 29> mtvfms =
	{
		//Uncompressed RGB Formats
		mtvf(MFVideoFormat_RGB8,"RGB8"),
		mtvf(MFVideoFormat_RGB555,"RBG555"),
		mtvf(MFVideoFormat_RGB565,"RGB565"),
		mtvf(MFVideoFormat_RGB24,"RGB24"),
		mtvf(MFVideoFormat_RGB32,"RBG32"),
		mtvf(MFVideoFormat_ARGB32,"ARGB32"),

		//YUV Formats: 8-Bit and Palettized
		mtvf(MFVideoFormat_AI44,"AI44"),
		mtvf(MFVideoFormat_AYUV,"AYUV"),
		mtvf(MFVideoFormat_I420,"I420"),
		mtvf(MFVideoFormat_IYUV,"IYUV"),
		mtvf(MFVideoFormat_NV11,"NV11"),
		mtvf(MFVideoFormat_NV12,"NV12"),
		mtvf(MFVideoFormat_UYVY,"UYVY"),
		mtvf(MFVideoFormat_Y41P,"Y41P"),
		mtvf(MFVideoFormat_Y41T,"Y41T"),
		mtvf(MFVideoFormat_Y42T,"Y42T"),
		mtvf(MFVideoFormat_YUY2,"YUY2"),
		mtvf(MFVideoFormat_YV12,"YV12"),

		//YUV Formats: 10-Bit and 16-Bit
		mtvf(MFVideoFormat_P010,"P010"),
		mtvf(MFVideoFormat_P016,"P016"),
		mtvf(MFVideoFormat_P210,"P210"),
		mtvf(MFVideoFormat_P216,"P216"),
		mtvf(MFVideoFormat_v210,"V210"),
		mtvf(MFVideoFormat_v216,"V216"),
		mtvf(MFVideoFormat_v410,"V410"),
		mtvf(MFVideoFormat_Y210,"Y210"),
		mtvf(MFVideoFormat_Y216,"Y216"),
		mtvf(MFVideoFormat_Y410,"Y410"),
		mtvf(MFVideoFormat_Y416,"Y416")
	};

	static const std::array<mtvf, 26> mtvfms_encoded =
	{
		mtvf(MFVideoFormat_DV25,"DV25"),
		mtvf(MFVideoFormat_DV50,"DV50"),
		mtvf(MFVideoFormat_DVC,"DVC"),
		mtvf(MFVideoFormat_DVH1,"DVH1"),
		mtvf(MFVideoFormat_DVHD,"DVHD"),
		mtvf(MFVideoFormat_DVSD,"DVSD"),
		mtvf(MFVideoFormat_DVSL,"DVSL"),
		mtvf(MFVideoFormat_H263,"H263"),
		mtvf(MFVideoFormat_H264,"H264"),
		mtvf(MFVideoFormat_H264_ES,"H264_ES"),
		mtvf(MFVideoFormat_HEVC,"HEVC"),
		mtvf(MFVideoFormat_HEVC_ES,"HEVC_ES"),
		mtvf(MFVideoFormat_M4S2,"M4S2"),
		mtvf(MFVideoFormat_MJPG,"MJPG"),
		mtvf(MFVideoFormat_MP43,"MP43"),
		mtvf(MFVideoFormat_MP4S,"MP4S"),
		mtvf(MFVideoFormat_MP4V,"MP4V"),
		mtvf(MFVideoFormat_MPEG2,"MPEG2"),
		mtvf(MFVideoFormat_MPG1,"MPG1"),
		mtvf(MFVideoFormat_MSS1,"MSS1"),
		mtvf(MFVideoFormat_MSS2,"MSS2"),
		mtvf(MFVideoFormat_WMV1,"WMV1"),
		mtvf(MFVideoFormat_WMV2,"WMV2"),
		mtvf(MFVideoFormat_WMV3,"WMV3"),
		mtvf(MFVideoFormat_WVC1,"WVC1"),
		mtvf(MFVideoFormat_420O,"420O")
	};
}