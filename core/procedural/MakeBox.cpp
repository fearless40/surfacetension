#include "../stdafx.h"
#include "../d3d11/VertexTypeManager.h"
#include "../render/MeshCPU.h"
#include <DirectXMath.h>

using namespace DirectX;

namespace Procedural {
RefCountPointer<MeshCPU> MakeBox() 
{
	RefCountPointer<MeshVertexCPU<VertexTypeManager::VertexPosColor> > mesh( new MeshVertexCPU<VertexTypeManager::VertexPosColor> );
	
	mesh->allocateNbrVerts(8);
	mesh->allocateNbrIndexs(36);


	VertexTypeManager::VertexPosColor vertices[] =
    {
        { XMFLOAT4( -1.0f,  1.0f, -1.0f, 1.0f ), XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f ) },
        { XMFLOAT4(  1.0f,  1.0f, -1.0f, 1.0f ), XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ) },
        { XMFLOAT4(  1.0f,  1.0f,  1.0f, 1.0f ), XMFLOAT4( 0.0f, 1.0f, 1.0f, 1.0f ) },
        { XMFLOAT4( -1.0f,  1.0f,  1.0f, 1.0f ), XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) },
        { XMFLOAT4( -1.0f, -1.0f, -1.0f, 1.0f ), XMFLOAT4( 1.0f, 0.0f, 1.0f, 1.0f ) },
        { XMFLOAT4(  1.0f, -1.0f, -1.0f, 1.0f ), XMFLOAT4( 1.0f, 1.0f, 0.0f, 1.0f ) },
        { XMFLOAT4(  1.0f, -1.0f,  1.0f, 1.0f ), XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) },
        { XMFLOAT4( -1.0f, -1.0f,  1.0f, 1.0f ), XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ) },
    };
		
	MeshCPU::IndexType indices[] =
    {
        3,1,0,
        2,1,3,

        0,5,4,
        1,5,0,

        3,4,7,
        0,4,3,

        1,6,5,
        2,6,1,

        2,7,6,
        3,7,2,

        6,4,5,
        7,4,6,
    };
	
	

	unsigned long nbrItems;
	VertexTypeManager::VertexPosColor * verts = mesh->lockVertsType( nbrItems );
	
	memcpy( verts, vertices, sizeof( VertexTypeManager::VertexPosColor ) * 8 );
	
	mesh->unlockVerts();

	MeshCPU::IndexType * in = mesh->lockIndexType( nbrItems );
	
	memcpy( in, indices, sizeof(MeshCPU::IndexType) * nbrItems );

	mesh->unlockIndex();

	return RefCountPointer<MeshCPU> (mesh->toMeshCPU());
}

RefCountPointer<MeshCPU> MakePlane() 
{
	RefCountPointer<MeshVertexCPU<VertexTypeManager::VertexPosColor> > mesh( new MeshVertexCPU<VertexTypeManager::VertexPosColor> );
	
	mesh->allocateNbrVerts(4);
	mesh->allocateNbrIndexs(6);

		
	unsigned long nbrItems;
	VertexTypeManager::VertexPosColor * verts = mesh->lockVertsType( nbrItems );
	verts[0].position = DirectX::XMFLOAT4( -0.5f, -0.5f, 0.5f, 1.0f );
	verts[0].color= DirectX::XMFLOAT4( 1,0,0,1);
	//verts[0].normal = DirectX::XMFLOAT4( 0,1,0,1);
	//verts[0].uv = DirectX::XMFLOAT2( 0,0 );

	verts[1].position = DirectX::XMFLOAT4( -0.5f, 0.5f, 0.5f, 1.0f );
	verts[1].color= DirectX::XMFLOAT4( 0,1,0,1);
	//verts[1].normal = DirectX::XMFLOAT4( 1,0,0,1);
	//verts[1].uv = DirectX::XMFLOAT2( 1,0 );

	verts[2].position = DirectX::XMFLOAT4( 0.5f, 0.5f, 0.5f, 1.0f ); 
	verts[2].color= DirectX::XMFLOAT4( 0,0,1,1);
	//verts[2].normal = DirectX::XMFLOAT4( 0,1,1,1);
	//verts[2].uv = DirectX::XMFLOAT2( 1,1 );

	verts[3].position = DirectX::XMFLOAT4( 0.5f, -0.5f, 0.5f, 1.0f);
	verts[3].color= DirectX::XMFLOAT4( 1,1,1,1);
	//verts[3].normal = DirectX::XMFLOAT4( 1,0,1,1);
	//verts[3].uv = DirectX::XMFLOAT2( 0,1 );
	mesh->unlockVerts();

	MeshCPU::IndexType * in = mesh->lockIndexType( nbrItems );
	in[0] = 0;
	in[1] = 1;
	in[2] = 2;
	in[3] = 0;
	in[4] = 2;
	in[5] = 3;
	mesh->unlockIndex();

	return RefCountPointer<MeshCPU> (mesh->toMeshCPU());
}
}