

#pragma once

#include "Procedural.h"
#include "MeshGeneratorHelper.h"

namespace Procedural {

class SphereGenerator : public MeshGeneratorHelper 
{
public:
	float		mRadius;
	uint32_t	mNumRings;
	uint32_t	mNumSegments;

	SphereGenerator( float radius = 1.0f, uint32_t nbrRings = 16, uint32_t numSegments = 16 ) : 
		mRadius( radius ),
		mNumRings( nbrRings ),
		mNumSegments( numSegments ) 
	{}

	void addToMeshBuilder( Procedural::MeshBuilderType & buffer ); 

};

}