#pragma once
#include <HelixCore/Types/Type.h>

enum class LogType : hxByte
{
	Info,
	Debug,
	Warning,
	Error,
	Critical
};

#define DECLARE_LOG_CATEGORY(logCategoryName) extern LogCategory logCateroryName;

#define DEFINE_LOG_CATEGORY(logCategoryName) LogCategory logCategoryName(logCategoryName);

class LogCategory
{
public:
	LogCategory(const hxChar* name);
};