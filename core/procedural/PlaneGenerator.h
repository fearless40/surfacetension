#pragma once

#include "Procedural.h"
#include "MeshGeneratorHelper.h"

namespace Procedural
{

class PlaneGenerator : public MeshGeneratorHelper
{
public:
	PlaneGenerator(void) {
		mNumSegX = 1;
		mNumSegY = 1;
		mNormal = DirectX::XMFLOAT3(0,0,1);
		mSizeX = 1.0f;
		mSizeY = 1.0f;
	}

	~PlaneGenerator(void) { }

	PlaneGenerator( int NumSegX,
					int NumSegY,
					DirectX::XMFLOAT3 Normal,
					float SizeX,
					float SizeY ) 
	{
		mNumSegX = NumSegX;
		mNumSegY = NumSegY;
		mNormal = Normal;
		mSizeX = SizeX;
		mSizeY = SizeY;
	}

	void addToMeshBuilder( Procedural::MeshBuilderType & buffer ); 

public:
	int mNumSegX;
	int mNumSegY;
	DirectX::XMFLOAT3 mNormal;
	float mSizeX;
	float mSizeY;
};

}