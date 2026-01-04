#pragma once
#include <chrono>
#include <HelixCore/Build/Build.h>
#include <HelixCore/Logger/LogCategory.h>
#include <HelixCore/Types/Type.h>

#ifdef HX_DEBUG
#define hxLogInfo(categorie, message, ...) LogInternal(categorie, LogType::Info, message, ##__VA_ARGS__)
#define hxLogDebug(categorie, message, ...) LogInternal(categorie, LogType::Debug, message, ##__VA_ARGS__)
#define hxLogWarning(categorie, message, ...) LogInternal(categorie, LogType::Warning, message, ##__VA_ARGS__)
#define hxLogError(categorie, message, ...) LogInternal(categorie, LogType::Error, message, ##__VA_ARGS__)
#define hxLogCritical(categorie, message, ...) LogInternal(categorie, LogType::Critical, message, ##__VA_ARGS__)
#else
	#define hxLogInfo(categorie, message, ...) void(0)
	#define hxLogDebug(categorie, message, ...) void(0)
	#define hxLogWarning(categorie, message, ...) void(0)
	#define hxLogError(categorie, message, ...) void(0)
	#define hxLogCritical(categorie, message, ...) void(0)
#endif // DEBUG

struct LogMessage 
{
	std::chrono::system_clock::time_point Timestamp;
	LogType Type;
	LogCategory Category;
	const hxChar* Message;
};

void LogInternal(LogCategory category, LogType type, const hxChar* message, ...);
