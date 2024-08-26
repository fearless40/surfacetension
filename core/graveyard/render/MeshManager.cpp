#include "../stdafx.h"
#include <d3d11.h>
#include "../d3d11/D3D11.h"
#include "../d3d11/VertexBufferManager.h"
#include "../d3d11/IndexBufferManager.h"
#include "../d3d11/VertexTypeManager.h"
#include "MeshManager.h"

typedef uint32_t STRIDE;
typedef uint32_t VOFFSET;

struct MeshHandleHolder
{
	VertexBufferHandle			mVertexBuffers[Mesh::MAX_NUMBER_OF_BUFFERS];
	IndexBufferHandle			mIndexBuffer;	
};

struct Tag
{
	uint32_t tag;
	MeshHandle handle;
};

// MeshCaches are related on a one to one basis to Meshes. Therefore if referring to 
// MesheHandles[0] then MeshCaches[0] will refer to its cached representation

std::vector< GenericHandleManager::ItemDescription< MeshCache > > Meshes;
std::vector< MeshHandleHolder > MeshHandleHolders;
std::vector<uint32_t> FreeList;
std::vector< Tag > Tags;




void toMesh( Mesh & mesh, const MeshHandleHolder & holder, const MeshCache & cache )
{
	mesh.mIndexBuffer = holder.mIndexBuffer;
	memcpy( mesh.mVertexBuffers, holder.mVertexBuffers, sizeof( VertexBufferHandle ) * Mesh::MAX_NUMBER_OF_BUFFERS );

	mesh.mTopology = cache.mTopology;
	mesh.mLayout = cache.mLayout;
	memcpy(mesh.mStrides, cache.mStrides, sizeof( STRIDE ) * Mesh::MAX_NUMBER_OF_BUFFERS );
	memcpy(mesh.mVOffset, cache.mVOffset, sizeof( VOFFSET ) * Mesh::MAX_NUMBER_OF_BUFFERS );
}

void splitMesh( const Mesh & mesh, MeshHandleHolder & holder, MeshCache & cache )
{
	holder.mIndexBuffer = mesh.mIndexBuffer;
	memcpy( holder.mVertexBuffers, mesh.mVertexBuffers, sizeof( VertexBufferHandle ) * Mesh::MAX_NUMBER_OF_BUFFERS );

	cache.mTopology = mesh.mTopology;
	cache.mLayout = mesh.mLayout;
	memcpy(cache.mStrides, mesh.mStrides, sizeof( STRIDE ) * Mesh::MAX_NUMBER_OF_BUFFERS );
	memcpy(cache.mVOffset, mesh.mVOffset, sizeof( VOFFSET ) * Mesh::MAX_NUMBER_OF_BUFFERS );
}

void freeMesh( MeshHandle mesh )
{
	GenericHandleManager::deleteHandle( mesh, Meshes, FreeList );
}

void makeCache( const MeshHandleHolder & holder, MeshCache & cache )
{
	cache.mBuffers[MeshCache::INDEX_BUFFER_POSITION] = IndexBufferManager::fromHandle( holder.mIndexBuffer );

	// Create the cache item already
	for( uint32_t buffernbr = 0; buffernbr < Mesh::MAX_NUMBER_OF_BUFFERS; ++buffernbr )
	{
		if( holder.mVertexBuffers[buffernbr].isNull() )
		{
			cache.mBuffers[MeshCache::VERTEX_BUFFER_POSITION + buffernbr] = nullptr;
			cache.mNbrOfVertexBuffers = buffernbr;
			break;
		}
		else 
		{
			cache.mBuffers[MeshCache::VERTEX_BUFFER_POSITION + buffernbr] = VertexBufferManager::fromHandle( holder.mVertexBuffers[buffernbr] );
		}
	}
}

void MeshManager::rebuildCache() 
{
	for( uint32_t meshnbr = 0; meshnbr < Meshes.size(); ++meshnbr )
	{
		MeshCache & cache = Meshes[meshnbr].mValue;
		MeshHandleHolder & holder = MeshHandleHolders[meshnbr];
		makeCache( holder, cache );
	}
}



MeshHandle MeshManager::newMesh( const Mesh & mesh, uint32_t tag )
{
	MeshHandleHolder meshhandle;
	MeshCache  meshcache;

	splitMesh( mesh, meshhandle, meshcache );
	makeCache( meshhandle, meshcache );

	// Need to store the handle value to sync the two vectors
	MeshHandle handle = GenericHandleManager::newHandle<MeshHandle, MeshCache>( meshcache, Meshes, FreeList );
	if( handle.mIndex > MeshHandleHolders.size() )
	{
		MeshHandleHolders.push_back( meshhandle );
	}
	else
	{
		MeshHandleHolders[handle.mIndex-1] = meshhandle;
	}

	if( tag != 0 )
	{
		Tag tagItem = {tag, handle};
		Tags.push_back( tagItem );
	}

	return handle;
}

void MeshManager::editMesh( MeshHandle handle, const Mesh & mesh )
{
#ifdef _DEBUG
	if( isValidHandle( handle ) == false )
	{
		//Todo: alert
		return;
	}
#endif
	MeshHandleHolder & holder = MeshHandleHolders[handle.mIndex-1];
	MeshCache  & cache = Meshes[handle.mIndex-1].mValue;

	splitMesh( mesh, holder, cache );
	makeCache( holder, cache );
}

Mesh MeshManager::fromHandle( MeshHandle handle )
{
#ifdef _DEBUG
	if( isValidHandle( handle ) == false )
	{
		//Todo: alert
		Mesh retValue;
		memset( &retValue, 0, sizeof( Mesh ) );
		return retValue;
	}
#endif
	
	Mesh mesh;
	MeshHandleHolder & holder = MeshHandleHolders[handle.mIndex-1];
	MeshCache & cache = Meshes[handle.mIndex-1].mValue;
	
	toMesh( mesh, holder, cache );
	
	return mesh;
}

MeshCache & MeshManager::getCache( MeshHandle handle )
{
	return Meshes[handle.mIndex-1].mValue;
}

bool MeshManager::isValidHandle( MeshHandle handle )
{
	return GenericHandleManager::isValidHandle( handle, Meshes );
}

void MeshManager::deleteMesh( MeshHandle handle )
{
	// No other clean up needed
	freeMesh( handle );
}


void MeshManager::deleteAll()
{
	// invalidate all the handles
	for( auto & item : Meshes )
	{
		++item.mGeneration;
	}
	
	Meshes.clear();
	FreeList.clear();
}

void MeshManager::deleteMeshWithTag( uint32_t tag )
{
	std::vector< Tag > positiveTags;
	
	for( auto & tagItem : Tags )
	{
		if( tagItem.tag == tag )
		{
			freeMesh( tagItem.handle );
		}
		else
		{
			positiveTags.push_back( tagItem );
		}
	}

	Tags = std::move(positiveTags);
}

ID3D11Buffer * MeshManager::getIndexBuffer( MeshHandle mesh )
{
	#ifdef _DEBUG
	if( isValidHandle( mesh ) == false )
	{
		//Todo: alert
		return nullptr;
	}
#endif
	
	return Meshes[mesh.mIndex-1].mValue.mBuffers[MeshCache::INDEX_BUFFER_POSITION];
}


ID3D11Buffer ** MeshManager::getVertexBuffers( MeshHandle mesh )
{
#ifdef _DEBUG
	if( isValidHandle( mesh ) == false )
	{
		//Todo: alert
		return nullptr;
	}
#endif
	
	return &Meshes[mesh.mIndex-1].mValue.mBuffers[MeshCache::VERTEX_BUFFER_POSITION];
}


uint32_t MeshManager::getNbrVertexBuffers( MeshHandle mesh )
{
#ifdef _DEBUG
	if( isValidHandle( mesh ) == false )
	{
		//Todo: alert
		return 0;
	}
#endif
	
	return Meshes[mesh.mIndex-1].mValue.mNbrOfVertexBuffers;
}