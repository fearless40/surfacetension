#pragma once

#include "Procedural.h"
#include "MeshGeneratorHelper.h"

namespace Procedural {

class BoxGenerator : public MeshGeneratorHelper
{
public:
	BoxGenerator(void) : mSizeX(1.0f),mSizeY(1.0f),mSizeZ(1.0f),mNumSegX(1),mNumSegY(1),mNumSegZ(1) { }
	~BoxGenerator(void) { }

	BoxGenerator(float SizeX, float SizeY, float SizeZ,int NumSegX, int NumSegY, int NumSegZ) : mSizeX(SizeX),mSizeY(SizeY),mSizeZ(SizeZ),mNumSegX(NumSegX),mNumSegY(NumSegY),mNumSegZ(NumSegZ) { }



	void addToMeshBuilder( Procedural::MeshBuilderType & buffer );

	float mSizeX,mSizeY,mSizeZ;
	int mNumSegX,mNumSegY,mNumSegZ;
};

}