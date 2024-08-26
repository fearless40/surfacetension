#pragma once


#include <Adam/Rect/RectT.h>
#include <Adam/Vector/Vector2.h>
#include "../util/Math.h"
#include "Procedural.h"

using namespace Procedural;
using namespace Math;

class MeshGeneratorHelper
{
	typedef ST::Vector2T<float>	Point;
	typedef ST::RectT<float, Point > Rectangle;

protected:
	/// U tile for texture coords generation
	float mUTile;

	/// V tile for texture coords generation
	float mVTile;

	/// Whether to produces normals or not
	bool mEnableNormals;

	/// The number of texture coordinate sets to include
	unsigned char mNumTexCoordSet;

	/// Rectangle in which the texture coordinates will be placed
	DirectX::XMFLOAT2 mUVOrigin;

	/// If set to true, the UV coordinates coming from the mesh generator will be switched.
	/// It can be used, for example, if your texture doesn't fit the mesh generator's assumptions about UV.
	/// If UV were to fit in a given rectangle, they still fit in it after the switch.
	bool mSwitchUV;

	/// Orientation to apply the mesh
	DirectX::XMVECTOR mOrientation;

	/// Scale to apply the mesh
	DirectX::XMVECTOR mScale;

	/// Position to apply to the mesh
	DirectX::XMVECTOR mPosition;

	// Whether a transform has been defined or not
	bool mTransform;

/*	// Debug output file
	std::string mDumpFileName;

	// Enable output to file or not
	bool mEnableDumpToFile;
	*/
public:
	/// Default constructor
	MeshGeneratorHelper() : mUTile(1.f),
					  mVTile(1.f),
					  mEnableNormals(true),
					  mNumTexCoordSet(1),
					  mUVOrigin(0,0),
					  mSwitchUV(false),
					  mTransform(false)

	{
		mOrientation = DirectX::XMQuaternionIdentity();
		mPosition = DirectX::XMVectorReplicate(0.0f);
		mScale = DirectX::XMVectorReplicate(1.0f);
	}

	
	/**
	 * Overloaded by each generator to implement the specifics
	 */
	//virtual void addToMeshBuilder(Procedural::MeshBuilderType & buffer) const=0;

	/**
	 * Sets U Tile, ie the number by which u texture coordinates are multiplied (default=1)
	 */
	inline void setUTile(float uTile)
	{
		mUTile = uTile;
	}

	/**
	 * Sets V Tile, ie the number by which v texture coordinates are multiplied (default=1)
	 */
	inline void setVTile(float vTile)
	{
		mVTile = vTile;
	}

	/**
	 * Sets the texture rectangle
	 */
	inline void setTextureRectangle(Rectangle textureRectangle)
	{
		mUVOrigin.x = textureRectangle.top;
		mUVOrigin.y = textureRectangle.left;
		mUTile = textureRectangle.right-textureRectangle.left;
		mVTile = textureRectangle.bottom-textureRectangle.top;
	}

	/**
	 * Sets whether normals are enabled or not (default=true)
	 */
	inline void setEnableNormals(bool enableNormals)
	{
		mEnableNormals = enableNormals;
	}

	/**
	 * Sets the number of texture coordintate sets (default=1)
	 */
	inline void setNumTexCoordSet(unsigned char numTexCoordSet)
	{
		mNumTexCoordSet = numTexCoordSet;
	}

	/// Sets whether to switch U and V texture coordinates
	inline void setSwitchUV(bool switchUV)
	{
		mSwitchUV = switchUV;
	}

	/// Sets an orientation to give when building the mesh
	inline void setOrientation(DirectX::XMFLOAT4 orientation)
	{
		mOrientation = Math::toVector(orientation);
		mTransform = true;
	}

	/// Sets an orientation to give when building the mesh
	inline void setOrientation(DirectX::XMVECTOR orientation)
	{
		mOrientation = orientation;
		mTransform = true;
	}

	/// Sets a translation baked into the resulting mesh
	inline void setPosition(const DirectX::XMFLOAT4 & position)
	{
		mPosition = Math::toVector( position );
		mTransform = true;
	}

	inline void setPosition( const DirectX::XMVECTOR position) 
	{
		mPosition = position;
		mTransform = true;
	}

	/// Sets a translation baked into the resulting mesh
	inline void setPosition(float x, float y, float z)
	{
		mPosition = Math::toVector(x,y,z);
		mTransform = true;
	}


	/// Sets a scale baked into the resulting mesh
	inline void setScale(const DirectX::XMFLOAT3 & scale)
	{
		mScale = Math::toVector(scale);
		mTransform = true;
	}

	/// Sets a uniform scale baked into the resulting mesh
	inline void setScale(float scale)
	{
		mScale = toVector(scale);
		mTransform = true;
	}

	/// Sets a scale baked into the resulting mesh
	inline void setScale(float x,float y, float z)
	{
		mScale = toVector(x, y, z);
		mTransform = true;
	}

	/// Sets a scale baked into the resulting mesh
	inline void setScale(DirectX::XMVECTOR scale)
	{
		mScale = scale;
		mTransform = true;
	}


	/// Resets all transforms (orientation, position and scale) that would have been applied to the mesh to their default values
	inline void resetTransforms()
	{
		mTransform = false;
		mPosition = toVector(0.0f);
		mOrientation = DirectX::XMQuaternionIdentity();
		mScale = toVector(1.0f);
	}

	


protected:
	/// Adds a new point to a triangle buffer, using the format defined for that MeshGenerator
	/// @arg buffer the triangle buffer to update
	/// @arg position the position of the new point
	/// @arg normal the normal of the new point
	/// @arg uv the uv texcoord of the new point
	inline void addPoint(Procedural::MeshBuilderType & buffer, DirectX::XMVECTOR position, DirectX::XMVECTOR normal, const DirectX::XMFLOAT2 & uv) const
	{
		Procedural::vertex vt;
		
		if (mTransform)
		{
			DirectX::XMVECTOR newPos;
			newPos = DirectX::XMVectorMultiply(mScale, position);
			newPos = DirectX::XMVector3Rotate(newPos, mOrientation);
			newPos = DirectX::XMVectorAdd(newPos, mPosition);

			Math::toFloats( newPos, vt.position );
		}
		else
		{
			Math::toFloats(position, vt.position);
		}

		if (mEnableNormals)
			Math::toFloats(normal, vt.normal);
		if (mSwitchUV){
			//for (unsigned char i=0;i<mNumTexCoordSet;i++)
				vt.uv.x = mUVOrigin.x +	uv.y * mUTile;
				vt.uv.y = mUVOrigin.y +	uv.x * mVTile;
		}
		else{
			//for (unsigned char i=0;i<mNumTexCoordSet;i++)
				vt.uv.x = mUVOrigin.x + uv.x*mUTile;
				vt.uv.y = mUVOrigin.y+uv.y*mVTile;
		}
		vt.position.w = 1.0f;
		vt.normal.w=0.0f;
		buffer.addVert( vt );
	}

};

