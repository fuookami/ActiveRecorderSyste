#pragma once

/************************************************************************/
/*  File:		FCapture.h
	Writer:		Fuookami
	Version:	v1.0
	Date:		2016-3-20
	Needed:		Windows SDK 7.0 or Windows KIT 8.0 ~ 10.0
	Description:
			Create a capture by Microsoft Media Foundation in Windows SDK.
		Because Windows SDK 7.0 is included in Windows KIT, so these classes
		can be used on the computer which has been installed Windows KIT.
			This File declares all the header files which are used in the 
		program, and declares some basic important funcion.

*/
/************************************************************************/

#include <windows.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mftransform.h>
#include <wmcodecdsp.h>
#include <evr.h>
#include <shlwapi.h>
#include <mferror.h>
#include <memory>

EXTERN_C const GUID DECLSPEC_SELECTANY CLSID_CMSH264DecoderMFT =
	{ 0x62CE7E72, 0x4C71, 0x4d20, 0xB1, 0x5D, 0x45, 0x28, 0x31, 0xA8, 0x7D, 0x9D };

using result = int;

template <class T>
void safeRelease(T **ppT)
{
	if (*ppT != nullptr)
	{
		(*ppT)->Release();
		*ppT = nullptr;
	}
}

template <class T>
void closeQTObject(T **ppT)
{
	if (*ppT != nullptr)
	{
		(*ppT)->close();
		delete (*ppT);
		(*ppT) = nullptr;
	}
}

template <class T>
class pMFType
{
public:
	pMFType(void) :pMFObj(nullptr) {};
	pMFType(T *_p) : pMFObj(_p) {};
	~pMFType(void) { safeRelease(&pMFObj); };
	T *get(void) { return pMFObj; };
	T **getPointer(void) { return &pMFObj; };
	void set(T *_p) { pMFObj = _p };
private:
	T *pMFObj;
};

template <class T>
class spMFType
{
public:
	spMFType(void) :MFObj(new pMFType<T>()) {};
	spMFType(T *_MFObj) :MFObj(new pMFType<T>(_MFObj)) {};
	spMFType(const spMFType<T> &ano) : MFObj(ano.MFObj) {};
	~spMFType(void) {};
	T *get(void) { return MFObj->get(); };
	T **getPointer(void) { return MFObj->getPointer(); };
	void set(T *_MFObj) { MFObj.reset(new pMFType<T>(_MFObj)); };
	T *operator=(T *_MFObj) { set(_MFObj); return get(); };
private:
	std::shared_ptr<pMFType<T>> MFObj;
};

template <class T>
class singleTon
{
public:
	static T *instance(void);
protected:
	singleTon(void) {};
	singleTon(const singleTon &) {};
	~singleTon(void) {};
	singleTon &operator=(const singleTon &) {};
private:
	static std::shared_ptr<T> m_pInstance;
};

template <class T>
std::shared_ptr<T> singleTon<T>::m_pInstance = nullptr;

template <class T>
T *singleTon<T>::instance(void)
{
	if (m_pInstance == nullptr)
	{
		m_pInstance.reset(new T());
	}
	return m_pInstance.get();
}