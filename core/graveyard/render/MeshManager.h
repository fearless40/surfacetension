#pragma once

#include "../d3d11/Handles.h"
#include "../util/GenericHandleManager.h"
#include "../d3d11/Mesh.h"

struct MeshManager
{
	static MeshHandle newMesh( const Mesh & mesh, uint32_t tag = 0 );

	static Mesh fromHandle( MeshHandle handle );

	static MeshCache & getCache( MeshHandle handle );

	static void editMesh( MeshHandle handle, const Mesh & mesh );

	static bool isValidHandle( MeshHandle handle );

	static void deleteMesh( MeshHandle handle );

	static void deleteAll();

	static void deleteMeshWithTag( uint32_t tag );

	static ID3D11Buffer * getIndexBuffer( MeshHandle mesh );

	static ID3D11Buffer ** getVertexBuffers( MeshHandle mesh );

	static uint32_t getNbrVertexBuffers( MeshHandle mesh );

	//static void registerHandleInvalidator( HandleInvalidationCallback callback );
private:
	static void rebuildCache();
};



/*
Mesh Struct <-- Owned by manager
Model Struct <-- Owned by manager
BuuferArray <-- Owned by manager

Mesh are only access through models
IF you change a mesh it will effect all models looking at it


Mesh holds basic gemoetry info
Model holds info to render geometry
Material holds color info
RenderableItem holds model, material and instance information such as position and stuff

RenderableItem simple array of structs
Uses a free list
Needs to be fast to be sorted
Can have multiple buffers of RenderableItems running around
*/