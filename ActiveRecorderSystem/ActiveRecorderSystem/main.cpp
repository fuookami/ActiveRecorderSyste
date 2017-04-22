#include "Basic.h"
#include "MessageHandle.h"
#include "SystemControler.h"
#include <QtWidgets/QApplication>
#include <QtCore/QTextCodec>

int main(int argc, char *argv[])
{
	HRESULT hr(S_OK);

	hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		handleErrorHR("Can not initialize the COM runtiome.", hr);
		return 0;
	}

	// Initialize the Media Foundation platform.
	hr = MFStartup(MF_VERSION);
	if (FAILED(hr))
	{
		handleErrorHR("Can not initialize the Media Foundation platform.", hr);
		return 0;
	}

	QTextCodec *codec = QTextCodec::codecForName("utf8");
	QTextCodec::setCodecForLocale(codec);
	QApplication a(argc, argv);
	FARSys *sys = FARSys::getPointer();
	sys->start();
	return a.exec();
}