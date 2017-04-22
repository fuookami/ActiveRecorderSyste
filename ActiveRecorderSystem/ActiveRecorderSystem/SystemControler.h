#pragma once


#include "FCapThread.h"
#include "FCapMonitor.h"
#include <QtWidgets/QMainWindow>

class FARSys : public QObject
{
	Q_OBJECT;

	friend class singleTon<FARSys>;

public:
	~FARSys();
	static FARSys *getPointer(void);
	void start(void);

private:
	FARSys(void);
	FARSys(const FARSys &);
	void init(void);
	void run(void);

private slots:
	void getSize(unsigned int _wid, unsigned int _hei);
	void refresh(byte *rgbBuff);

private:
	unsigned int wid;
	unsigned int hei;

private:
	FCapThread *capThr;
	FCapMonitor *monitor;
};