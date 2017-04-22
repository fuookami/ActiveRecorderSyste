#include "SystemControler.h"

FARSys::FARSys(void)
{
	init();
}

FARSys::FARSys(const FARSys &)
{
	init();
}

FARSys::~FARSys(void)
{
	
}

FARSys *FARSys::getPointer(void)
{
	return singleTon<FARSys>::instance();
}

void FARSys::start()
{
	capThr = new FCapThread();
	connect(capThr, SIGNAL(updateSize(unsigned int, unsigned int)), this, SLOT(getSize(unsigned int, unsigned int)));
}

void FARSys::init(void)
{
	capThr = nullptr;
	monitor = nullptr;
}

void FARSys::run(void)
{
	connect(capThr, SIGNAL(buffRefresh(byte *)), this, SLOT(refresh(byte *)));
	capThr->run();
}

void FARSys::getSize(unsigned int _wid, unsigned int _hei)
{
	disconnect(capThr);
	wid = _wid;
	hei = _hei;

	monitor = new FCapMonitor(wid, hei);
	monitor->show();

	run();
}

void FARSys::refresh(byte *rgbBuff)
{
	monitor->setBuff(rgbBuff);
}