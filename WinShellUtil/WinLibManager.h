#pragma once
class WinLibManager
{
public:
	WinLibManager(void);
	~WinLibManager(void);

	void ExecuteCommand(int argc, _TCHAR* argv[]);
	void ShowHelp();


	void ListKnownFolder();
	void AddFolder(std::wstring libraryName, std::wstring folderPath);
};

