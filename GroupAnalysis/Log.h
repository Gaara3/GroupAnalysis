#ifndef __LOG_H__
#define __LOG_H__

#include <string>
using std::string;

class CLog
{
public:
	CLog(void);
	~CLog(void);
	void	WriteLog(const char* pSourcePath, const char* pFunName, const long lLine, const string pLogText);
	void	ScanfWrite(const char* pSourcePath, const char* pFunName, const long lLine, const string pLogText, ...);
protected:
	string	GetTime();
	string  GetFileName();
	string  m_logPath;
};

extern CLog	g_log;

#define SL_LOG(x)				g_log.WriteLog(__FILE__, __FUNCTION__, __LINE__, x)
#define SL_LOG1(x, p1)			<span style="white-space:pre">	</span>g_log.ScanfWrite(__FILE__, __FUNCTION__, __LINE__, x, p1)
#define SL_LOG2(x, p1, p2)		<span style="white-space:pre">	</span>g_log.ScanfWrite(__FILE__, __FUNCTION__, __LINE__, x, p1, p2)
#define SL_LOG3(x, p1, p2, p3)	<span style="white-space:pre">	</span>g_log.ScanfWrite(__FILE__, __FUNCTION__, __LINE__, x, p1, p2, p3)

#endif //__LOG_H__
