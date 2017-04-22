#include "RGB24ShowLabel.h"

RGB24ShowLabel::RGB24ShowLabel(QWidget *parent /* = nullptr */)
	:QLabel(parent), img(nullptr)
{
}

RGB24ShowLabel::~RGB24ShowLabel(void)
{
}

void RGB24ShowLabel::showImage(QImage *_img)
{
	img = _img;
	repaint();
}

void RGB24ShowLabel::paintEvent(QPaintEvent * event)
{
	if (img != nullptr)
	{
		QPainter painter(this);
		painter.drawImage(QPoint(0, 0), *img);
	}
}