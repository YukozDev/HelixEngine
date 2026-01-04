#include <HelixCore/Logger/LogMessage.h>
#include <HelixCore/Logger/Logger.h>

void LogInternal(LogCategory categorie, LogType type, const hxChar* message, ...)
{
	// TODO: Format message with variadic arguments

	LogMessage msg;
	msg.Timestamp = std::chrono::system_clock::now();
	msg.Type = type;
	msg.Category = categorie;
	msg.Message = message;

	Logger::GetInstance().Enqueue(msg);
}