#pragma once

#include "Basic.h"
#include "FCapture.h"
#include <QThread>

class FCapThread :public QThread
{
	Q_OBJECT;

public:
	FCapThread();
	~FCapThread();

	void run(void);

signals:
	void buffRefresh(byte *rgbBuff);
	void updateSize(unsigned int wid, unsigned int hei);

private slots:
	void comfirmCapSrcSucceed(void);
	void refresh(byte *rgbBuff);

private:
	unsigned int wid;
	unsigned int hei;

private:
	FCapture *fCap;
};