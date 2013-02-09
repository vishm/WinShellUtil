// WinShellUtil.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WinLibManager.h"

///////////////////////////////////////////////////////////////////////////////
// Simple help information.
void ShowHelp()
{
	std::cout << "WinShellUtil - Simple tool for managing Windows Shell Library\n" << std::endl;	
	
	std::cout << "AddFolder LibraryName FolderPath" << std::endl;
	std::cout << "\tie. AddFolder Documents \\\\NAS\\Home" << std::endl;
	std::cout << "RemoveFolder LibraryName FolderPath" << std::endl;
	std::cout << "\tie. AddFolder Documents \\\\NAS\\Home" << std::endl;

}

enum OPERATIONENUM { Invalid, AddFolder, RemoveFolder};

///////////////////////////////////////////////////////////////////////////////
// Convert argument command into an operation
OPERATIONENUM GetOperation(std::wstring command)
{
	struct COMMANDSTRUCT
	{	
		std::wstring	m_argName;
		OPERATIONENUM	m_enumValue;	
	};

	COMMANDSTRUCT instructions[] = 
	{ 
		{L"ADDFOLDER",		OPERATIONENUM::AddFolder},
		{L"REMOVEFOLDER",	OPERATIONENUM::RemoveFolder}
	};

	// Uppercase command and then see if we support that option
	std::transform( std::begin(command), std::end(command), std::begin(command), ::toupper);
		
	auto foundEntry = std::find_if( std::begin(instructions), std::end(instructions), 
						[&](COMMANDSTRUCT cmd){ return command.compare(cmd.m_argName) == 0;});

	return (foundEntry != std::end(instructions)) ?  foundEntry->m_enumValue : OPERATIONENUM::Invalid;
}

int _tmain(int argc, _TCHAR* argv[])
{
	::CoInitialize(nullptr);

	WinLibManager libManager;

	if ( !(argc > 1) )	// Show help if insufficient number <= 1 arg specified.///
	{
		ShowHelp();
	}
	else
	{
		try
		{
			// Figure out what action was requestd from command line
			std::wstring command = CT2W(argv[1]);
			OPERATIONENUM commandId = GetOperation(command);
			if ( commandId == OPERATIONENUM::Invalid )
			{
				std::wcout << L"Unknown command specified " << command << std::endl;
				std::wcout << std::endl;
				ShowHelp();
			}
			else
			{
				std::wstring libraryName = CT2W(argv[2]);
				std::wstring folderName = CT2W(argv[3]);

				std::wstring operationToAction;
				bool bInvalidCommand = false;

				switch (commandId)
				{
					case OPERATIONENUM::AddFolder:
						{
							libManager.AddFolder(libraryName, folderName);
							operationToAction = L"Adding folder";						
							break;
						}
					case OPERATIONENUM::RemoveFolder:
						{
							libManager.RemoveFolder(libraryName, folderName);
							operationToAction = L"Removing folder";
							break;
						}
					default:
						bInvalidCommand = true;
						break;
				}

				std::wcout << L"Successfully " << operationToAction.c_str() << " \"" << folderName.c_str() 
							<< L"\" Library \"" << libraryName.c_str() << "\"" << std::endl;
			}
		}
		catch(WinLibManager::hr_exception& e)
		{
			std::cout << "COM Error: " << e.what() << std::endl;
		}
		catch(std::exception& e)
		{
			std::cout << "General Error: " << e.what() << std::endl;
		}
	}

	
	::CoUninitialize();
	
	return 0;
}

