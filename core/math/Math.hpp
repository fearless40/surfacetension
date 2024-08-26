#pragma once

#include <DirectXMath.h>

namespace math
{
	using Matrix = DirectX::XMFLOAT4X4;
	using Vector4 = DirectX::XMFLOAT4;
	using Vector3 = DirectX::XMFLOAT3;
	using Quat = Vector4;

	using fMatrix = DirectX::XMMATRIX;
	using fVector4 = DirectX::XMVECTOR;
	using fQuat = fVector4;

	namespace dxmath
	{
		using namespace DirectX;
	}
}