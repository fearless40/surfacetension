#pragma once

/* Render Que 
 * Purpose of the class is to sort the data that needs to be sent to the gpu in an efficent manner
 * 
 * Sort Order
 * 
 * Sort by material first
 * 1. if the material has translucency then place it in a seperate que
 * 2. Sort by texture array
 * 3. Sort by raster state
 * 4. Sort by mesh data (can be optional)
 *
 * Send all the data to the gpu (either though multithreaded mode) or single threaded mode
 * RenderQue are for access by one thread at a time, create more than one render que for multithreaded behavior
 */

#include "../d3d11/Handles.h"
#include "Render.h"
#include "RenderableInstance.h"

class Material;
class Model;
class D3D11;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct StateCache;
struct ID3D11CommandList;


class RenderQue
{
public:
	
	typedef std::vector<RenderableInstance> ItemsVector;

	enum EstimatedNumberItems {
		ENI_Items,
		ENI_Instanced
	};

	RenderQue(void);
	~RenderQue(void);


	// Creates the deferred rending context. Call only once
	void initalize( D3D11 & d3d );

	void setEstimatedNumberItems( unsigned int nbrItems, EstimatedNumberItems eni = ENI_Items );

	template <class iterator> void addItemStream( iterator begin, iterator end ) {
		mItems.copy( begin, end );
	}

	template <class iterator> void addItemStreamInstance( iterator begin, iterator end )
	{
		mItemsInstanced.copy( begin, end );
	}

	void setVertexShaderConstantBuffer( ConstantBufferArrayHandle buf );
	void setVertexShaderConstantBuffer( ConstantBufferHandle buf, uint32_t pos );

	void setPixelShaderConstantBuffer( ConstantBufferArrayHandle buf );
	void setPixelShaderConstantBuffer( ConstantBufferHandle buf, uint32_t pos );

	void addItem( RenderableInstance item );
	void addItemInstanced( RenderableInstance item );

	// Not implemented yet
	void setRenderTarget();

	// Call to initalize the rendering subsytem sets up some items for further use
	// Will in the end have a render target object to use, not currently implemented
	// Therefore will awlways use the cached value from the inital initalization
	void begin();

	// Sorts the data and adds all the commands to the command list (main working function)
	void end();

	// Actually renders the command list into the immediete context
	void render(D3D11 * d3d); 

	void clearRenderTargerts( );

protected:
	
	// Pointer to a Defferred Contex (allows multithreading)
	ID3D11DeviceContext * mDevice;

	
	// Useful to que up a set of commands and then reuse them without having to draw them again
	ID3D11CommandList	* mCommandList;

	// Holds the internal state of the device and prevents calling unnecessary binds
	// Pimpl model as this may change a lot in the future
	StateCache * mCache;

	

	ItemsVector mItems;
	ItemsVector mItemsInstanced;

	// The deferred context does not inherit any state including the render target or the depthStencil
	// Therefore cache it and use it as needed
	ID3D11RenderTargetView * mRenderTarget;
	ID3D11DepthStencilView * mDepthStencil;

	void sortAll();
	
};

