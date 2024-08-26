#include "../stdafx.h"
#include <d3d11.h>
#include "../d3d11/D3D11.h"
#include "../d3d11/VertexTypeDescription.h"
#include "MeshCPU.h"
#include "../d3d11/Mesh.h"
#include "Model.h"
#include "ModelBuilder.h"
#include "ModelManager.h"
#include "MeshManager.h"
#include "../d3d11/VertexBufferManager.h"
#include "../d3d11/IndexBufferManager.h"



const unsigned long MAX_VBUFFER_SIZE_IN_BYTES = 16000000; //16 MB (can be bigger up to 128 MB according to direct3d however for simplicity will do this.



ModelHandle ModelBuilder::makeStaticGeometryModel( D3D11 & d3d, VertexTypeDescription vType, D3D11_PRIMITIVE_TOPOLOGY topology, RefCountPointer<MeshCPU> mesh)
{
	Mesh d3dMesh;
	Model retModel;

	memset( &d3dMesh, 0, sizeof(Mesh) );

	unsigned long memSize = 0;
	void * mem = nullptr;

	mesh->lockVerts( &mem, memSize );

		d3dMesh.mVertexBuffers[0] = VertexBufferManager::newImmutable( d3d, mem, memSize );
		
	mesh->unlockVerts();

	mem = nullptr; memSize = 0;

	mesh->lockIndex( &mem, memSize );

		d3dMesh.mIndexBuffer =  IndexBufferManager::newImmutable( d3d, mem, memSize );

	mesh->unlockVerts();


	d3dMesh.mTopology = topology;
	d3dMesh.mLayout = vType.layout;
	d3dMesh.mVOffset[0] = 0;
	d3dMesh.mStrides[0] = vType.stride;


	retModel.mMesh = MeshManager::newMesh(d3dMesh);
	retModel.mIOffset = 0;
	retModel.mINumber = memSize / MeshCPU::IndexSizeInBytes;
	retModel.mIVertexOffset = 0;
	
	return ModelManager::newModel( retModel );
}

/*
ModelBuilder::ModelVector ModelBuilder::makeStaticGeometryModelStream( D3D11 * d3d, VertexTypeDescription vType, D3D11_PRIMITIVE_TOPOLOGY topology, MeshCPUVector & items )
{
	struct TempMemory {
		unsigned char * mMem;
		unsigned long mSize;

		void allocate( unsigned long size )
		{
			mMem = new unsigned char[ size ];
			mSize = size;
		}
		
		~TempMemory() {
			if( mMem ) 
				delete [] mMem;
		}
		TempMemory(unsigned long size) {
			allocate(size);
		};

		TempMemory( TempMemory && tm )
		{
			mMem = tm.mMem;
			mSize = tm.mSize;
			tm.mMem = nullptr;
			tm.mSize = 0;
		}
	};

	std::vector<TempMemory> vectorMem;
	std::vector<TempMemory> indexMem;
	ModelVector models;
	MeshCPUVector::iterator end = items.end();
	unsigned long vTotalSize = 0;
	unsigned long vSize = 0;

	unsigned long iTotalSize = 0;
	unsigned long iSize = 0;

	RefCountPointer<MeshD3D> d3dMesh;

	// Make the memory allocation less jumpy
	models.reserve( items.size() );

	for( MeshCPUVector::iterator loop = items.begin(); loop != end; ++loop )
	{
		vSize = (*loop)->getVertexBufferSize();
		iSize = (*loop)->getIndexBufferSize();

		if( (vTotalSize + vSize > MAX_VBUFFER_SIZE_IN_BYTES) || ( iTotalSize + iSize > MAX_VBUFFER_SIZE_IN_BYTES) )
		{
			// Allocate the memory for the verts
			vectorMem.push_back( TempMemory(vTotalSize) );
			indexMem.push_back( TempMemory(iTotalSize) );
			vTotalSize = 0;
			iTotalSize = 0;
		}
		vTotalSize += vSize;
		iTotalSize += iSize;

		// Allocate all the models at the same time
		models.push_back( RefCountPointer<Model>( new Model(topology, vType) ) );
	}

	// Add the last entries into the memory used
	vectorMem.push_back( TempMemory(vTotalSize) );
	indexMem.push_back( TempMemory(iTotalSize) );

	// Now all contingous ram is allocated
	// Now we have to run the loop all over again...

	ModelVector::iterator model = models.begin();
	Model * m;
	void * vRaw, * iRaw;
	vTotalSize = iTotalSize = vSize = iSize = 0;
	std::vector<TempMemory>::iterator vCurrentRamBlock = vectorMem.begin();
	std::vector<TempMemory>::iterator iCurrentRamBlock = indexMem.begin();
	ID3D11Buffer * iBuffer = nullptr;
	d3dMesh = new MeshD3D();
	for( MeshCPUVector::iterator loop = items.begin(); loop != end; ++loop )
	{
		// For easy of typing
		m = (*model).get();
		(*loop)->lockVerts( &vRaw, vSize );
		(*loop)->lockIndex( &iRaw, iSize );

		if( (vTotalSize + vSize > MAX_VBUFFER_SIZE_IN_BYTES) || (iTotalSize + iSize > MAX_VBUFFER_SIZE_IN_BYTES) )
		{
			// Set the mesh buffer
			d3dMesh->setVBuffer( BufferHelpers::createVBImmutable(d3d, vCurrentRamBlock->mMem, vCurrentRamBlock->mSize ) );
			d3dMesh->setIBuffer( BufferHelpers::createIBImmutable(d3d, iCurrentRamBlock->mMem, iCurrentRamBlock->mSize ) );
			++vCurrentRamBlock;
			vTotalSize = 0;
			++iCurrentRamBlock;
			iTotalSize = 0;
			// Allocate a new mesh object
			d3dMesh = new MeshD3D();
		}
		m->mIOffset = iTotalSize;
		m->mINumber = iSize / MeshCPU::IndexSizeInBytes;
		memcpy( iCurrentRamBlock->mMem + iTotalSize, iRaw, iSize );
		iTotalSize += iSize;


		m->mVOffset = 0; //vTotalSize;
		m->mIVertexOffset = vTotalSize / vType.stride;
		m->mMesh = d3dMesh;
		memcpy( vCurrentRamBlock->mMem + vTotalSize, vRaw, vSize );
		vTotalSize += vSize;

		(*loop)->unlockVerts();
		(*loop)->unlockIndex();

		++model;

	}

	// Set the mesh buffer
	d3dMesh->setVBuffer( BufferHelpers::createVBStatic(d3d, vCurrentRamBlock->mMem, vCurrentRamBlock->mSize ) );
	d3dMesh->setIBuffer( BufferHelpers::createIBStatic(d3d, iCurrentRamBlock->mMem, iCurrentRamBlock->mSize ) );

	return models;
}

*/
