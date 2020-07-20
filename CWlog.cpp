/*

Rex init 20200721

*/

#include "pch.h"
#include "CWLog.h"

//.................................................................................
int CCWLog::OpenLog() 
{
	this->pF = ::fopen(LogPath, _T("a+"));
	if (pF == NULL)
	{
		OutputDebugString("X, CCWLog");
		return CWLOG_FAIL;
	}
	else
	{
		OutputDebugString("O, CCWLog");
		//::fclose(pF);
	}
	return CWLOG_PASS;
}

//.................................................................................
void CCWLog::ClsLog() {
	if (pF) {
		::fclose(pF);
		pF = NULL;
	}
}

//.................................................................................
CCWLog::CCWLog(IN char* dftPath)
{
	strcpy(LogPath, dftPath);

	if (this->OpenLog() == CWLOG_FAIL) {
		CreateDefaultPath();
	}
}

//.................................................................................
CCWLog::CCWLog(IN std::string dftPath)
{
	strcpy(LogPath, dftPath.c_str());

	if (this->OpenLog() == CWLOG_FAIL) {
		CreateDefaultPath();
	}
}

//.................................................................................
CCWLog::CCWLog()
{
	CreateDefaultPath();
}

//.................................................................................
void CCWLog::CreateDefaultPath() {

	char		CurrentDir[MAX_PATH];
	SYSTEMTIME	systemTime;

	::memset(CurrentDir, 0, sizeof(CurrentDir));
	::GetCurrentDirectory(MAX_PATH, CurrentDir);

	GetLocalTime(&systemTime);
	char sysdate_logName[100];
	sprintf(sysdate_logName, ("%04d_%02d_%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay);

	::sprintf(LogPath, _T("%s\\%s.log"), CurrentDir, sysdate_logName);


	if (this->OpenLog() == CWLOG_FAIL) {
		CWDBG("X, CCWLog, CreateDefaultPath");
	}
	else
	{
		CWDBG("O, CCWLog, CreateDefaultPath");
		this->ClsLog();
	}

}

//.................................................................................
CCWLog::~CCWLog()
{
	this->ClsLog();
}

//.................................................................................
int CCWLog::WLog(IN char* szData) {
	SYSTEMTIME systemTime;

	if (this->OpenLog() == CWLOG_FAIL) {
		CWDBG("X, CCWLog, WLog");
		return CWLOG_FAIL;
	}

	GetLocalTime(&systemTime);
	char sysdate[100];
	sprintf(sysdate, _T("%02d:%02d:%02d%:%03d"), systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);

	::_ftprintf(pF, _T("%s %s\n"), sysdate, szData);
	::fflush(pF);
	
	this->ClsLog();
	// ::fclose(pF);
	return CWLOG_PASS;
}


//.................................................................................
int CCWLog::WLog(IN std::string sData)
{
	this->WLog((char*) sData.c_str());
	return CWLOG_PASS;
}



//demo code
void DemoOnly() {

	/*
	//create With Full path
	CCWLog cw1("E:\\win7_utility\\TEST.log");
	cw1.WLog("CW1 a123");
	std::string s = ("CW1 A123456789 123456789");
	cw1.WLog(s.c_str()); //input std::string

	//create with current path and default log name(current date)
	CCWLog cw2;
	cw2.WLog("cw2 a123");
	std::string s1 = ("CW2 A123456789 123456789");
	cw2.WLog((char*)s1.c_str());

	//create with current path and log name = "TEST"
	CCWLog cw3("TEST.log");
	cw3.WLog("cw3 12"); //input char
	*/
}