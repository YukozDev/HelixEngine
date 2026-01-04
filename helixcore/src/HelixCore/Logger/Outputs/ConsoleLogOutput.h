#pragma once
#include <HelixCore/Logger/ILogOutput.h>

class ConsoleLogOutput : public ILogOutput
{
	public:
		ConsoleLogOutput() = default;
		~ConsoleLogOutput() override = default;

		void Write(const LogMessage& message) override;
		void Flush() override;
};

