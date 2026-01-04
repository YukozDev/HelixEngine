#pragma once
#include <HelixCore/Logger/LogCategory.h>
#include <HelixCore/Logger/Outputs/ConsoleLogOutput.h>

// Define the log category for graphics-related logging
DECLARE_LOG_CATEGORY(LogGraphics);

class GraphicsLog
{
public:
	GraphicsLog();
	~GraphicsLog();

	void Initialize();
	void Shutdown();

private:
	ConsoleLogOutput* m_consoleOutput = nullptr;
};
