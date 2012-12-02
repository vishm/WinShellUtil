#pragma once
class WinLibManager
{

public:
	WinLibManager(void);
	~WinLibManager(void);
	
	void ListKnownFolder();
	void AddFolder(std::wstring libraryName, std::wstring folderPath);

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
};

