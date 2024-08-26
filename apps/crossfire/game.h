
#pragma once

//#include "Coord.h"
#include "../engine/GameEvent.h"


namespace Game {

	void Initalize();

	void onLogicEvent( const Engine::NextLogicFrame & frame );

	void onRenderEvent( const Engine::NextRenderFrame & frame);

	void onGameInitalizeEvent(const Engine::GameInitalizeData & data);
}

namespace GameUtil {
	//using CoordVector = std::vector<Game::FixedFunctionFloat>;
}