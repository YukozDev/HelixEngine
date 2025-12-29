#pragma once
#include <HelixCore/Logger/LogMessage.h>

class ILogOutput
{
public:
	virtual ~ILogOutput() = default;
	virtual void Write(const LogMessage& message) = 0;
	virtual void Flush() = 0;
};