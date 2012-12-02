// WinShellUtil.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WinLibManager.h"

int _tmain(int argc, _TCHAR* argv[])
{

	::CoInitialize(nullptr);

	WinLibManager libManager;
	libManager.ExecuteCommand(argc, argv);

	
	::CoUninitialize();
	
	return 0;
}

