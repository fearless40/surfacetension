#pragma once

class Model;
class Material;
class RenderQue;

#include "Render.h"
#include <utility>
/*
struct RenderableInstance
{
public:
	RenderableInstance(void) : mKey( EMPTY_KEY ) { }

	RenderableInstance( const RenderableInstance & ri ) :
		mModel( ri.mModel ),
		mMaterial( ri.mMaterial ),
		vsConstants( ri.vsConstants ),
		psConstants( ri.psConstants )
	{	}

	RenderableInstance( RenderableInstance && ri ) : 
		mModel( std::move(ri.mModel) ),
		mMaterial( std::move(ri.mMaterial) ),
		vsConstants( std::move(ri.vsConstants) ),
		psConstants( std::move(ri.psConstants) )
	{
		//mModel = ri.mModel; 
		//mMaterial = ri.mMaterial;
		//vsConstants = ri.vsConstants;
		//psConstants = ri.psConstants;
	}

	~RenderableInstance(void) { }

	RefCountPointer< Model > & getModel() { return mModel; }
	RefCountPointer< Material > & getMaterial() { return mMaterial; }

	RefCountPointer<ConstantBufferArray> & getVSConstantBuffers() { return vsConstants; }
	RefCountPointer<ConstantBufferArray> & getPSConstantBuffers() { return psConstants; }


	RefCountPointer< Model > mModel;
	RefCountPointer< Material > mMaterial;

	RefCountPointer<ConstantBufferArray> vsConstants;
	RefCountPointer<ConstantBufferArray> psConstants;
	
	SortingKey mKey;

	friend RenderQue;
};
*/


struct RenderableInstance
{
public:
	RenderableInstance(void) : mKey( EMPTY_KEY ) { }

	ModelHandle			mModel;
	MaterialHandle		mMaterial;

	BufferArrayHandle	mVSConstants;
	BufferArrayHandle	mPSConstants;
	
	SortingKey mKey;

};
/*
struct Renderable
{
	ModelHandle			mModel;
	MaterialHandle		mMaterial;
};

struct RenderableItem 
{
	ModelHandle			mModel;
	MaterialHandle		mMaterial;

	BufferArrayHandle	mVSConstants;
	BufferArrayHandle	mPSConstants;
};

struct RenderableInstance
{
	uint32_t			mNumberItems;
	VertexBufferHandle  mInstanceData;
	RenderableItem		mItem;
};
*/