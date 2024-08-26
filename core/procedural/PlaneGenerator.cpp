#include "../stdafx.h"
#include "PlaneGenerator.h"


using namespace Procedural;
using namespace DirectX;

XMVECTOR getPerpendicular( XMVECTOR normal ) {
	static const float fSquareZero = (float)(1e-06 * 1e-06);
  
	XMVECTOR UNIT_X  =  XMVectorSet( 1, 0, 0, 0);
	XMVECTOR UNIT_Y  =	XMVectorSet(0,1,0,0);
	

	XMVECTOR perp = XMVector3Cross( normal, UNIT_X );
  
	// Check length
	if( XMVectorGetX(XMVector3LengthSq(perp)) < fSquareZero )
	{
		/* This vector is the Y axis multiplied by a scalar, so we have
		to use another axis.
		*/
		perp = XMVector3Cross( normal, UNIT_Y );
	}
	
	return XMVector3Normalize(perp);
	
}

void PlaneGenerator::addToMeshBuilder( MeshBuilderType & buffer ) 
{
	buffer.rebaseOffset();
	buffer.estimateVertexCount((mNumSegX+1)*(mNumSegY+1));
	buffer.estimateIndexCount(mNumSegX*mNumSegY*6);
	int offset = 0;
	XMVECTOR normal = XMLoadFloat3( &mNormal );

	XMVECTOR vX = getPerpendicular( normal );
	XMVECTOR vY = XMVector3Cross( normal, vX);
	
	XMVECTOR delta1 = XMVectorScale( vX, mSizeX / (float) mNumSegX );
	XMVECTOR delta2 = XMVectorScale( vY, mSizeY / (float) mNumSegY );
	// build one corner of the square
	XMVECTOR orig = XMVectorSubtract(XMVectorScale( vX, -0.5f*mSizeX),
									XMVectorScale(vY, 0.5f*mSizeY));


	for (int i1 = 0; i1<=mNumSegX; i1++)
	{
		for (int i2 = 0; i2<=mNumSegY; i2++)
		{
			XMVECTOR pos = XMVectorAdd( orig, XMVectorScale( delta1, (float) i1 ) );
			pos			 = XMVectorAdd( pos, XMVectorScale( delta2, (float) i2 ) );
			
		
			XMFLOAT2 uv;

			uv.x = (float)i1 / (float) mNumSegX;
			uv.y = (float)i2 / (float) mNumSegY;
			
			
			addPoint( buffer, pos, normal, uv);
		}
	}

	bool reverse = false;
	
	if ( toScalar( XMVector3Dot(XMVector3Cross(delta1, delta2), normal)) > 0)
		reverse= true;

	for (unsigned short n1 = 0; n1<mNumSegX; n1++)
	{
		for (unsigned short n2 = 0; n2<mNumSegY; n2++)
		{
			if (reverse)
			{
				buffer.index(offset+0);
				buffer.index(offset+(mNumSegY+1));
				buffer.index(offset+1);
				buffer.index(offset+1);
				buffer.index(offset+(mNumSegY+1));
				buffer.index(offset+(mNumSegY+1)+1);
			}
			else
			{
				buffer.index(offset+0);
				buffer.index(offset+1);
				buffer.index(offset+(mNumSegY+1));
				buffer.index(offset+1);
				buffer.index(offset+(mNumSegY+1)+1);
				buffer.index(offset+(mNumSegY+1));
			}
			offset++;
		}
		offset++;
	}
}


