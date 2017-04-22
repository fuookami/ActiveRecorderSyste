#pragma once

#include "Basic.h"
#include <QtGui/QImage>
#include <RGB24ShowLabel.h>
#include <QtWidgets/QMainWindow>

class FCapMonitor :public QMainWindow
{
	Q_OBJECT;

public:
	FCapMonitor(unsigned int _wid, unsigned int _hei, QWidget *parent = nullptr);
	~FCapMonitor(void);
	void setBuff(byte *buff);

private:
	unsigned int wid;
	unsigned int hei;
	RGB24ShowLabel *l;
	int count;
	QImage *qImg;
};