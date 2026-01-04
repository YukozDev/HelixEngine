#include <chrono>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <HelixCore/Logger/Outputs/ConsoleLogOutput.h>

void ConsoleLogOutput::Write(const LogMessage& message)
{
	/*
		msg.Timestamp = std::chrono::system_clock::now();
		msg.Type = type;
		msg.Category = categorie;
		msg.Message = message;

		Logger::GetInstance().Enqueue(msg);
		[RenderThread] [ThreadId = 1] [14:22:01.123] DrawCall submitted

		14:22:01.123 [Debug - Graphics] : DrawCall submitted
	*/

	// Convert timestamp to time_t for formatting
	std::time_t time = std::chrono::system_clock::to_time_t(message.Timestamp);
	std::tm  tm{};

	// Convert to local time (WIN32), thread-safe version 
	localtime_s(&tm, &time);

	std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " [" /* << static_cast<hxInt>(message.Type) << " - " */ << message.Category.GetName() << "] : " << message.Message << "\n";
}

void ConsoleLogOutput::Flush()
{
	fflush(stdout);
}
