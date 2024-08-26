#pragma once

namespace Engine {
	struct NextLogicFrame {
		float elapsedTimeMS;
		NextLogicFrame(float tm) : elapsedTimeMS(tm) {}
	};

	struct NextRenderFrame {
		float elapsedTimeMS;
		NextRenderFrame(float tm) : elapsedTimeMS(tm) {}
	};

	struct KeyEvent {
		unsigned short vkCode{ 0 };
		bool isKeyUp{ false };
		int repeatCount{ 0 };
	};

	struct GameInitalizeData {};
}