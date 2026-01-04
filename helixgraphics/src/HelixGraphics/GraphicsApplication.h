#pragma once
#include <HelixGraphics/Logger/GraphicsLog.h>

class GraphicsApplication
{
	public:
		void Run();
		void Shutdown();

		void Draw();

	private:
		GraphicsLog* m_GraphicsLog = nullptr;
};

