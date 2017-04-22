#pragma once

/************************************************************************/
/*  File:		FCapture.h
	Writer:		Fuookami
	Version:	v1.0
	Date:		2016-3-25
	Needed:		Windows SDK 7.0 or Windows KIT 8.0 ~ 10.0
	Description:
			Main class of the lower layer system. It will intelligentized
		create a controler to control the capture device source and the source
		reader. User just need to control its progress. After creating, user 
		can get the frame buffer through member function "getBuffer()" easily.

*/
/************************************************************************/

#include "Basic.h"
#include "FCapSrc.h"
#include "FCapSrcSelectWid.h"
#include "FDecoder.h"
#include "FTransform.h"
#include <QtWidgets/QMainWindow>

class FCapture : public QObject
{
	Q_OBJECT;

	friend class singleTon<FCapture>;
public:
	~FCapture(void);
	static FCapture *getPointer(void);
	void initCapSrc(void);
	bool autoSetMediaType(void);
	void getSize(unsigned int &_wid, unsigned int &_hei);
	void waitBuffRefresh(void);

signals:
	void capSrcSucceed(void);
	void buffRefresh(byte *currRGBBuff);

private slots :
	void capSrcComfirm(const int index);

private:
	FCapture(void);
	FCapture(const FCapture &);
	void init(void);

private:
	bool listSrcMediaTypes(spMFType<IMFMediaTypeHandler> &pHandler, std::vector<spMFType<IMFMediaType>> &pmts);
	int autoSelectMeiaTypes(std::vector<spMFType<IMFMediaType>> &pmts);
	bool addDecoder(void);
	void setFrameInfo(void);
	void tryGetSampleToSetBit(void);

private:
	IMFMediaSource *pSrc;
	spMFType<IMFMediaType> pInMT, pOutMT;
	IMFTransform *pDecoder;
	spMFType<IMFSourceReader> pSrcReader;

private:
	FCapSrc *pFCapSrc;
	FCapSrcSelector *pFCapSrcSelectWid;
	FDecoder *decoder;
	FTransform *transform;
	pTransFun_t transFun;

private:
	bool readyForGet;
	long long lastFrameTime;
	unsigned int bit;

private:
	unsigned int frameHeight;
	unsigned int frameWidth;
	unsigned int fps;
	std::shared_ptr<byte> rgbBuff;
};