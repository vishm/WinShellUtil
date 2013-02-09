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

void WinLibManager::AddFolder(const std::wstring& strLibraryName, const std::wstring& strFolderPath)
{
	AddRemoveFolder(strLibraryName, strFolderPath);
}

void WinLibManager::RemoveFolder(const std::wstring& strLibraryName, const std::wstring& strFolderPath)
{
	AddRemoveFolder(strLibraryName, strFolderPath, false);
}


///////////////////////////////////////////////////////////////////////////////
// WinLibManager::AddRemoveFolder() - Add a folder path to existing Windows library
//
// Exceptions:	std::exception	- bad folder path supplied
//				hr_exception	- COM related error that may have been raised by 
//								  Windows Shell COM calls.
void WinLibManager::AddRemoveFolder(const std::wstring& strLibraryName, 
									const std::wstring& strFolderPath, 
									bool bAddFolder /*= true*/) 
{
	// APIs access the real library name as exists on file system as described below
	// http://msdn.microsoft.com/en-us/library/windows/desktop/dd798389(v=vs.85).aspx
	const LPWSTR strLibraryName_EXT = L".library-ms";
	std::wstring realstrLibraryName(strLibraryName);
	realstrLibraryName += std::wstring(strLibraryName_EXT);

	if ( !::PathFileExists(strFolderPath.c_str()) )
	{
		std::wstringstream errMsg;
		errMsg << "Known folder \"" << strLibraryName << "\" will not be updated as "<< L"\"" << strFolderPath << L"\"" << L" was not found";
		CW2A errString(errMsg.str().c_str());
		throw std::exception(errString);
	}

	try
	{
		// Aim to get relevant library
		CComPtr<IShellItem2> ptrShellItem;
		HR( ::SHCreateItemInKnownFolder(FOLDERID_UsersLibraries, KF_FLAG_DEFAULT_PATH|KF_FLAG_NO_ALIAS, 
			realstrLibraryName.c_str(), IID_PPV_ARGS(&ptrShellItem)) );
		
		// Now lets add folder path to library collection	
		CComPtr<IShellLibrary> ptrShellLibrary;
		HR( ::SHLoadLibraryFromItem(ptrShellItem, STGM_READWRITE, IID_PPV_ARGS(&ptrShellLibrary)) );
	
		
		HR( bAddFolder ? 
			::SHAddFolderPathToLibrary(ptrShellLibrary, strFolderPath.c_str()) :
			::SHRemoveFolderPathFromLibrary(ptrShellLibrary, strFolderPath.c_str()) );
	
		HR( ptrShellLibrary->Commit() );		

	}
	catch(hr_exception& e)
	{
		std::wstringstream errMsg;
		errMsg << L"Failure to " << (bAddFolder ? L"AddFolder" : L"RemoveFolder") 
								 << L" \"" << strFolderPath.c_str() << "\" to library " 
								 << "\"" << strLibraryName << "\"" << std::endl;
		CW2T errMessage(errMsg.str().c_str());

		throw hr_exception(errMessage, e.GetHRESULT());
	}


}