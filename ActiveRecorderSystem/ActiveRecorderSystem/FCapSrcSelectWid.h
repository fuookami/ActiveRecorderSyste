#pragma once

#include "ui_FCapSrcSelector.h"
#include "FCapSrc.h"
#include <vector>
#include <QtWidgets/QMainWindow>

class FCapSrcSelector : public QMainWindow
{
	Q_OBJECT;

public:
	static const int wid = 320;
	static const int hei = 110;

public:
	FCapSrcSelector(const std::vector<DeviceName> &deviceList, QWidget *parent = nullptr);
	~FCapSrcSelector(void);

signals:
	void comfirm(const int index);

private slots:
	void comboChangeHandle(const int index);
	void comfirmHandle(void);

private:
	void putOnComboBox(const std::vector<DeviceName> &deviceList);

private:
	Ui::FCapSrcSelectWid *ui;
};