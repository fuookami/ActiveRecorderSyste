#pragma once

#include <QtGui/QPainter>
#include <QtWidgets/QLabel>

class RGB24ShowLabel :public QLabel
{
	Q_OBJECT;

public:
	RGB24ShowLabel(QWidget *parent = nullptr);
	~RGB24ShowLabel(void);
	void showImage(QImage *_img);

protected:
	void paintEvent(QPaintEvent *event);

private:
	QImage *img;
};