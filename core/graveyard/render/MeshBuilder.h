#pragma once

#include "MeshCPU.h"

template <typename VertexType, typename IndexType>
class MeshBuilder
{
public:
	typedef VertexType vtype;
	typedef IndexType itype;

	typedef std::vector<vtype> VertArray;
	typedef std::vector<itype> IndexArray;

	void estimateVertexCount( int nbrVerts ) {
		mEstimatedVertexCount += nbrVerts;
		mVerts.reserve( mEstimatedVertexCount );
	}

	void estimateIndexCount( int nbrIndexs ) {
		mEstimatedIndexCount += nbrIndexs;
		mIndexs.reserve( mEstimatedIndexCount );
	}

	vtype & newVert() {
		vtype vert;
		mVerts.push_back(vert);
		return mVerts.back();
	}

	MeshBuilder<vtype,itype> & addVert( const vtype & vert ) {
		mVerts.push_back( vert );
		return *this;
	}

	
	MeshBuilder<vtype,itype> & addIndex( itype i ) {
		mIndexs.push_back(i + mRebaseOffset );
		return *this;
	}

	MeshBuilder<vtype,itype> & index( itype i ) {
		mIndexs.push_back( i + mRebaseOffset );
		return *this;
	}

	void rebaseOffset() {
		mRebaseOffset = mVerts.size();
	}
	

	void clear() {
		mVerts.clear();
		mIndexs.clear();
		mEstimatedVertexCount = mEstimatedIndexCount = 	mRebaseOffset = 0;
	}

	VertArray & getVerts() { return mVerts; }
	IndexArray & getIndexs() { return mIndexs; }

	RefCountPointer<MeshVertexCPU<vtype> >  toMeshVertexCPU() {
		MeshVertexCPU<vtype> * mesh = new MeshVertexCPU<vtype>;
		mesh->allocateNbrVerts( mVerts.size() );
		mesh->allocateNbrIndexs( mIndexs.size() );

		MeshCPU::ulong nbrVerts = 0;

		vtype * verts = mesh->lockVertsType(nbrVerts);

		for( MeshCPU::ulong i = 0; i < nbrVerts; ++i )
		{
			verts[i] = mVerts[i];
			
		};
		mesh->unlockVerts();

		MeshCPU::ulong nbrIndex = 0;
		MeshCPU::IndexType * indexs = mesh->lockIndexType(nbrIndex);
		for( MeshCPU::ulong i = 0; i < nbrIndex; ++i )
		{
			indexs[i] = mIndexs[i];
		}
		mesh->unlockIndex();

		return RefCountPointer<MeshVertexCPU<vtype> > (mesh);

	}


	MeshBuilder(void) : mEstimatedIndexCount(0), mEstimatedVertexCount(0), mRebaseOffset(0) { }
	~MeshBuilder(void) { }

protected:
	VertArray mVerts;
	IndexArray mIndexs;

	unsigned int mEstimatedVertexCount;
	unsigned int mEstimatedIndexCount;

	unsigned int mRebaseOffset;

};

