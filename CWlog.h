/*

Rex init 20200721

*/


#pragma once
#ifndef _CWLOG_
#define _CWLOG_

#define CWLOG_DEBUG
#ifdef CWLOG_DEBUG
#define CWDBG(...) OutputDebugString(__VA_ARGS__) 
#elif
#define outputDebugString 
#endif

#include <iostream> 

extern CString g_Msg;


typedef enum {
	CWLOG_PASS = 0,
	CWLOG_FAIL = 1

}CWLOG_ERR;

class CCWLog
{
public:
	CCWLog(IN char* dftPath);
	CCWLog(IN std::string dftPath);
	CCWLog();
	~CCWLog();

	void CreateDefaultPath();
	int WLog(IN char* szData);
	int WLog(IN std::string sData);
	int OpenLog();
	void ClsLog();
	
private:

	char			CurrentDir[MAX_PATH];
	char			LogPath[MAX_PATH];
	FILE			*pF;
};

#endif // !_CWLOG_