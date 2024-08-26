#pragma once

#include "Math.h"
#include "../Graphics/Generic/Color.h"
#include <variant>

namespace Engine::Lights {

	struct Directional {
		Engine::Vector3 direction;
		Graphics::Generic::RGBA color;
	};

	struct Spot {
		Engine::Vector3 direction;
		Engine::Vector3 position;
		Graphics::Generic::RGBA color;
		float       SpotAngle;              // 4 bytes
		float       ConstantAttenuation;    // 4 bytes
		float       LinearAttenuation;      // 4 bytes
		float       QuadraticAttenuation;   // 4 bytes
	};

	struct Point {
		Engine::Vector3 position;
		Graphics::Generic::RGBA color;
		float       ConstantAttenuation;    // 4 bytes
		float       LinearAttenuation;      // 4 bytes
		float       QuadraticAttenuation;   // 4 bytes
	};

	using AnyLight = std::variant<Directional, Spot, Point>;

}