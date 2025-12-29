#include "Logger.h"


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
	std::lock_guard<std::mutex> lock(m_LoggerMutex);

	if(m_Running)
	{
		return;
	}	

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

	// wake up the thread for process the message
	m_ConditionVariable.notify_one();
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
	delete m_Instance;

	m_Instance = nullptr;
}
