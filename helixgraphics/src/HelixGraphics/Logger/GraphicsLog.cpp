#include <HelixGraphics/Logger/GraphicsLog.h>

#include <HelixCore/Logger/Logger.h>
#include <HelixCore/Logger/Outputs/ConsoleLogOutput.h>

// Define the log category for graphics-related logging
DEFINE_LOG_CATEGORY(LogGraphics, "Graphics", LogType::All);

GraphicsLog::GraphicsLog()
{
}

GraphicsLog::~GraphicsLog()
{
	Shutdown();
}

void GraphicsLog::Initialize()
{
	// TODO : Need to be initialized somewhere else in Core or entry point
	Logger::GetInstance().Initialize();

	// TODO: Rework log outputs management
	m_consoleOutput = new ConsoleLogOutput();
	Logger::GetInstance().RegisterLogOutput(m_consoleOutput);
}

void GraphicsLog::Shutdown()
{
	if (m_consoleOutput)
	{
		delete m_consoleOutput;
		m_consoleOutput = nullptr;
	}
}