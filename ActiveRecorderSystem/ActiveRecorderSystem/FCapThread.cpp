#include "FCapThread.h"

FCapThread::FCapThread()
{
	fCap = FCapture::getPointer();
	fCap->initCapSrc();
	connect(fCap, SIGNAL(capSrcSucceed()), this, SLOT(comfirmCapSrcSucceed()));
}

FCapThread::~FCapThread()
{
	disconnect(fCap);
}

void FCapThread::run(void)
{
	connect(fCap, SIGNAL(buffRefresh(byte *)), this, SLOT(refresh(byte *)));
	while (1)
	{
		fCap->waitBuffRefresh();
	}
}

void FCapThread::comfirmCapSrcSucceed(void)
{
	disconnect(fCap);
	fCap->autoSetMediaType();
	fCap->getSize(wid, hei);
	emit updateSize(wid, hei);
}

void FCapThread::refresh(byte *rgbBuff)
{
	emit buffRefresh(rgbBuff);
}