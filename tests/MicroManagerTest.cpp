#include "../stdafx.h"
#include "../d3d11/D3D11.h"
#include "../engine/MicroManager.h"
#include "../d3d11/VertexShaderManager.h"
#include "../d3d11/ShaderManager.h"
#include "../render/Camera.h"
#include "../render/RenderableInstance.h"
#include "../render/RenderQue.h"
#include "../procedural/Procedural.h"
#include "../procedural/BoxGenerator.h"
#include "../render/ModelBuilder.h"
#include "../render/ModelManager.h"
#include "../procedural/SphereGenerator.h"
#include "MicroMangerTest.h"


namespace Test {

	struct BoxDemo2
	{
		float x,y,z;
		float roll, pitch, yaw;
		RenderableInstance ri;
	};

	struct MicroManagerTest::InternalData 
	{
		BasicMM::MicroManager * mang;
		RenderQue				que;
		ShaderManager			sManager;
		ID3D11PixelShader	*	ps;
		Camera					cam;
		ModelHandle				model;
		ModelHandle				sphere;
		std::vector< BoxDemo2 >	boxes;
	};
		
	void MicroManagerTest::clearData()
	{

	}

	MicroManagerTest::MicroManagerTest( D3D11 & d3d ) : mData( new MicroManagerTest::InternalData() ),
		mIsOk( false ),
		md3d( d3d )
	{
		mData->que.initalize( d3d );
		mData->mang = new BasicMM::MicroManager( d3d, mData->que );
	}

	MicroManagerTest::~MicroManagerTest()
	{
		if( mData && mData->mang )
			delete mData->mang;
		delete mData;
	}
		



	bool MicroManagerTest::isOk()
	{
		return mIsOk;
	}
		
	bool MicroManagerTest::initalize(float width, float height)
	{
		VertexShader vertexShader;
	
		vertexShader.set(  mData->sManager.loadVertexFromFile(&md3d, L"EngineVSPosNormalUV") );
		mData->ps = mData->sManager.loadPixelFromFile(&md3d, L"PSPosNormalUV");

	
		VertexShaderHandle modelShader = VertexShaderManager::newVertexShader( vertexShader );

		md3d.getDeviceContext()->PSSetShader(mData->ps,NULL,NULL);

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

		mData->cam.setPerspectiveFOV( DirectX::XMConvertToRadians(45),width / height, 0.1, 500 );
		mData->cam.lookAt( .0f,.0f,.0f );
		mData->cam.setPosition( .0f, .0f, -30.0f );
		mData->cam.setRotationEuler( .0f,.0f,.0f);


		int count = 0;
		MaterialHandle fakeMaterial;
		// Generate the box Array and positions
		for( float xPos = -10.0f; xPos < 10.0f; xPos += 2.0f )
		{
			for( float yPos = -10.0f; yPos < 10.0f; yPos += 2.0f )
			{
				count++;
				BoxDemo2 bx = {xPos, yPos, 0.0f, 0.0f, 0.0f, 0.0f, mData->mang->newRi( count % 2 == 0 ? mData->model : mData->sphere, fakeMaterial )};
				mData->boxes.push_back( bx );
			}
		}

		mIsOk = true;

		matrix proj;
		mData->cam.getProjData( &proj );
		mData->mang->displayUpdate( proj );


		return mIsOk;

	}

	using namespace DirectX;

	bool MicroManagerTest::frame()
	{

		matrix view;

		md3d.getDeviceContext()->PSSetShader(mData->ps,NULL,NULL);

		mData->cam.getViewData( &view );

		mData->mang->frameNew( view, 0.0f );
	
		D3D11 & d3d = md3d;
		
		DirectX::XMVECTOR scale = Math::toVector(1.0f,1.0f,1.0f,0.0f);
		DirectX::XMVECTOR rotPoint = Math::toVector(0.0f,0.0f,0.0f,0.0f);
		DirectX::XMVECTOR quat;
		DirectX::XMVECTOR translation;
		
		int count = 0;
		for( auto & it : mData->boxes )
		{
			matrix world;

			// Animat the boxes 
	
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

			translation = Math::toVector( it.x, it.y, it.z, 0.0f );
			quat = DirectX::XMQuaternionRotationRollPitchYaw(	XMConvertToRadians( it.pitch ),
																XMConvertToRadians( it.yaw ), 
																XMConvertToRadians( it.roll));
			XMStoreFloat4x4(&world,XMMatrixTranspose(XMMatrixAffineTransformation(scale, rotPoint, quat, translation)));

			mData->mang->prep( it.ri, world );
			
			
		 
			
		}

		mData->mang->frameEnd();
		mData->mang->render();

		return true;

		

	};



}