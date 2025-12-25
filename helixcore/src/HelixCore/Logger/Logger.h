#pragma once
#include <HelixCore/Build/Build.h>
#include <HelixCore/Logger/LogCategory.h>
#include <HelixCore/Logger/LogHandler.h>

#ifdef HX_DEBUG
	#define hxLogInfo(category, message) Logger(category, message, LogType::Info)
	#define hxLogDebug(category, message) Logger(category, message, LogType::Debug)
	#define hxLogWarning(category, message) Logger(category, message, LogType::Warning)
	#define hxLogError(category, message) Logger(category, message, LogType::Error)
	#define hxLogCritical(category, message) Logger(category, message, LogType::Critical)
#else
	#define hxLogInfo(category, message) ((void)0)
	#define hxLogDebug(category, message) ((void)0)
	#define hxLogWarning(category, message) ((void)0)
	#define hxLogError(category, message) ((void)0)
	#define hxLogCritical(category, message) ((void)0)
#endif // HX_DEBUG

class Logger
{
public:
	Logger(LogHandler& handler);
	~Logger();

	void FormatMessage(LogCategory& category, const char* message, LogType type);

private:
	LogHandler& m_Handler;
};

