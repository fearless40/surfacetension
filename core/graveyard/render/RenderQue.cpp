#include "../stdafx.h"
#include <algorithm>
#include <d3d11.h>
#include "../d3d11/D3D11.h"
#include "../d3d11/VertexTypeDescription.h"
#include "../d3d11/Mesh.h"
#include "Model.h"
#include "Material.h"
#include "RenderQue.h"
#include "ModelManager.h"
#include "MeshManager.h"
#include "../d3d11/VertexShaderManager.h"
#include "../d3d11/IndexBufferManager.h"
#include "../d3d11/ConstantBufferManager.h"
#include "../d3d11/ConstantBufferArrayManager.h"

const int MAX_NUMBER_BUFFERS = 14;

struct StateCache {
	// I purposely did not point refcounted items in this structure
	VertexShaderHandle	 vsCurrent;
	ID3D11Buffer *  vertexBufferCurrent;
	ID3D11Buffer *   indexBufferCurrent;
	ID3D11Buffer * vertexConstantBuffers[MAX_NUMBER_BUFFERS];
	D3D11_PRIMITIVE_TOPOLOGY	topology;
	ID3D11InputLayout *			layout;
	unsigned int		vertexBufferOffset;
	unsigned int		indexBufferOffset;
};

RenderQue::RenderQue(void) : 
	mCache( new StateCache ),
	mDevice( nullptr ),
	mRenderTarget( nullptr ),
	mDepthStencil( nullptr )
{
}


RenderQue::~RenderQue(void)
{
	// Remove cache memory
	delete mCache;

	if( mDevice )
		mDevice->Release();

	if( mRenderTarget )
		mRenderTarget->Release();

	if( mDepthStencil )
		mDepthStencil->Release();

	mRenderTarget = nullptr;
	mDepthStencil = nullptr;
}


void RenderQue::initalize( D3D11 & d3d ) 
{
	HRESULT hr;

	if( mRenderTarget )
		mRenderTarget->Release();

	if( mDepthStencil )
		mDepthStencil->Release();
	
	mDevice = d3d.getDeviceContext();
	//mDevice->AddRef();
	
	/*hr = d3d.getDevice()->CreateDeferredContext( 0, &mDevice );

	if( FAILED(hr) ) 
	{
		//Todo: Put some notification in here...
		return;
	}*/
	
	// Now grab the default renderTarget and depthStencil
	unsigned int nbrItems = 1;
	ID3D11RenderTargetView * rtArray[1] ;
	ID3D11DepthStencilView * dsArray ;

	d3d.getDeviceContext()->OMGetRenderTargets( nbrItems, rtArray, &dsArray );

	if( rtArray ) 
	{
		mRenderTarget = rtArray[0];
	}

	if( dsArray )
	{
		mDepthStencil = dsArray;
	}
	else
	{
		mDepthStencil = d3d.getDepthStencilView();
	}

}


void RenderQue::begin()
{
	// We always assume the device is in a blank state when we start
	// Can change in the future but should be good enough now

	//mDevice->OMSetRenderTargets(1, &mRenderTarget, mDepthStencil );

	//Clear the cache (currently it is emtpy)
	memset(mCache,0, sizeof(StateCache) ); 

	// Now clear the lists but don't free the memory
	mItems.clear();
	mItemsInstanced.clear();
}

void RenderQue::clearRenderTargerts( )
{
	float color[4] = {0.0f,0.0f,0.0f,0.0f};
	
	// Clear the back buffer.
	mDevice->ClearRenderTargetView(mRenderTarget, color);
    
	// Clear the depth buffer.
	mDevice->ClearDepthStencilView(mDepthStencil, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


void RenderQue::addItem( RenderableInstance item )
{
	mItems.push_back( item );
}

void RenderQue::addItemInstanced( RenderableInstance item  )
{
	mItemsInstanced.push_back( item );
}

void RenderQue::setVertexShaderConstantBuffer( ConstantBufferArrayHandle bufh ) 
{
		if( bufh.isNull() )
			return;

		ConstantBufferArray buf(bufh);

		mDevice->VSSetConstantBuffers( buf.getStartBindLocation(), buf.nbrBuffers(), buf);
}

void RenderQue::setVertexShaderConstantBuffer( ConstantBufferHandle buf, uint32_t pos )
{
	if( buf.isNull() )
		return;

	ID3D11Buffer * buft = ConstantBufferManager::fromHandle( buf );

	mDevice->VSSetConstantBuffers( pos, 1, &buft );
}

void RenderQue::setPixelShaderConstantBuffer( BufferArrayHandle bufh ) 
{
		if( bufh.isNull() )
			return;

		ConstantBufferArray buf(bufh);

		mDevice->PSSetConstantBuffers( buf.getStartBindLocation(), buf.nbrBuffers(), buf );
}

void RenderQue::setPixelShaderConstantBuffer( ConstantBufferHandle buf, uint32_t pos )
{
	if( buf.isNull() )
		return;

	ID3D11Buffer * buft = ConstantBufferManager::fromHandle( buf );

	mDevice->PSSetConstantBuffers( pos, 1, &buft );
}

void RenderQue::end()
{
	// Sort the item ques
	sortAll();

	// Now loop through each item and add it the deferred context with the appropiate bindings and what not

	//Currently ignoring everything that has to do with material like properties
	ItemsVector::iterator end = mItems.end();
	ItemsVector::iterator loop = mItems.begin();
	

	for( ; loop != end; ++loop )
	{
		// Compare the items in the loop to what the cached item is
		Model & model = ModelManager::fromHandle( loop->mModel );
		MeshCache & mesh = MeshManager::getCache( model.mMesh );
		
		if( mCache->layout != mesh.mLayout )
		{
			mCache->layout = mesh.mLayout;
			mDevice->IASetInputLayout( mCache->layout );
		}

		if( mCache->topology != mesh.mTopology )
		{
			mCache->topology = mesh.mTopology;
			mDevice->IASetPrimitiveTopology( mCache->topology);
		}

		if( mCache->vsCurrent != model.mVSShader )
		{
			mCache->vsCurrent = model.mVSShader ;
			mDevice->VSSetShader( VertexShaderManager::fromHandle( mCache->vsCurrent ), NULL, 0 );
		}

		if( mCache->vertexBufferCurrent != mesh.mBuffers[MeshCache::VERTEX_BUFFER_POSITION] ||
			mCache->vertexBufferOffset != mesh.mVOffset[0])
		{
			mCache->vertexBufferCurrent = mesh.mBuffers[MeshCache::VERTEX_BUFFER_POSITION];
			mCache->vertexBufferOffset = mesh.mVOffset[0];
			mDevice->IASetVertexBuffers( 0, mesh.mNbrOfVertexBuffers,
				&mesh.mBuffers[MeshCache::VERTEX_BUFFER_POSITION],
				mesh.mStrides, mesh.mVOffset );
		}

		if( mCache->indexBufferCurrent != mesh.mBuffers[MeshCache::INDEX_BUFFER_POSITION])
		{
			mCache->indexBufferCurrent = mesh.mBuffers[MeshCache::INDEX_BUFFER_POSITION];
			mDevice->IASetIndexBuffer( mCache->indexBufferCurrent, DXGI_FORMAT_R32_UINT, model.mIOffset );
		}

		// Now set the vertex shader constants
		/*
		if( !model.mConstantBuffers.isNull() )
		{
			BufferArray & cVBuffers = BufferArrayManager::fromHandle(model.mConstantBuffers);
			bool foundChanges = false;
			for( unsigned int vsBufferIndex = cVBuffers.mStartBindLocation; vsBufferIndex < cVBuffers.getNumberOfBuffers(); ++vsBufferIndex) 
			{
				if( vsBufferIndex > MAX_NUMBER_BUFFERS )
					break; //Todo: warn that something went wrong

				if( mCache->vertexConstantBuffers[vsBufferIndex] != cVBuffers.getBuffer( vsBufferIndex - cVBuffers.mStartBindLocation ) )
				{
					mCache->vertexConstantBuffers[vsBufferIndex] = cVBuffers.getBuffer( vsBufferIndex - cVBuffers.mStartBindLocation );
					foundChanges = true;
					break;
				}
			}

			if( foundChanges ) 
			{
				mDevice->VSSetConstantBuffers( cVBuffers.mStartBindLocation, cVBuffers.getNumberOfBuffers(), cVBuffers.getBuffers() );
			}
		}
		*/
		/*if( !model.mConstantBuffers.isNull() )
		{
			mDevice->VSSetConstantBuffers( ConstantBufferArrayManager::getStartBindingLocation( model.mConstantBuffers),
			ConstantBufferArrayManager::getStartBindingLocation( model.mConstantBuffers ),
			ConstantBufferArrayManager::getBindableBuffers( model.mConstantBuffers ) );
		}*/

		// Now set the buffers that are always changing 
		if( !loop->mVSConstants.isNull() )
		{
			mDevice->VSSetConstantBuffers( ConstantBufferArrayManager::getStartBindingLocation( loop->mVSConstants),
			ConstantBufferArrayManager::getNbrBuffers( loop->mVSConstants),
			ConstantBufferArrayManager::getBindableBuffers( loop->mVSConstants) );
		}
		
		if( !loop->mPSConstants.isNull() )
		{
			mDevice->VSSetConstantBuffers( ConstantBufferArrayManager::getStartBindingLocation( loop->mPSConstants),
			ConstantBufferArrayManager::getNbrBuffers( loop->mPSConstants),
			ConstantBufferArrayManager::getBindableBuffers( loop->mPSConstants) );
		}


		// To actually render the mesh
		mDevice->DrawIndexed( model.mINumber,model.mIOffset, model.mIVertexOffset);
	}
	

	//mDevice->FinishCommandList( false, &mCommandList );
}


void RenderQue::sortAll()
{
	// If we really want could prob do multi threaded here
	// Todo: maybe consider if the number of items exceeds a certain threshold then do a special sort
	//	rather than a normal quick sort
	// I don't care if items retain their close ordering...


	auto sortFunction = [](RenderableInstance & first, RenderableInstance & second) { return first.mKey > second.mKey; };
	if( mItems.size() > 0 )
		std::sort(mItems.begin(), mItems.end(), sortFunction ); 

	if( mItemsInstanced.size() > 0 )
		std::sort(mItemsInstanced.begin(), mItemsInstanced.end(), sortFunction);

}

void RenderQue::render( D3D11 * d3d )
{
	//d3d->getDeviceContext()->ExecuteCommandList( mCommandList, false );
}
