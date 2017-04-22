#include "FCapture.h"
#include "FMediaType.h"
#include "FVideoFormat.h"
#include "MessageHandle.h"
#include <cmath>
#include <QtWidgets/QMessageBox>
#include <QLabel>

FCapture::FCapture(void)
{
	init();
}

FCapture::FCapture(const FCapture &)
{
	init();
}

FCapture::~FCapture(void)
{
	closeQTObject(&pFCapSrcSelectWid);
}

FCapture *FCapture::getPointer(void)
{
	return singleTon<FCapture>::instance();
}

void FCapture::initCapSrc(void)
{
	pFCapSrcSelectWid = new FCapSrcSelector(pFCapSrc->getDeviceList());
	pFCapSrcSelectWid->show();
	connect(pFCapSrcSelectWid, SIGNAL(comfirm(const int)), this, SLOT(capSrcComfirm(const int)));
}

void FCapture::capSrcComfirm(const int index)
{
	if (!pFCapSrc->createCapSrc(index))
	{
		handleError("can not create the capture source");
		closeQTObject(&pFCapSrcSelectWid);
		delete this;
	}
	else
	{
		pSrc = pFCapSrc->getCapSrc();
		closeQTObject(&pFCapSrcSelectWid);
		handleProcess("succeed to create a capture source");
		emit capSrcSucceed();
	}
}

void FCapture::init(void)
{
	pFCapSrc = FCapSrc::getPointer();
	pFCapSrcSelectWid = nullptr;
	pSrc = nullptr;
	pDecoder = nullptr;
	decoder = nullptr;
	frameHeight = 0;
	frameWidth = 0;
	fps = 0;
	transFun = nullptr;
	lastFrameTime = 0;
	readyForGet = false;
}

bool FCapture::autoSetMediaType(void)
{
	spMFType<IMFMediaTypeHandler> pHandler;
	std::vector<spMFType<IMFMediaType>> pmts;
	HRESULT hr(S_OK);

	transform = FTransform::getPointer();
	decoder = FDecoder::getPointer();

	if (!listSrcMediaTypes(pHandler, pmts))
	{
		return false;
	}
	int or (autoSelectMeiaTypes(pmts));
	if (or == -1)
	{
		handleError("Can not select any media type to suit the system.");
		return false;
	}

	MFCreateMediaType(pInMT.getPointer());
	pmts[or].get()->CopyAllItems(pInMT.get());

	unsigned int tempWid, tempHei;
	MFGetAttributeSize(pInMT.get(), MF_MT_FRAME_SIZE, &tempWid, &tempHei);
	MFSetAttributeSize(pInMT.get(), MF_MT_FRAME_SIZE, 1920, 1080);

	hr = pHandler.get()->SetCurrentMediaType(pInMT.get());
	if (FAILED(hr))
	{
		MFSetAttributeSize(pInMT.get(), MF_MT_FRAME_SIZE, tempWid, tempHei);
		hr = pHandler.get()->SetCurrentMediaType(pInMT.get());
	}
	if (FAILED(hr))
	{
		handleErrorHR("Can not set the capture media type", hr);
		return false;
	}
	else
	{
		GUID inSub;
		pInMT.get()->GetGUID(MF_MT_SUBTYPE, &inSub);
		handleProcess(std::string("The system select and succeed set ") + FVideoFormat::subtypeGuidToString(inSub) + 
			std::string(" as the input media type of the capture."));
	}

	hr = MFCreateSourceReaderFromMediaSource(pSrc, nullptr, pSrcReader.getPointer());
	if (FAILED(hr))
	{
		handleErrorHR("Can not create a source reader from the capture", hr);
		return false;
	}

	GUID intputSubType;
	pInMT.get()->GetGUID(MF_MT_SUBTYPE, &intputSubType);
	result res(FVideoFormat::subtypeIsEncoded(intputSubType));
	if (res == FVideoFormatCheckTable::Compressed)
	{
		if (!addDecoder())
		{
			return false;
		}
	}
	else if(res == FVideoFormatCheckTable::UnknownSubtypeGUID)
	{
		handleError("This is an unknown subtype");
		return false;
	}
	else
	{
		MFCreateMediaType(pOutMT.getPointer());
		pInMT.get()->CopyAllItems(pOutMT.get());
	}

	GUID outputSubType;
	pOutMT.get()->GetGUID(MF_MT_SUBTYPE, &outputSubType);
	if (outputSubType == MFVideoFormat_RGB24)
	{
		setFrameInfo();
		return true;
	}
	else
	{
		if (!transform->hasTramsFun(outputSubType))
		{
			handleError(std::string("No trans function for ") + FVideoFormat::subtypeGuidToString(outputSubType));
			return false;
		}
		transFun = transform->getFunBySubGuid(outputSubType);
		setFrameInfo();
		return true;
	}
}

void FCapture::getSize(unsigned int & _wid, unsigned int & _hei)
{
	_wid = frameWidth;
	_hei = frameHeight;
}

bool FCapture::listSrcMediaTypes(spMFType<IMFMediaTypeHandler> &pHandler, std::vector<spMFType<IMFMediaType>> &pmts)
{
	HRESULT hr(S_OK);
	IMFPresentationDescriptor *pPD(nullptr);
	IMFStreamDescriptor *pSD(nullptr);

	hr = pSrc->CreatePresentationDescriptor(&pPD);
	if (FAILED(hr))
	{
		handleErrorHR("Can not get capture presentation desriptor.", hr);
		return false;
	}

	BOOL fSelected;
	hr = pPD->GetStreamDescriptorByIndex(0, &fSelected, &pSD);
	if (FAILED(hr))
	{
		handleErrorHR("Can not get capture stream desriptor.", hr);
		safeRelease(&pPD);
		return false;
	}

	hr = pSD->GetMediaTypeHandler(pHandler.getPointer());
	if (FAILED(hr))
	{
		handleErrorHR("Can not get capture media type handler.", hr);
		safeRelease(&pPD);
		safeRelease(&pSD);
		return false;
	}

	unsigned long mtNum(0);
	hr = pHandler.get()->GetMediaTypeCount(&mtNum);
	if (FAILED(hr))
	{
		handleErrorHR("Can not get the number of capture media types.", hr);
		safeRelease(&pPD);
		safeRelease(&pSD);
		safeRelease(pHandler.getPointer());
		return false;
	}
	else if (mtNum == 0)
	{
		handleErrorHR("Can not get any capture media types.", hr);
		safeRelease(&pPD);
		safeRelease(&pSD);
		safeRelease(pHandler.getPointer());
		return false;
	}

	for (unsigned long i(0); i < mtNum; ++i)
	{
		spMFType<IMFMediaType> pMT;
		hr = pHandler.get()->GetMediaTypeByIndex(i, pMT.getPointer());
		if (FAILED(hr))
		{
			handleErrorHR(std::string("Can not get capture media type No.") + std::to_string(i), hr);
			continue;
		}

		pmts.push_back(pMT);
	}

	safeRelease(&pPD);
	safeRelease(&pSD);
	return true;
}

int FCapture::autoSelectMeiaTypes(std::vector<spMFType<IMFMediaType>> &pmts)
{
	unsigned int maxFps(0);

	std::vector<int> aa, ab;
	for (size_t i(0), j(pmts.size()); i < j; ++i)
	{
		IMFMediaType *pmt = pmts[i].get();
		GUID majorType;
		pmt->GetGUID(MF_MT_MAJOR_TYPE, &majorType);
		if (majorType == MFMediaType_Video)
		{
			aa.push_back(i);
			unsigned int currFps(FMediaType::getFrameRate(pmt));
			maxFps = (currFps > maxFps) ? currFps : maxFps;
		}
	}

	unsigned int maxSize(0), maxHei(0), maxWid(0);
	for (size_t i(0), j(aa.size()); i < j; ++i)
	{
		IMFMediaType *pmt = pmts[aa[i]].get();
		unsigned int currFps(FMediaType::getFrameRate(pmt));
		if (currFps == maxFps)
		{
			unsigned int currHei, currWid;
			MFGetAttributeSize(pmt, MF_MT_FRAME_SIZE, &currWid, &currHei);
			if ((currHei * 16) == (currWid * 9))
			{
				ab.push_back(aa[i]);
				if (currHei*currWid > maxSize)
				{
					maxHei = currHei;
					maxWid = currHei;
					maxSize = currHei*currWid;
				}
			}
		}
	}
	aa.clear();

	std::vector<GUID> canTransSubType(transform->listAvailableSubtype());
	for (size_t i(0), j(ab.size()); i < j; ++i)
	{
		IMFMediaType *pmt = pmts[ab[i]].get();
		GUID subtype;
		pmt->GetGUID(MF_MT_SUBTYPE, &subtype);
		//如果能直接转码
		if (std::find(canTransSubType.cbegin(), canTransSubType.cend(), subtype) != canTransSubType.cend())
		{
			return ab[i];
		}
	}

	std::vector<GUID> canDecodeSubType(decoder->listAvailableInputSubtype());
	for (size_t i(0), j(ab.size()); i < j; ++i)
	{
		IMFMediaType *pmt = pmts[ab[i]].get();
		GUID subtype;
		pmt->GetGUID(MF_MT_SUBTYPE, &subtype);
		//如果能解码
		if (std::find(canDecodeSubType.cbegin(), canDecodeSubType.cend(), subtype) != canTransSubType.cend())
		{
			std::vector<GUID> currDecoderOutputGUID(decoder->listAvailableOutputSubtype(subtype));
			//如果解码器输出能转码
			if (std::find(canTransSubType.cbegin(), canTransSubType.cend(), subtype) != canTransSubType.cend())
			{
				return ab[i];
			}
		}
	}

	return -1;
}

bool FCapture::addDecoder(void)
{
	HRESULT hr(S_OK);
	spMFType<IMFSourceReaderEx> pSrcReaderEx;
	hr = pSrcReader.get()->QueryInterface(IID_IMFSourceReaderEx, (void **)pSrcReaderEx.getPointer());
	if (FAILED(hr))
	{
		handleErrorHR("Can not get the srcreaderEx", hr);
		return false;
	}

	GUID inGUID, outGUID;
	bool hasOut(false);
	pInMT.get()->GetGUID(MF_MT_SUBTYPE, &inGUID);
	std::vector<GUID> canTransGUIDs(transform->listAvailableSubtype());
	std::vector<GUID> decodeOutGUIDs(decoder->listAvailableOutputSubtype(inGUID));
	for (const GUID &canTransGUID : canTransGUIDs)
	{
		if (std::find(decodeOutGUIDs.cbegin(), decodeOutGUIDs.cend(), canTransGUID) != decodeOutGUIDs.cend())
		{
			outGUID = canTransGUID;
			hasOut = true;
		}
	}
	if (!hasOut)
	{
		handleError(std::string("Can not get any output type of the decoder of ") + FVideoFormat::subtypeGuidToString(inGUID));
		return false;
	}

	unsigned int tempFPS(0), tempHei(0), tempWid(0);
	tempFPS = FMediaType::getFrameRate(pInMT.get());
	MFGetAttributeSize(pInMT.get(), MF_MT_FRAME_SIZE, &tempWid, &tempHei);
	switch (decoder->setDecoder(inGUID, outGUID, tempFPS, tempWid, tempHei))
	{
	case FDecoderCheck::UnsupportedInputSub:
		handleError(std::string("Can not set ") + FVideoFormat::subtypeGuidToString(inGUID) + std::string(" as decoder input type."));
		return false;
	case FDecoderCheck::UnsupportedOutputSub:
		handleError(std::string("Can not set ") + FVideoFormat::subtypeGuidToString(outGUID) + std::string(" as decoder output type."));
		return false;
	case FDecoderCheck::UnsucceedCreate:
		handleError(std::string("Can not create \'") + FVideoFormat::subtypeGuidToString(inGUID) + std::string("\' to \'")
			+ FVideoFormat::subtypeGuidToString(outGUID) + std::string("\' decoder."));
		return false;
	default:
		break;
	}

	pDecoder = decoder->getDecoder();
	hr = pSrcReaderEx.get()->AddTransformForStream(MF_SOURCE_READER_FIRST_VIDEO_STREAM, pDecoder);
	if (FAILED(hr))
	{
		switch (hr)
		{
		case MF_E_INVALIDMEDIATYPE:
			handleError(std::string("The transform does not support the current stream format, and no conversion was possible."));
			break;
		case MF_E_INVALIDREQUEST:
			handleError(std::string("Invalid request."));
			break;
		case MF_E_INVALIDSTREAMNUMBER:
			handleError(std::string("The dwStreamIndex parameter is invalid."));
			break;
		default:
			break;
		}
		return false;
	}

	pSrcReaderEx.get()->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, pOutMT.getPointer());
	return true;
}

void FCapture::setFrameInfo(void)
{
	readyForGet = true;
	MFGetAttributeSize(pOutMT.get(), MF_MT_FRAME_SIZE, &frameWidth, &frameHeight);
	fps = FMediaType::getFrameRate(pOutMT.get());
	rgbBuff.reset(new byte[frameWidth * frameHeight * FTransformFunctions::RGB24_Bit / 8]);
	tryGetSampleToSetBit();
}

void FCapture::tryGetSampleToSetBit(void)
{
	IMFSample *pSample(nullptr);
	long long currTimeStamp;
	unsigned long streamIndex, flags;
	while (1)
	{
		pSrcReader.get()->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &streamIndex, &flags, &currTimeStamp, &pSample);

		if (pSample != nullptr)
		{
			IMFMediaBuffer *pBuff(nullptr);
			pSample->GetBufferByIndex(0, &pBuff);
			byte *pYUYVbuff(nullptr);
			DWORD maxLen(0), currLen(0);
			pBuff->Lock(&pYUYVbuff, &maxLen, &currLen);
			bit = currLen / (frameHeight * frameWidth) * 8;
			lastFrameTime = currTimeStamp;
			safeRelease(&pBuff);
			break;
		}
	}
	safeRelease(&pSample);
}

void FCapture::waitBuffRefresh(void)
{
	IMFSample *pSample(nullptr);
	long long currTimeStamp;
	unsigned long streamIndex, flags;
	while (1)
	{
		pSrcReader.get()->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &streamIndex, &flags, &currTimeStamp, &pSample);

		if (pSample != nullptr)
		{
			IMFMediaBuffer *pBuff(nullptr);
			pSample->GetBufferByIndex(0, &pBuff);
			byte *pYUYVbuff(nullptr);
			DWORD maxLen(0), currLen(0);
			pBuff->Lock(&pYUYVbuff, &maxLen, &currLen);
			transFun(pYUYVbuff, rgbBuff.get(), frameWidth, frameHeight, bit);
			lastFrameTime = currTimeStamp;
			safeRelease(&pBuff);
			break;
		}
	}

	safeRelease(&pSample);
	emit buffRefresh(rgbBuff.get());
}