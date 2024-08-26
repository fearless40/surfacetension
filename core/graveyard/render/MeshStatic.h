#pragma once

#include "MeshD3D.h"

struct ID3D11Buffer;
class MeshCPU;
class D3D11;
class Model;

class MeshStatic : public MeshD3D
{


public:
	MeshStatic(void);
	~MeshStatic(void);

	void createVerts( D3D11 * d3d, void * verts, unsigned int memSize  );
	void createIndexs( D3D11 * d3d, void * indexs, unsigned int memSize );
	void fromMeshCPU( D3D11 * d3d, RefCountPointer<MeshCPU> mesh );
};

