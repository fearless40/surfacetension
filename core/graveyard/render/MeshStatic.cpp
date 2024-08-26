#include "../stdafx.h"
#include <d3d11.h>
#include "../d3d11/Mesh.h"
#include "MeshStatic.h"
#include "../d3d11/D3D11.h"
#include "MeshCPU.h"
#include "../d3d11/BufferManagerHelpers.h"

MeshStatic::MeshStatic(void)
{

}


MeshStatic::~MeshStatic(void)
{
	
}



void MeshStatic::createVerts( D3D11 * d3d, void * verts, unsigned int memSize  )
{
	setVBuffer( BufferHelpers::createVBStatic(d3d,verts, memSize) );
}

void MeshStatic::createIndexs( D3D11 * d3d, void * indexs, unsigned int memSize )
{
	setIBuffer( BufferHelpers::createIBStatic(d3d, indexs, memSize) );
}

void MeshStatic::fromMeshCPU( D3D11 * d3d, RefCountPointer<MeshCPU> mesh )
{
	void * verts, * index;
	unsigned long vSize, iSize;

	mesh->lockVerts( &verts, vSize );
	if( verts != nullptr && vSize > 0 )
	{
		createVerts( d3d, verts, vSize );
	}
	mesh->unlockVerts();

	mesh->lockIndex( &index, iSize );
	if( index != nullptr && iSize > 0 )
	{
		createIndexs( d3d, index, iSize );
	}

	return;
}
	

