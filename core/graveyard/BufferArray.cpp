#include "stdafx.h"
#include <d3d11.h>
#include "BufferArray.h"


BufferArray::BufferArray(void) : 
	mBuffers( nullptr ),
	mNbrBuffers( 0 ),
	mStartBindLocation( 0 )
{
}

BufferArray::BufferArray( const BufferArray & ba ) :
	mNbrBuffers(0),
	mBuffers( nullptr ),
	mStartBindLocation( 0 )
{
	if( ba.getNumberOfBuffers() < 1 )
		return;

	setNumberOfBuffers( ba.getNumberOfBuffers() );
	for( int loop = 0; loop < mNbrBuffers; ++loop )
	{
		mBuffers[loop] = ba.mBuffers[loop];
	}
}



BufferArray::~BufferArray(void)
{
	if( mBuffers )
		delete [] mBuffers;
}


void BufferArray::setNumberOfBuffers( unsigned int buffNbr ) 
{
	if( buffNbr > (D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT - 1) )
	{
		//Error! 
		//Can't bind this many buffers
		return;
	}

	if( buffNbr < 1 )
		return;

	if( mBuffers )
		delete [] mBuffers;


	mBuffers = new ID3D11Buffer*[buffNbr];
	mNbrBuffers = buffNbr;

}

		


/*void BufferArray::setBindStartLocation(unsigned int startPos )
{
	if( mNbrBuffers + startPos > (D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT - 1)  )
	{
		//Over binding the number of buffers and slots now...
		return;
	}

	mStartBindLocation = startPos;
}*/


void BufferArray::setBuffer( ID3D11Buffer * buf, unsigned int index )
{
	if( index > mNbrBuffers )
	{
		return;
	}
	
	mBuffers[index] = buf;
}


ID3D11Buffer * BufferArray::getBuffer( unsigned int index )
{
	if( index > mNbrBuffers )
	{
		return nullptr;
	}
	
	return mBuffers[index];
}

