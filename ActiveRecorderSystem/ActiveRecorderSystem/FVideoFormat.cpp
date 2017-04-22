#include "FVideoFormat.h"

std::string FVideoFormat::subtypeGuidToString(const GUID &subtype)
{
	for (int i(0), j(FVideoFormatCheckTable::mtvfms.size());i < j; ++i)
	{
		if (::IsEqualGUID(FVideoFormatCheckTable::mtvfms[i].guid, subtype))
		{
			return FVideoFormatCheckTable::mtvfms[i].name;
		}
	}
	for (int i(0), j(FVideoFormatCheckTable::mtvfms_encoded.size());i < j;++i)
	{
		if (::IsEqualGUID(FVideoFormatCheckTable::mtvfms_encoded[i].guid, subtype))
		{
			return FVideoFormatCheckTable::mtvfms_encoded[i].name;
		}
	}
	return std::string("Unknown Subtype.");
}

int FVideoFormat::subtypeIsEncoded(const GUID &subtype)
{
	for (int i(0), j(FVideoFormatCheckTable::mtvfms.size());i < j; ++i)
	{
		if (FVideoFormatCheckTable::mtvfms[i].guid == subtype)
		{
			return FVideoFormatCheckTable::Uncompressed;
		}
	}
	for (int i(0), j(FVideoFormatCheckTable::mtvfms_encoded.size());i < j;++i)
	{
		if (FVideoFormatCheckTable::mtvfms_encoded[i].guid == subtype)
		{
			return FVideoFormatCheckTable::Compressed;
		}
	}

	return FVideoFormatCheckTable::UnknownSubtypeGUID;
}
