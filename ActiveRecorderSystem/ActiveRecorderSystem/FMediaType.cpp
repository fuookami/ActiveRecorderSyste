#include "FMediaType.h"
#include "FVideoFormat.h"
#include <sstream>

HRESULT FMediaType::createVideoMediaType(IMFMediaType **ppMT, const GUID &subtype, 
	const unsigned int frameRate, const unsigned int frameWidth, const unsigned int frameHeight)
{
	HRESULT hr(S_OK);
	IMFMediaType *pMT(nullptr);

	hr = MFCreateMediaType(&pMT);
	if (FAILED(hr))
	{
		return hr;
	}

	pMT->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
	pMT->SetGUID(MF_MT_SUBTYPE, subtype);
	MFSetAttributeRatio(pMT, MF_MT_FRAME_RATE, frameRate, 1);
	MFSetAttributeSize(pMT, MF_MT_FRAME_SIZE, frameWidth, frameHeight);

	safeRelease(ppMT);
	*ppMT = pMT;
	return S_OK;
}

UINT32 FMediaType::getFrameRate(IMFMediaType *pmt)
{
	MFRatio currFPSR;
	UINT32 currFPS(0);
	MFGetAttributeRatio(pmt, MF_MT_FRAME_RATE, (UINT32 *)&(currFPSR.Numerator),
		(UINT32 *)&(currFPSR.Denominator));
	currFPS = currFPSR.Numerator / currFPSR.Denominator;
	return currFPS;
}

std::string FMediaType::getMediaTypeInfo(IMFMediaType *pmt, const int index)
{
	GUID subtype;
	pmt->GetGUID(MF_MT_SUBTYPE, &subtype);
	UINT32 currFPS(FMediaType::getFrameRate(pmt));
	UINT32 wid(0), hei(0);
	MFGetAttributeSize(pmt, MF_MT_FRAME_SIZE, &wid, &hei);

	std::ostringstream sout(std::ostringstream::out);

	if (index != -1)
	{
		sout << index;
	}
	sout << '\t' << FVideoFormat::subtypeGuidToString(subtype) << '\t' << currFPS <<
		'\t' << wid << " x " << hei << std::endl;

	return sout.str();
}