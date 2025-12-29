#pragma once
#include <condition_variable>
#include <queue>
#include <mutex>
#include <thread>
#include <vector>
#include <HelixCore/Logger/ILogOutput.h>
#include <HelixCore/Logger/LogMessage.h>
#include <HelixCore/Types/Type.h>


class Logger final
{
	public:
		// Singleton should not be cloneable
		Logger(const Logger& other) = delete;

		// Singleton should not be assignable
		Logger& operator=(const Logger&) = delete;

		static Logger& GetInstance();

		void Initialize();
		void Shutdown();

		void Dispatch();
		void Enqueue(LogMessage& message);

		void RegisterLogOutput(ILogOutput* output);
		void ClearRegisterLogOutput();


	private:
		Logger() = default;
		~Logger();

		static Logger* m_Instance;

		std::mutex m_LoggerMutex;
		std::thread m_LogThread;

		std::vector<ILogOutput*> m_LogOutputs;
		std::queue<LogMessage> m_MessageQueue;
		std::condition_variable m_ConditionVariable;

		hxBool m_Running = false;
};

