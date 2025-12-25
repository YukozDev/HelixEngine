#pragma once
#include <HelixCore/Types/Type.h>

// Use this macro in header files to declare a log category
#define DECLARE_LOG_CATEGORY(logCategoryName) extern LogCategory logCategoryName;

// Use this macro in one cpp file to avoid multiple definition errors
#define DEFINE_LOG_CATEGORY(logCategoryName) LogCategory logCategoryName(#logCategoryName);

enum class LogType : hxByte
{
	Info,
	Debug,
	Warning,
	Error,
	Critical
};

class LogCategory
{
public:
	explicit LogCategory(const char* name);
	~LogCategory();

private:

};
