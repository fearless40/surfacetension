
#include "../stdafx.h"
#include "../d3d11/D3D11.h"
#include "../render/RenderQue.h"
#include "MicroManager.h"
#include "../d3d11/ConstantBufferManager.h"
#include "../d3d11/ConstantBufferArrayManager.h"
#include "../render/ModelManager.h"

namespace BasicMM {

__declspec(align(16)) struct perViewData {
		matrix view;
		float4  time;
};

MicroManager::MicroManager( D3D11 & d3d, RenderQue & que ) : 
	md3d( d3d ), mQue( que )
{
	matrix def;

	//mQue.initalize( md3d );

	mPerFrame = ConstantBufferManager::newDynamic( md3d,  &def, sizeof( perViewData ) );
	mPerDisplay = ConstantBufferManager::newDynamic( md3d, &def, sizeof( matrix )  );

	mQue.setVertexShaderConstantBuffer( mPerDisplay, 0 );
	mQue.setVertexShaderConstantBuffer( mPerFrame, 1 );


}




RenderableInstance MicroManager::newRi( ModelHandle model, MaterialHandle mat )
{
	ConstantBufferArray cba(1);

	cba.setStartBindLocation(2);

	matrix def;
	DirectX::XMStoreFloat4x4( &def, DirectX::XMMatrixTranspose(DirectX::XMMatrixIdentity() ) );
	
	// Allocate world transform data
	// Currently that is all the engine expects
	cba.addDynamicBuffer( md3d, 0, &def, sizeof( matrix ) );

	// Since materials are not used ignore it
	RenderableInstance ri;
	ri.mModel = model;
	ri.mVSConstants = cba.getHandle();
	ri.mKey = (ModelManager::fromHandle(model).mMesh.mIndex) << 16;

	return ri;

}

void MicroManager::prep( RenderableInstance ri, matrix transform )
{
	ConstantBufferArrayManager::editBuffer( md3d, ri.mVSConstants, 0, &transform, sizeof( matrix ) );
	mQue.addItem( ri );
}

void MicroManager::frameNew(matrix CamView, float time)
{
	perViewData data = { CamView, float4(time,0,0,0) };
	
	mQue.begin();
	mQue.clearRenderTargerts();
	ConstantBufferManager::editBuffer( md3d, mPerFrame,  &data, sizeof( perViewData ) );
	

}

void MicroManager::frameEnd()
{
	mQue.end();
}

void MicroManager::render()
{
	mQue.render( &md3d );
	md3d.endRender();
}

void MicroManager::displayUpdate( matrix projection )
{
	ConstantBufferManager::editBuffer( md3d, mPerDisplay,  &projection, sizeof( matrix ) );
}

}