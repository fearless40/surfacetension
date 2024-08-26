
#include "../d3d11/Handles.h"
#include "../render/RenderableInstance.h"
#include "../util/Math.h"


using namespace Math;

// MicroManger
// The class is used to help minimze writes to the video card unless it is necessary.
// The data maintined in the class is a list of objects to write to the screen 
// and update the ConstantBuffer data in each object so it works with the shaders.
// For instance in this game the Shaders expect worldTransfrorm to be in the 2nd
// register spot. This class would help standerdize that information.
// No culling is done in this class. User a seperate culling class to do the dirty 
// work. This class can and should be edited in the future to allow different modes 
// of updateing the ConstantBuffer data.

// The class does NOT cull objects. That is up to the rest of the code. This code is 
// strictly for interacting with the graphics card in a more organized and efficent manner.
// It converts data into video card constant data and will automatically sort models and render them
// using the most efficient process it can. 

// The class will also handle the organization of instance rendering. Instance rendering must be done
// by organized by a higher up level however this will take care of the low level details and
// choose the appropiate vertex shaders for the job

class D3D11;
class RenderQue;

namespace BasicMM {

class MicroManager 
{
public:

	struct UserData
	{
		void		* data;
		uint32_t	  size;
	};
	
	MicroManager( D3D11 & d3d, RenderQue & que );
	
	//void initalize(D3D11 & d3d, RenderQue & que); 

	// Per application setup functions (change very infrequently)
	// Mostly to setup constant buffers for the per appliction frame data
	/*void setCameraProjection( matrix projection );
	void setUserRareUpdateData( UserData data );

	// Per frame data
	static void setCameraView( matrix view );
	static void setFrameTime( float time );
	static void setUserFrameUpdate( UserData data );*/

	// Need to be static across different invocations of MicroManagers...
	// In other words must have the following functions if you expect to use it as a drop in replacement in code
	
	// Per object
		// Non-instanced
	void prep( RenderableInstance ri, matrix transform );
	void prep( RenderableInstance ri, matrix transform, UserData data );
	
		//Instanced
	void prepInstanced( RenderableInstance ri, uint32_t nbrInstances, matrix * transforms, UserData data );
	void prepInstanced( RenderableInstance ri, uint32_t nbrInstances, matrix * transforms );

	RenderableInstance newRi( ModelHandle model, MaterialHandle mat );


	// Display related items
	void displayUpdate( matrix projection );

	// Frame related items

	void frameNew(matrix CamView, float time);
	void frameEnd();

	// Drawing related items
	void render();
	
private:
	MicroManager();
	D3D11 & md3d;
	RenderQue & mQue;
	ConstantBufferHandle mPerFrame;
	ConstantBufferHandle mPerDisplay;
	
};


};
