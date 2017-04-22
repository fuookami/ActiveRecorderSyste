#pragma once

/************************************************************************/
/*  File:		FCapSrc.h
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

#include "Basic.h"
#include <vector>
#include <memory>

static const int maxFriendlyNameLen = 128;
static const int maxMonikerNameLen = 256;

using DeviceName = struct
{
	WCHAR FriendlyName[maxFriendlyNameLen];
	WCHAR MonikerName[maxMonikerNameLen];
};

class FCapSrc
{
	friend class singleTon<FCapSrc>;
public:
	~FCapSrc(void);
	static FCapSrc *getPointer(void);
	IMFMediaSource *getCapSrc(void);

	std::vector<DeviceName> &getDeviceList(void);
	bool createCapSrc(int index);

private:
	FCapSrc(void);
	FCapSrc(const FCapSrc &);

private:
	void listDevice(void);
	void releaseSource(void);

private:
	spMFType<IMFMediaSource> m_capSrc;
	bool hasList;
	IMFActivate **ppDevices;
	std::vector<DeviceName> deviceList;
};