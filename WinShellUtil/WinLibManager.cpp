#include "stdafx.h"
#include <algorithm>
#include "WinLibManager.h"

WinLibManager::WinLibManager(void)
{
}


WinLibManager::~WinLibManager(void)
{
}


void WinLibManager::ExecuteCommand(int argc, _TCHAR* argv[])
{
	if ( !(argc > 1) )
	{
		ShowHelp();
	}
	else
	{
		std::wstring command = CT2W(argv[1]);
		std::transform( std::begin(command), std::end(command), std::begin(command), ::toupper);
		
		if ( command.compare(L"LIST") == 0 )
		{
			std::wstring libraryName = CT2W(argv[2]);
			std::wstring folderName = CT2W(argv[3]);

			ListKnownFolder();
		} else if ( command.compare(L"ADDFOLDER") == 0 )
		{
			std::wstring libraryName = CT2W(argv[2]);
			std::wstring folderName = CT2W(argv[3]);

			AddFolder(libraryName, folderName);
		}
	}
}

void WinLibManager::AddFolder(std::wstring libraryName, std::wstring folderPath)
{
	// APIs access the real library name as exists on file system as described below
	// http://msdn.microsoft.com/en-us/library/windows/desktop/dd798389(v=vs.85).aspx
	
	std::wstring realLibraryName(libraryName);
	realLibraryName += std::wstring(L".library-ms");

	// Aim to get relevant library
	CComPtr<IShellItem2> ptrShellItem;
	HRESULT hr = ::SHCreateItemInKnownFolder(FOLDERID_UsersLibraries, KF_FLAG_DEFAULT_PATH|KF_FLAG_NO_ALIAS, 
		realLibraryName.c_str(), IID_PPV_ARGS(&ptrShellItem));

	// Now lets add folder path to library collection
	if ( SUCCEEDED(hr) )
	{
		CComPtr<IShellLibrary> ptrShellLibrary;
		hr = ::SHLoadLibraryFromItem(ptrShellItem, STGM_READWRITE, IID_PPV_ARGS(&ptrShellLibrary));

		if ( SUCCEEDED(hr) )
		{
			std::wstring operationResult(L"Failure");
			hr = ::SHAddFolderPathToLibrary(ptrShellLibrary, folderPath.c_str());
			if ( SUCCEEDED(hr) ) 
			{
				hr = ptrShellLibrary->Commit();
				operationResult = L"Success: ";
			}

			std::wcout << operationResult.c_str() << L"Adding folder \"" << folderPath.c_str() << L"\" Library \"" << libraryName.c_str() << "\"" << std::endl;
		}
	}


}

void WinLibManager::ListKnownFolder()
{
}

void WinLibManager::ShowHelp()
{
	std::cout << "help" << std::endl;
}