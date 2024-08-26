#pragma once
//Only contains static functions!

#include "../d3d11/VertexTypeDescription.h"

class Model;
class MeshCPU;
class D3D11;

class ModelBuilder
{
public:
	typedef std::vector< RefCountPointer<Model> > ModelVector;
	typedef std::vector< RefCountPointer<MeshCPU> > MeshCPUVector;


	static ModelHandle makeStaticGeometryModel( D3D11 & d3d, VertexTypeDescription vType, D3D11_PRIMITIVE_TOPOLOGY topology, RefCountPointer<MeshCPU> mesh);

	//static ModelVector makeStaticGeometryModelStream( D3D11 * d3d, VertexTypeDescription vType, D3D11_PRIMITIVE_TOPOLOGY topology, MeshCPUVector & items );
};