#include "MessageHandle.h"

void handleErrorHR(const char * str, HRESULT hr)
{
	std::ostringstream sout;
	sout << str << "\nhr = 0x" << std::hex << hr << std::endl;
	std::string sstr = sout.str();
	QMessageBox::information(nullptr, QString::fromLocal8Bit("程序遇到了某些错误"), QString::fromLocal8Bit(sstr.c_str()));
}

void handleErrorHR(const std::string & str, HRESULT hr)
{
	handleErrorHR(str.c_str(), hr);
}

void handleError(const char *str)
{
	QMessageBox::information(nullptr, QString::fromLocal8Bit("程序遇到了某些错误"), QString::fromLocal8Bit(str));
}

void handleError(const std::string &str)
{
	QMessageBox::information(nullptr, QString::fromLocal8Bit("程序遇到了某些错误"), QString::fromLocal8Bit(str.c_str()));
}

void handleProcess(const char *str)
{
	QMessageBox::information(nullptr, QString::fromLocal8Bit("程序正常地运行中"), QString::fromLocal8Bit(str));
}

void handleProcess(const std::string &str)
{
	QMessageBox::information(nullptr, QString::fromLocal8Bit("程序正常地运行中"), QString::fromLocal8Bit(str.c_str()));
}