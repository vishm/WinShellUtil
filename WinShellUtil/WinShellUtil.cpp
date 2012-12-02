// WinShellUtil.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WinLibManager.h"

void ShowHelp()
{
	std::cout << "WinShellUtil - Simple tool for managing Windows Shell Library\n" << std::endl;	
	
	std::cout << "AddFolder LibraryName FolderPath" << std::endl;
	std::cout << "\tie. AddFolder Documents \\\\NAS\\Home" << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{

	::CoInitialize(nullptr);

	WinLibManager libManager;

	if ( !(argc > 1) )
	{
		ShowHelp();
	}
	else
	{
		try
		{
			std::wstring command = CT2W(argv[1]);
			std::transform( std::begin(command), std::end(command), std::begin(command), ::toupper);
		
			if ( command.compare(L"ADDFOLDER") == 0 )
			{
				std::wstring libraryName = CT2W(argv[2]);
				std::wstring folderName = CT2W(argv[3]);

				libManager.AddFolder(libraryName, folderName);

				std::wcout << L"Successfully " << L"Adding folder \"" << folderName.c_str() << L"\" Library \"" << libraryName.c_str() << "\"" << std::endl;			}
		}
		catch(WinLibManager::hr_exception& e)
		{
			std::cout << e.what() << std::endl;
		}
		catch(std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	
	::CoUninitialize();
	
	return 0;
}

