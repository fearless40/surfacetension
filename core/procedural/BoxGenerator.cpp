#include "../stdafx.h"
#include "BoxGenerator.h"
#include "PlaneGenerator.h"

using namespace Procedural;
using namespace DirectX;

void BoxGenerator::addToMeshBuilder( Procedural::MeshBuilderType & buffer )
{
	PlaneGenerator pg;
	
	auto calcPosition = []( XMVECTOR pos, float mSize, XMVECTOR orient,  XMVECTOR unitdir )  {
		XMVECTOR calc = XMVector3Rotate(unitdir, orient);
		calc = XMVectorScale( calc, mSize * 0.5f);
		return XMVectorAdd( pos, calc );
	};

	if (mTransform)
	{
		pg.setScale(mScale);
		pg.setOrientation(mOrientation);
	}

	pg.mNumSegX = mNumSegY;
	pg.mNumSegY = mNumSegX;
	pg.mSizeX = mSizeY;
	pg.mSizeY = mSizeX;
	pg.mNormal = XMFLOAT3(0.f,0.f,-1.f);
	pg.setPosition( calcPosition( mPosition, mSizeZ, mOrientation, toVector(0.f,0.f,-1.f) ) );
	pg.addToMeshBuilder( buffer );

	pg.mNumSegX = mNumSegY;
	pg.mNumSegY = mNumSegX;
	pg.mSizeX = mSizeY;
	pg.mSizeY = mSizeX;
	pg.mNormal = XMFLOAT3(.0f,.0f,1.0f);
	pg.setPosition( calcPosition( mPosition, mSizeZ, mOrientation, toVector(0.f,0.f,1.f) ) );
	pg.addToMeshBuilder( buffer );

	pg.mNumSegX = mNumSegZ;
	pg.mNumSegY = mNumSegX;
	pg.mSizeX = mSizeZ;
	pg.mSizeY = mSizeX;
	pg.mNormal = XMFLOAT3(.0f,-1.f,.0f);
	pg.setPosition( calcPosition( mPosition, mSizeY, mOrientation, toVector(0.f,-1.f,0.f) ) );
	pg.addToMeshBuilder( buffer );

	pg.mNumSegX = mNumSegZ;
	pg.mNumSegY = mNumSegX;
	pg.mSizeX = mSizeZ;
	pg.mSizeY = mSizeX;
	pg.mNormal = XMFLOAT3(0.f,1.f,0.f);
	pg.setPosition( calcPosition( mPosition, mSizeY, mOrientation, toVector(0.f,1.f,0.f) ) );
	pg.addToMeshBuilder( buffer );
	
	pg.mNumSegX = mNumSegZ;
	pg.mNumSegY = mNumSegY;
	pg.mSizeX = mSizeZ;
	pg.mSizeY = mSizeY;
	pg.mNormal = XMFLOAT3(-1.f,0.f,0.f);
	pg.setPosition( calcPosition( mPosition, mSizeX, mOrientation, toVector(-1.f,0.f,0.f) ) );
	pg.addToMeshBuilder( buffer );

	pg.mNumSegX = mNumSegZ;
	pg.mNumSegY = mNumSegY;
	pg.mSizeX = mSizeZ;
	pg.mSizeY = mSizeY;
	pg.mNormal = XMFLOAT3(1.f,0.f,0.f);
	pg.setPosition( calcPosition( mPosition, mSizeX, mOrientation, toVector(1.f,0.f,0.f) ) );
	pg.addToMeshBuilder( buffer );
}