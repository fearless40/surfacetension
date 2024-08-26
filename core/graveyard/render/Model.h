#pragma once

#include "../d3d11/Handles.h"
#include "../d3d11/VertexTypeDescription.h"

class RenderQue;
class MeshCPU;
class ModelBuilder;
class D3D11;


class Model
{
public:
	struct Flags 
	{
		uint32_t isInstanceMesh : 1;
		uint32_t isProgressive	: 1;
		uint32_t isTerrain		: 1;
		uint32_t isDynamic		: 1;
	};

public:
	// The following items we don't want the user messing with unless they are creating the Model
	// The models will be created by the content pipeline rather than by a user so always hide it.
	
	// It works something like this:
	// Vertex[mVOffset (in bytes) + IndexBuffer[mIOffset]+ mIVertexOffset] is the first vertex in the list
	// Allows packing of more than one mesh in one buffer

	unsigned int mIOffset;		// Start Indecies position in the IndexBuffer
	unsigned int mINumber;		// Number of indecies to read
	unsigned int mIVertexOffset;// At what position do the verts actually start
	
	MeshHandle				mMesh;
	
public:
	// The user can change the following whenever they want this allows them to make a model look different
	// on a per instance basis


	//ConstantBufferArrayHandle	mConstantBuffers;

	Flags						mFlags;
	VertexShaderHandle			mVSShader;

public:
	Model();
	~Model();


#ifdef _DEBUG
	void render(D3D11 * d3d);
#endif

//friend RenderQue;
//friend ModelBuilder;
};

/*
class ModelHelper 
{
protected:
	ModelHandle		mHandle;
	Model		 &	mData;

public:
	ModelHandle clone();


};
*/






