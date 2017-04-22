#include "FCapMonitor.h"
#include "FTransformFunctions.h"
#include <QtWidgets/QApplication>
#include <QtGui/QPainter>
#include <QtWidgets/QDesktopWidget>

FCapMonitor::FCapMonitor(unsigned int _wid, unsigned int _hei, QWidget *parent /* = nullptr */)
	: wid(_wid), hei(_hei), count(0), QMainWindow(parent)
{
	l = new RGB24ShowLabel(nullptr);
	l->setMinimumSize(wid, hei);
	l->setMaximumSize(wid, hei);
	l->setGeometry(QApplication::desktop()->screenGeometry().width(), 0, wid, hei);
	l->setWindowFlags(Qt::FramelessWindowHint);
	l->setAutoFillBackground(true);
	l->show();

	qImg = new QImage(wid, hei, QImage::Format_RGB888);
}

FCapMonitor::~FCapMonitor(void)
{
	delete l;
	delete qImg;
}

void FCapMonitor::setBuff(byte *buff)
{
	delete qImg;
	qImg = new QImage(buff, wid, hei, QImage::Format_RGB888);
	l->showImage(qImg);
}