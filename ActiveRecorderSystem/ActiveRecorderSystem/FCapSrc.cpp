#include "FCapSrc.h"
#include "MessageHandle.h"

FCapSrc::FCapSrc(void)
{
	ppDevices = nullptr;
	hasList = false;
}

FCapSrc::FCapSrc(const FCapSrc &)
{
	ppDevices = nullptr;
	hasList = false;
}

FCapSrc::~FCapSrc(void)
{
	releaseSource();
}

FCapSrc *FCapSrc::getPointer(void)
{
	return singleTon<FCapSrc>::instance();
}

IMFMediaSource *FCapSrc::getCapSrc(void)
{
	return m_capSrc.get();
}

std::vector<DeviceName> &FCapSrc::getDeviceList(void)
{
	if (!hasList)
	{
		listDevice();
		hasList = true;
	}
	return deviceList;
}

bool FCapSrc::createCapSrc(int index)
{
	if (m_capSrc.get() != nullptr)
	{
		return true;
	}
	if (FAILED(ppDevices[index]->ActivateObject(IID_PPV_ARGS(m_capSrc.getPointer()))))
	{
		releaseSource();
		return false;
	}
	else
	{
		releaseSource();
		return true;
	}
}

void FCapSrc::listDevice(void)
{
	HRESULT hr(S_OK);
	DeviceName name;
	//������������ҳ
	IMFAttributes *pAttributes(nullptr);
	hr = MFCreateAttributes(&pAttributes, 1);
	if (FAILED(hr))
	{
		handleErrorHR(std::string("Can not create attributes."), hr);
		return;
	}

	//���ùؼ���-ö������Ƶ�ɼ��豸
	hr = pAttributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
	if (FAILED(hr))
	{
		safeRelease(&pAttributes);
		handleErrorHR(std::string("Can not set attributes guid."), hr);
		return;
	}

	//��ȡ�������
	UINT32 dwCount(0);
	hr = MFEnumDeviceSources(pAttributes, &ppDevices, &dwCount);
	if (FAILED(hr))
	{
		safeRelease(&pAttributes);
		handleErrorHR(std::string("Can not get devices enum."), hr);
		return;
	}

	if (dwCount == 0)
	{
		safeRelease(&pAttributes);
		safeRelease(ppDevices);
		hr = E_FAIL;
		handleErrorHR(std::string("Can not get any devices."), hr);
		return;
	}

	for (UINT32 i(0); i < dwCount; ++i)
	{
		// ��ȡ�豸�Ѻ���
		ppDevices[i]->GetString(MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, name.FriendlyName, maxFriendlyNameLen, nullptr);
		// ��ȡ�豸Moniker��  
		ppDevices[i]->GetString(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK, name.MonikerName, maxMonikerNameLen, nullptr);
		// �����б�  
		deviceList.push_back(name);
	}
}

void FCapSrc::releaseSource(void)
{
	for (size_t i(0), j(deviceList.size());i < j;++i)
	{
		safeRelease(&(ppDevices[i]));
	}
	safeRelease(ppDevices);
	deviceList.clear();
}