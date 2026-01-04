#pragma once
#include <HelixCore/Types/Type.h>

enum class LogType : hxByte
{
	Info,
	Debug,
	Warning,
	Error,
	Critical,
	All
};

#define DECLARE_LOG_CATEGORY(logCategoryName) extern LogCategory logCategoryName; 

#define DEFINE_LOG_CATEGORY(logCategoryName, name, logType) LogCategory logCategoryName(name, logType);

class LogCategory
{
public:
	LogCategory() = default;

	LogCategory(const hxChar* name, LogType type) : m_Name(name), m_Type(type)
	{} 
	
	const hxChar* GetName() const { return m_Name; } 
	LogType GetType() const { return m_Type; }

private:
	const hxChar* m_Name = nullptr;
	LogType m_Type = LogType::All;
};
