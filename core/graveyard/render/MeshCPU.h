#pragma once

#include <vector>
#include "../util/InvasiveRefCount.h"
#include "../util/RefCountPointer.h"

// Very basic conterpart to a d3d vertex buffer and index buffer
// Only kept in the CPU memory

class MeshCPU : public InvasiveRefCount
{

	
public:
	typedef unsigned long	ulong;
	typedef unsigned char	byte;
	typedef unsigned __int32 IndexType;
	static const unsigned int IndexSizeInBytes = sizeof(IndexType);
	

	MeshCPU(void);
	~MeshCPU(void);


	void allocateVerts( ulong memSize );
	void allocateIndex( ulong nbrItems );

	void lockVerts( void ** verts, ulong & memSize);
	void unlockVerts();
	
	void lockIndex( void ** index, ulong & memSize);
	void unlockIndex();

	ulong getVertexBufferSize() { return mVertsSize; }
	ulong getIndexBufferSize() { return mIndexSize; }

protected:



	

	void	  *	mVerts;
	void	  * mIndex;

	ulong mVertsSize;
	ulong mIndexSize;
};

typedef RefCountPointer<MeshCPU> MeshCPUPtr;

template< typename VertexType >
class MeshVertexCPU : public MeshCPU
{
public:
	
	void allocateNbrVerts( ulong nbrVerts ) 
	{
		allocateVerts( nbrVerts * sizeof( VertexType ) );
	}

	void allocateNbrIndexs( ulong nbrIndex )
	{
		allocateIndex( IndexSizeInBytes * nbrIndex );
	}

	VertexType * lockVertsType( ulong & nbrVerts ) 
	{
		ulong mSize = 0;
		VertexType * vt = nullptr;

		lockVerts( reinterpret_cast<void**>(&vt), mSize );

		nbrVerts = mSize / sizeof(VertexType);

		return vt;
	}

	IndexType * lockIndexType( ulong & nbrIndex )
	{
		ulong mSize = 0;
		IndexType * index = nullptr;

		lockIndex( reinterpret_cast<void**>(&index), mSize );

		nbrIndex = mSize / IndexSizeInBytes;

		return index;
	}

	MeshCPU * toMeshCPU() {
		return static_cast<MeshCPU*>(this);
	}
};
