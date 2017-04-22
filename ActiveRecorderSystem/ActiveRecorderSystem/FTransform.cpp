#include "FTransform.h"

FTransform::FTransform(void)
{

}

FTransform::FTransform(const FTransform &)
{

}

FTransform::~FTransform(void)
{

}

FTransform *FTransform::getPointer(void)
{
	return singleTon<FTransform>::instance();
}

bool FTransform::hasTramsFun(const GUID & subtype) const
{
	for (std::vector<FTransFun>::size_type i(0), j(funCheckTable.size());i < j; ++i)
	{
		if (subtype == funCheckTable[i].inputBuffSubtype)
		{
			return true;
		}
	}
	return false;
}

pTransFun_t FTransform::getFunBySubGuid(const GUID &subtype)
{
	for (std::vector<FTransFun>::size_type i(0), j(funCheckTable.size());i < j; ++i)
	{
		if (subtype == funCheckTable[i].inputBuffSubtype)
		{
			return funCheckTable[i].pFun;
		}
	}
	return nullptr;
}

std::vector<GUID> FTransform::listAvailableSubtype(void) const
{
	std::vector<GUID> availSubGUID;
	for (std::vector<FTransFun>::size_type i(0), j(funCheckTable.size());i < j; ++i)
	{
		availSubGUID.push_back(funCheckTable[i].inputBuffSubtype);
	}
	return availSubGUID;
}