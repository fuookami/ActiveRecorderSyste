#include "FCapSrcSelectWid.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>

FCapSrcSelector::FCapSrcSelector(const std::vector<DeviceName> &deviceList, QWidget *parent /* = nullptr */)
	:QMainWindow(parent)
{
	ui = new Ui::FCapSrcSelectWid();
	ui->setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint);
	setAutoFillBackground(true);

	int winx = QApplication::desktop()->screenGeometry().width() / 2 - wid / 2;
	int winy = QApplication::desktop()->screenGeometry().height() / 2 - hei / 2;
	setGeometry(winx, winy, wid, hei);

	ui->ComfirmBtn->setEnabled(false);
	putOnComboBox(deviceList);

	connect(ui->ComfirmBtn, SIGNAL(clicked()), this, SLOT(comfirmHandle()));
}

FCapSrcSelector::~FCapSrcSelector(void)
{
	ui->SrcNames->clear();
	disconnect(ui->ComfirmBtn);
	disconnect(ui->SrcNames);
	delete ui;
}

void FCapSrcSelector::comboChangeHandle(const int index)
{
	if (index != -1)
	{
		ui->ComfirmBtn->setEnabled(true);
	}
}

void FCapSrcSelector::comfirmHandle(void)
{
	disconnect(ui->ComfirmBtn);
	disconnect(ui->SrcNames);
	ui->ComfirmBtn->setEnabled(false);
	emit comfirm(ui->SrcNames->currentIndex());
}

void FCapSrcSelector::putOnComboBox(const std::vector<DeviceName> &deviceList)
{
	for (const DeviceName &device : deviceList)
	{
		char displayFriendlyName[maxFriendlyNameLen * 2];
		WideCharToMultiByte(CP_ACP, 0, device.FriendlyName, -1,
			displayFriendlyName, maxFriendlyNameLen * 2, "", nullptr);
		ui->SrcNames->addItem(QString::fromLocal8Bit(displayFriendlyName));
	}
	ui->SrcNames->setCurrentIndex(-1);

	connect(ui->SrcNames, SIGNAL(currentIndexChanged(int)), this, SLOT(comboChangeHandle(int)));
}