#pragma once

#include <DirectXMath.h>

namespace Math {

	typedef DirectX::XMFLOAT2	float2;
	typedef DirectX::XMFLOAT3	float3;
	typedef DirectX::XMFLOAT4	float4;
	typedef DirectX::XMFLOAT4X4	matrix;
	typedef float4				quaterion;
	typedef quaterion			quat;
	typedef DirectX::XMFLOAT2	point;

	// Actual directx internal data type
	typedef DirectX::XMVECTOR	xVector;
	typedef DirectX::XMMATRIX	XMatrix;

	inline DirectX::XMVECTOR toVector( float x ) {
		return DirectX::XMVectorReplicate( x );
	}

	inline DirectX::XMVECTOR toVector( int x ) {
		return DirectX::XMVectorReplicateInt( x );
	}

	inline DirectX::XMVECTOR toVector( float x, float y, float z, float w = 0 ) {
		return DirectX::XMVectorSet( x,y,z,w );
	}

	inline DirectX::XMVECTOR toVector( int x, int y, int z, int w = 0 ) {
		return DirectX::XMVectorSetInt( x,y,z,w );
	}

	inline DirectX::XMVECTOR toVector( const float3 & v ) {
		return DirectX::XMLoadFloat3( &v );
	}

	inline DirectX::XMVECTOR toVector( const float4 v ) {
		return DirectX::XMLoadFloat4( &v );
	}

	inline float toScalar( DirectX::XMVECTOR v ) {
		return DirectX::XMVectorGetX( v );
	}

	inline DirectX::XMFLOAT4 & toFloats( DirectX::XMVECTOR v, DirectX::XMFLOAT4 & values ) {
		DirectX::XMStoreFloat4( &values, v );
		return values;
	}

	inline DirectX::XMFLOAT3 & toFloats( DirectX::XMVECTOR v, DirectX::XMFLOAT3 & values) {
		DirectX::XMStoreFloat3(&values, v);
		return values;
	}
};