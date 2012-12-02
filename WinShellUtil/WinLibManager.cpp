#include "stdafx.h"
#include "WinLibManager.h"

#define HR(x) { HRESULT _hrTemp0002 = x; if ( FAILED(_hrTemp0002) ) throw hr_exception(_hrTemp0002); }

class hr_exception : public std::exception
{
	HRESULT m_hresult;
	std::string m_errorString;

public:
	hr_exception(HRESULT hr) : std::exception()
	{
		m_hresult = hr;
	}

	hr_exception(LPCTSTR message, HRESULT hr) : std::exception()
	{
		m_hresult = hr;
		m_errorString = CT2A(message);
	}

	HRESULT GetHRESULT()
	{
		return m_hresult;		
	}

	virtual const char *what() const
	{
		return m_errorString.c_str();
	}
	
};

WinLibManager::WinLibManager(void)
{
}


WinLibManager::~WinLibManager(void)
{
}

void WinLibManager::AddFolder(std::wstring libraryName, std::wstring folderPath)
{

	// APIs access the real library name as exists on file system as described below
	// http://msdn.microsoft.com/en-us/library/windows/desktop/dd798389(v=vs.85).aspx
	const LPWSTR LIBRARYNAME_EXT = L".library-ms";
	std::wstring realLibraryName(libraryName);
	realLibraryName += std::wstring(LIBRARYNAME_EXT);

	if ( !::PathFileExists(folderPath.c_str()) )
	{
		std::wstringstream errMsg;
		errMsg << "\"" << libraryName << "\" will not be updated as "<< L"\"" << folderPath << L"\"" << L" was not found";
		CW2A errString(errMsg.str().c_str());
		throw std::exception(errString);
	}

	try
	{
		// Aim to get relevant library
		CComPtr<IShellItem2> ptrShellItem;
		HR( ::SHCreateItemInKnownFolder(FOLDERID_UsersLibraries, KF_FLAG_DEFAULT_PATH|KF_FLAG_NO_ALIAS, 
			realLibraryName.c_str(), IID_PPV_ARGS(&ptrShellItem)) );
		
		// Now lets add folder path to library collection	
		CComPtr<IShellLibrary> ptrShellLibrary;
		HR( ::SHLoadLibraryFromItem(ptrShellItem, STGM_READWRITE, IID_PPV_ARGS(&ptrShellLibrary)) );
	
		HR( ::SHAddFolderPathToLibrary(ptrShellLibrary, folderPath.c_str()) );
		HR( ptrShellLibrary->Commit() );		
	}
	catch(hr_exception& e)
	{
		std::wstringstream errMsg;
		errMsg << L"Failure to AddFolder \"" << folderPath.c_str() << "\" to library " << "\"" << libraryName << "\"" << std::endl;
		CW2T errMessage(errMsg.str().c_str());

		throw hr_exception(errMessage, e.GetHRESULT());
	}
}

void WinLibManager::ListKnownFolder()
{
}
