#include "../stdafx.h"
#include "MeshCPU.h"


MeshCPU::MeshCPU(void)
{
	mVerts = nullptr;
	mIndex = nullptr;

	mVertsSize = 0;
	mIndexSize = 0;


}


MeshCPU::~MeshCPU(void)
{
	delete [] mVerts;
	delete [] mIndex;

	mVerts = nullptr;
	mIndex = nullptr;

	mVertsSize = 0;
	mIndexSize = 0;
}

void MeshCPU::allocateVerts( ulong memSize )
{
	mVerts = new byte[memSize];
	if( mVerts )
		mVertsSize = memSize;
}

void MeshCPU::allocateIndex( ulong memSize )
{
	mIndex = (void*) new byte[memSize];
	if( mIndex )
		mIndexSize = memSize;
}

void MeshCPU::lockVerts( void ** verts, ulong & memSize)
{
	if( mVerts )
	{
		*verts = mVerts;
		memSize = mVertsSize;
	}
	else
	{
		memSize = 0;
		*verts = nullptr;
	}
}

	
void MeshCPU::unlockVerts()
{
	// Do nothing for now
}
	
void MeshCPU::lockIndex( void ** index, ulong & memSize)
{
	if( mIndex )
	{
		*index = mIndex;
		memSize = mIndexSize;
	}
	else
	{
		memSize = 0;
		*index = nullptr;
	}
}
	
void MeshCPU::unlockIndex()
{
	// Do nothing for now
}

