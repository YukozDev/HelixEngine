#include "Logger.h"

Logger::Logger(LogHandler& handler) : m_Handler(handler)
{
}

Logger::~Logger()
{
}

void Logger::FormatMessage(LogCategory& category, const char* message, LogType type)
{
}
