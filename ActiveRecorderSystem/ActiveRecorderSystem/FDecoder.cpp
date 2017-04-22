#include "FDecoder.h"
#include "FMediaType.h"
#include "MessageHandle.h"
#include <algorithm>

FDecoder::FDecoder(void)
{

}

FDecoder::FDecoder(const FDecoder &)
{

}

FDecoder::~FDecoder(void)
{

}

FDecoder *FDecoder::getPointer(void)
{
	return singleTon<FDecoder>::instance();
}

bool FDecoder::hasDecoder(const GUID &subGUID) const
{
	for (std::vector<FDecoderCheck::decoderLine>::size_type i(0), j(FDecoderCheck::decoderCheckTable.size());i < j;++i)
	{
		if (FDecoderCheck::decoderCheckTable[i].inputSubtype == subGUID)
		{
			return true;
		}
	}
	return false;
}

result FDecoder::setDecoder(const GUID &inputSubtype, const GUID &outputSubtype, unsigned int frameRate,
	unsigned int frameWidth, unsigned int frameHeight)
{
	IMFTransform *pDecoder(nullptr);
	HRESULT hr(S_OK);
	result returnValue(FDecoderCheck::OK);

	IMFMediaType *inputType(nullptr), *outputType(nullptr);

	bool hasDecoder(false);
	CLSID decoderCLSID;

	for (std::vector<FDecoderCheck::decoderLine>::size_type i(0), j(FDecoderCheck::decoderCheckTable.size());i < j;++i)
	{
		if (FDecoderCheck::decoderCheckTable[i].inputSubtype == inputSubtype)
		{
			hasDecoder = true;
			decoderCLSID = FDecoderCheck::decoderCheckTable[i].rclsid;
			break;
		}
	}
	if (!hasDecoder)
	{
		returnValue = FDecoderCheck::UnsupportedInputSub;
		goto end;
	}

	hr = CoCreateInstance(decoderCLSID, nullptr, CLSCTX_INPROC_SERVER, 
		IID_IMFTransform, (void**)&pDecoder);
	if (FAILED(hr))
	{
		returnValue = FDecoderCheck::UnsucceedCreate;
		goto end;
	}

	hr = FMediaType::createVideoMediaType(&inputType, inputSubtype, frameRate, frameWidth, frameHeight);
	if (FAILED(hr))
	{
		returnValue = FDecoderCheck::UnsucceedCreate;
		goto end;
	}

	hr = FMediaType::createVideoMediaType(&outputType, outputSubtype, frameRate, frameWidth, frameHeight);
	if (FAILED(hr))
	{
		returnValue = FDecoderCheck::UnsucceedCreate;
		goto end;
	}

	hr = pDecoder->SetInputType(0, inputType, 0);
	if(FAILED(hr))
	{
		returnValue = FDecoderCheck::UnsupportedInputSub;
		goto end;
	}

	hr = pDecoder->SetOutputType(0, outputType, 0);
	if (FAILED(hr))
	{
		returnValue = FDecoderCheck::UnsupportedOutputSub;
		goto end;
	}

	m_Decoder.set(pDecoder);

end:
	safeRelease(&inputType);
	safeRelease(&outputType);
	return returnValue;
}

IMFTransform *FDecoder::getDecoder(void)
{
	return m_Decoder.get();
}

std::vector<GUID> FDecoder::listAvailableInputSubtype(void) const
{
	std::vector<GUID> availSubGUID;
	for (std::vector<FDecoderCheck::decoderLine>::size_type i(0), j(FDecoderCheck::decoderCheckTable.size());i < j;++i)
	{
		availSubGUID.push_back(FDecoderCheck::decoderCheckTable[i].inputSubtype);
	}
	return availSubGUID;
}

std::vector<GUID> FDecoder::listAvailableOutputSubtype(const GUID &inputSubtype)
{
	IMFTransform *pDecoder(nullptr);
	HRESULT hr(S_OK);

	bool hasDecoder(false);
	CLSID decoderCLSID;

	for (std::vector<FDecoderCheck::decoderLine>::size_type i(0), j(FDecoderCheck::decoderCheckTable.size());i < j;++i)
	{
		if (FDecoderCheck::decoderCheckTable[i].inputSubtype == inputSubtype)
		{
			hasDecoder = true;
			decoderCLSID = FDecoderCheck::decoderCheckTable[i].rclsid;
			break;
		}
	}
	if (!hasDecoder)
	{
		return std::vector<GUID>();
	}

	hr = CoCreateInstance(decoderCLSID, nullptr, CLSCTX_INPROC_SERVER,
		IID_IMFTransform, (void**)&pDecoder);
	if (FAILED(hr))
	{
		handleErrorHR("Can not read the decoder available output subtype GUID.", hr);
	}

	std::vector<GUID> guids;
	for (DWORD i(0);SUCCEEDED(hr);++i)
	{
		IMFMediaType *pMT(nullptr);
		pDecoder->GetOutputAvailableType(0, i, &pMT);
		GUID subTypeGuid;
		pMT->GetGUID(MF_MT_SUBTYPE, &subTypeGuid);
		if (std::find(guids.cbegin(), guids.cend(), subTypeGuid) == guids.cend())
		{
			guids.push_back(subTypeGuid);
		}
		safeRelease(&pMT);
	}
	safeRelease(&pDecoder);

	return guids;
}
