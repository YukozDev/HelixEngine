#include <HelixCore/Logger/Logger.h>

// Static
Logger* Logger::m_Instance = nullptr;

Logger& Logger::GetInstance()
{
	if(m_Instance == nullptr)
	{
		m_Instance = new Logger();
	}

	return *m_Instance;
}

Logger::~Logger()
{
	if (m_Running)
	{
		Shutdown();
	}
}

void Logger::Initialize()
{
	if(m_Running)
	{
		return;
	}	

	std::lock_guard<std::mutex> lock(m_LoggerMutex);

	m_LogThread = std::thread([this]() { Dispatch(); });

	m_Running = true;
}

void Logger::Dispatch()
{
	while (true)
	{
		LogMessage message;

		{
			std::unique_lock<std::mutex> lock(m_LoggerMutex);

			// Sleep as long as there is no message in the queue or the logger is running
			m_ConditionVariable.wait(lock, [this]() {
				return !m_MessageQueue.empty() || !m_Running;
			});

			if (!m_Running && m_MessageQueue.empty())
			{
				break;
			}

			message = m_MessageQueue.front();
			m_MessageQueue.pop();
		}

		std::vector<ILogOutput*> copyLogOutputs;
		{
			// To avoid race condition and invalide data
			std::lock_guard<std::mutex> lock(m_LoggerMutex);

			// To avoid dead lock and perf
			copyLogOutputs = m_LogOutputs;
		}

		for (ILogOutput* output : copyLogOutputs)
		{
			if (output == nullptr)
			{
				return;
			}
			
			// The write operation can be slow, so we do not want to hold the lock during this operation
			// WARN : The write operation does not have an internal lock
			output->Write(message);
		}
	}
}

void Logger::Enqueue(LogMessage& message)
{
	{
		std::lock_guard<std::mutex> lock(m_LoggerMutex);

		if (!m_Running)
		{
			return;
		}

		m_MessageQueue.push(message);
	}

	// Wake up the logging thread that a new message is available
	m_ConditionVariable.notify_all();
}

void Logger::Flush()
{
	std::lock_guard<std::mutex> lock(m_LoggerMutex);

	if (m_LogOutputs.empty())
	{
		return;
	}

	for (ILogOutput* output : m_LogOutputs)
	{
		if (output == nullptr)
		{
			return;
		}
		output->Flush();
	}
}

void Logger::RegisterLogOutput(ILogOutput* output)
{
	std::lock_guard<std::mutex> lock(m_LoggerMutex);

	if (output == nullptr)
	{
		return;
	}

	if (std::find(m_LogOutputs.begin(), m_LogOutputs.end(), output) == m_LogOutputs.end())
	{
		m_LogOutputs.push_back(output);
	}
}

void Logger::ClearRegisterLogOutput()
{
	std::lock_guard<std::mutex> lock(m_LoggerMutex);

	if (m_LogOutputs.empty())
	{
		return;
	}

	m_LogOutputs.clear();
}

void Logger::Shutdown()
{
	std::lock_guard<std::mutex> lock(m_LoggerMutex);

	if (!m_Running)
	{
		return;
	}

	m_Running = false;
	m_ConditionVariable.notify_all();

	if(m_LogThread.joinable())
	{
		// Wait for the logging thread to finish
		m_LogThread.join();
	}

	// Flush all log outputs
	Flush();

	// Logger Singleton
	delete m_Instance;
	m_Instance = nullptr;
}
