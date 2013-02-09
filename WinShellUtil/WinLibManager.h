#pragma once



/// Simple class that manages all interactions with Windows Library and aims to
/// offer high level functionality
class WinLibManager
{	
public:		
	void AddFolder(const std::wstring& strLibraryName, const std::wstring& strFolderPath);
	void RemoveFolder(const std::wstring& strLibraryName, const std::wstring& strFolderPath);
	void AddRemoveFolder(const std::wstring& strLibraryName, const std::wstring& strFolderPath, bool bAddFolder = true);


	// store for tracking bad HRESULT that are raised.
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

