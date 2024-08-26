
#include "../stdafx.h"

#include "../d3d11/D3D11.h"
#include "../d3d11/Handles.h"
#include <DirectXMath.h>
#include "../d3d11/VertexShaderManager.h"
#include "../d3d11/ShaderManager.h"
#include "../render/Camera.h"
#include "../render/MeshCPU.h"
#include "../d3d11/Mesh.h"
#include "../render/Model.h"
#include "../procedural/Procedural.h"
#include "../procedural/PlaneGenerator.h"
#include "../procedural/BoxGenerator.h"
#include "../procedural/SphereGenerator.h"
#include "../render/ModelBuilder.h"
#include "../d3d11/ConstantBufferManager.h"
#include "../d3d11/IndexBufferManager.h"
#include "../d3d11/VertexBufferManager.h"
#include "../render/ModelManager.h"
#include "../render/RenderableInstance.h"
#include "../render/RenderQue.h"
#include "../d3d11/ConstantBufferArrayManager.h"
#include "LowLevelTest.h"


namespace Test {

	struct cb {
		DirectX::XMFLOAT4X4	view;
		DirectX::XMFLOAT4X4 proj;
		DirectX::XMFLOAT4X4 viewproj;
	};

	struct perObject {
		DirectX::XMFLOAT4X4 world;
	};

	struct BoxDemo
	{
		float x,y,z;
		float roll, pitch, yaw;
		BufferArrayHandle cb;
	};

	struct LowLevelTest::InternalData {
		ModelHandle model;
		ModelHandle sphere;
		ShaderManager sManager;
		ConstantBufferHandle cbuffer;
		Camera cam;
		std::vector< BoxDemo > boxes;
		std::vector< RenderableInstance > instances;
		RenderQue que;
		ID3D11PixelShader * ps;
	};

	LowLevelTest::LowLevelTest( D3D11 & d3d ) : md3d(d3d), mIsOk(false), mData( new LowLevelTest::InternalData() )
	{
		clearData();
	}

	LowLevelTest::~LowLevelTest()
	{
		delete mData;
	}

	void LowLevelTest::clearData()
	{
		//memset( mData, 0, sizeof( LowLevelTest::InternalData ) );
	}

	bool LowLevelTest::isOk()
	{
		return mIsOk;
	}

	bool LowLevelTest::initalize(float width, float height)
	{
		VertexShader vertexShader;
	
		vertexShader.set(  mData->sManager.loadVertexFromFile(&md3d, L"VSPosNormalUV") );
		mData->ps = mData->sManager.loadPixelFromFile(&md3d, L"PSPosNormalUV");

	
		VertexShaderHandle modelShader = VertexShaderManager::newVertexShader( vertexShader );

		//d3d.getDeviceContext()->VSSetShader(vs,NULL,NULL);
		md3d.getDeviceContext()->PSSetShader(mData->ps,NULL,NULL);

	
		Procedural::PlaneGenerator pg;
		pg.mSizeX = 1.0f;
		pg.mSizeY = 1.0f;
		pg.mNumSegX = 128;
		pg.mNumSegY = 128;
		pg.mNormal = DirectX::XMFLOAT3(0.f,0.f,-1.0f);
		Procedural::MeshBuilderType mbt;
		Procedural::BoxGenerator	bg;
	

		bg.addToMeshBuilder( mbt );

		auto mesh = mbt.toMeshVertexCPU();

		mData->model = ModelBuilder::makeStaticGeometryModel( md3d, VertexTypeManager::getVertexType(VertexTypeManager::Pos_Normal_UV), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, mesh->toMeshCPU() );
		ModelManager::fromHandle(mData->model).mVSShader = modelShader;

		mbt.clear();

		Procedural::SphereGenerator sg(1,128,128);

		sg.addToMeshBuilder( mbt );
		auto meshSphere = mbt.toMeshVertexCPU();
		mData->sphere = ModelBuilder::makeStaticGeometryModel( md3d, VertexTypeManager::getVertexType(VertexTypeManager::Pos_Normal_UV), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, meshSphere->toMeshCPU() );
		ModelManager::fromHandle(mData->sphere).mVSShader = modelShader;

		mData->cam.setPerspectiveFOV( DirectX::XMConvertToRadians(45),width / height, 0.1f, 500.0f );
		mData->cam.lookAt( .0f,.0f,.0f );
		mData->cam.setPosition( .0f, .0f, -30.0f );
		mData->cam.setRotationEuler( .0f,.0f,.0f);

		cb buf;
		memset( &buf, 0, sizeof( cb ) );
	
		//DirectX::XMStoreFloat4x4(&buf.world, DirectX::XMMatrixIdentity());
		//mData->cam.getViewProjData( &buf.viewProj );
		mData->cbuffer = ConstantBufferManager::newDynamic( md3d, &buf, sizeof(cb) );
	
	

	//	vs->Release();
		//ps->Release();

		DirectX::XMVECTOR scale = toVector(1.0f,1.0f,1.0f,0.0f);
		DirectX::XMVECTOR rotPoint = toVector(0.0f,0.0f,0.0f,0.0f);
		DirectX::XMVECTOR quat = DirectX::XMQuaternionIdentity();
		DirectX::XMVECTOR translation;

		int count = 0;
		// Generate the box Array and positions
		for( float xPos = -10.0f; xPos < 10.0f; xPos += 2.0f )
		{
			for( float yPos = -10.0f; yPos < 10.0f; yPos += 2.0f )
			{
				count++;
				BoxDemo bx = {xPos, yPos, 0.0f, 0.0f, 0.0f, 0.0f};
				translation = toVector(xPos, yPos, 0.0f, 0.0f);
				DirectX::XMMATRIX mat = DirectX::XMMatrixAffineTransformation(scale, rotPoint, quat, translation);
				perObject pObj;
				XMStoreFloat4x4(&pObj.world,XMMatrixTranspose(mat) );
				ConstantBufferArray bArray(ConstantBufferArrayManager::newBufferArray(1,1));
				bArray.addDynamicBuffer(md3d,0,&bx, sizeof(perObject) );
				bx.cb = bArray;
				mData->boxes.push_back( bx );
			}
		}

		mData->instances.reserve(count);

		mData->que.initalize( md3d );

		mData->que.setVertexShaderConstantBuffer( mData->cbuffer, 0 );

		mIsOk = true;

		return mIsOk;
	}


	using namespace DirectX;

	bool LowLevelTest::frame() 
	{
		cb buf;
		static float roll = 0;

		XMMATRIX view;
		XMMATRIX proj;
		XMMATRIX world;

		XMVECTOR up		= Math::toVector(.0f,1.0f,.0f,.0f);
		XMVECTOR look	= Math::toVector(.0f,.0f,.0f,.0f);
		XMVECTOR pos	= Math::toVector(.0f,.0f,-10.0f,.0f);
		

		roll += 0.0001f;
	
		world	= XMMatrixRotationRollPitchYaw(0.f,roll,0.f);
		view	= mData->cam.getView();
		proj	= mData->cam.getProj();

	
		
		XMStoreFloat4x4(&buf.view, XMMatrixTranspose(view));
		XMStoreFloat4x4(&buf.proj, XMMatrixTranspose(proj));
		XMStoreFloat4x4(&buf.viewproj, XMMatrixMultiplyTranspose( view, proj ) );
	
	
		mData->cam.getViewProjData( &buf.viewproj );
	
		D3D11 & d3d = md3d;
	
		ConstantBufferManager::editBuffer( d3d ,mData->cbuffer, &buf, sizeof(buf) );

		
		//d3d.beginRender();


		// To clear the render targets
	
		mData->que.begin();

		mData->que.clearRenderTargerts();

	
		DirectX::XMVECTOR scale = toVector(1.0f,1.0f,1.0f,0.0f);
		DirectX::XMVECTOR rotPoint = toVector(0.0f,0.0f,0.0f,0.0f);
		DirectX::XMVECTOR quat;
		DirectX::XMVECTOR translation;
		perObject pObj;
		int count = 0;
		for( auto & it : mData->boxes )
		{
			// Animat the boxes 
			// I know this is slow but a useful test
		
			if( count % 2 == 0) 
			{
				it.roll += 0.1f;
			}
			if( count % 3 == 0)
			{
				it.yaw += 0.1f;
			}
			if( count % 7 == 0)
			{
				it.pitch += 0.1f;
			}

				
			++count;

			translation = toVector( it.x, it.y, it.z, 0.0f );
			quat = DirectX::XMQuaternionRotationRollPitchYaw(	XMConvertToRadians( it.pitch ),
																XMConvertToRadians( it.yaw ), 
																XMConvertToRadians( it.roll));
			world = DirectX::XMMatrixAffineTransformation(scale, rotPoint, quat, translation);
			XMStoreFloat4x4(&pObj.world,XMMatrixTranspose(world) );
			
			
			ConstantBufferArrayManager::editBuffer( d3d, it.cb, 0, &pObj, sizeof( pObj ) );
			
			
			//d3d.getDeviceContext()->VSSetConstantBuffers( 1, 1, ConstantBufferArrayManager::getBindableBuffers( it.cb ) );
			
			

			RenderableInstance ri;
			ri.mModel = count % 2 == 0 ? mData->model : mData->sphere;
			ri.mKey = ((count % 2) << 16) + count;
			ri.mVSConstants = it.cb;

			mData->que.addItem( ri );
			
			//ModelManager::fromHandle(mData->model).render(&d3d);
			
		}

		mData->que.end();
		mData->que.render( &d3d );

		d3d.endRender();


		return true;
	}




	void GenerateQuickBox() 
	{
		/*RefCountPointer<MeshVertexCPU<VertexTypeManager::VertexPos> > mesh( new MeshVertexCPU<VertexTypeManager::VertexPos> );
	
		mesh->allocateNbrVerts(3);
		mesh->allocateNbrIndexs(3);

		
		unsigned long nbrItems;
		VertexTypeManager::VertexPos * verts = mesh->lockVertsType( nbrItems );
		verts[0].position = DirectX::XMFLOAT4( -0.5f, -0.5f, 0.5f, 1.0f );
		verts[1].position = DirectX::XMFLOAT4( 0.0f, 0.5f, 0.5f, 1.0f );
		verts[2].position = DirectX::XMFLOAT4( 0.5f, -0.5f, 0.5f, 1.0f ); 
		mesh->unlockVerts();

		MeshCPU::IndexType * in = mesh->lockIndexType( nbrItems );
		in[0] = 0;
		in[1] = 1;
		in[2] = 2;
		mesh->unlockIndex();
		*/

		/*RefCountPointer<MeshVertexCPU<VertexTypeManager::VertexPosNormalUV> > mesh( new MeshVertexCPU<VertexTypeManager::VertexPosNormalUV> );
	
		mesh->allocateNbrVerts(4);
		mesh->allocateNbrIndexs(6);

		
		unsigned long nbrItems;
		VertexTypeManager::VertexPosNormalUV * verts = mesh->lockVertsType( nbrItems );
		verts[0].position = DirectX::XMFLOAT4( -0.5f, -0.5f, 0.5f, 1.0f );
		//verts[0].color= DirectX::XMFLOAT4( 1,0,0,1);
		verts[0].normal = DirectX::XMFLOAT4( 0,1,0,1);
		verts[0].uv = DirectX::XMFLOAT2( 0,0 );

		verts[1].position = DirectX::XMFLOAT4( -0.5f, 0.5f, 0.5f, 1.0f );
		//verts[1].color= DirectX::XMFLOAT4( 0,1,0,1);
		verts[1].normal = DirectX::XMFLOAT4( 1,0,0,1);
		verts[1].uv = DirectX::XMFLOAT2( 1,0 );

		verts[2].position = DirectX::XMFLOAT4( 0.5f, 0.5f, 0.5f, 1.0f ); 
		//verts[2].color= DirectX::XMFLOAT4( 0,0,1,1);
		verts[2].normal = DirectX::XMFLOAT4( 0,1,1,1);
		verts[2].uv = DirectX::XMFLOAT2( 1,1 );

		verts[3].position = DirectX::XMFLOAT4( 0.5f, -0.5f, 0.5f, 1.0f);
		//verts[3].color= DirectX::XMFLOAT4( 1,1,1,1);
		verts[3].normal = DirectX::XMFLOAT4( 1,0,1,1);
		verts[3].uv = DirectX::XMFLOAT2( 0,1 );
		mesh->unlockVerts();

		MeshCPU::IndexType * in = mesh->lockIndexType( nbrItems );
		in[0] = 0;
		in[1] = 1;
		in[2] = 2;
		in[3] = 0;
		in[4] = 2;
		in[5] = 3;
		mesh->unlockIndex();*/
	}


}