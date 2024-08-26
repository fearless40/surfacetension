#pragma once
#include "Math.h"
#include "../Graphics/Generic/Color.h"

namespace Engine {
	struct Material {
		Graphics::Generic::RGBA emissive;
		Graphics::Generic::RGBA diffuse;
		Graphics::Generic::RGBA ambient;
		Graphics::Generic::RGBA specular;
		float specular_power;
	};
}