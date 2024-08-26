#include "../stdafx.h"
#include "SphereGenerator.h"


using namespace Procedural;
using namespace DirectX;

void SphereGenerator::addToMeshBuilder( Procedural::MeshBuilderType & buffer )
{
	assert(mNumRings>0 && mNumSegments>0 && "Num seg must be positive");
	assert(mRadius>0 && "Radius must be positive");

	buffer.rebaseOffset();
	buffer.estimateVertexCount((mNumRings+1)*(mNumSegments+1));
	buffer.estimateIndexCount(mNumRings*(mNumSegments+1)*6);

	float fDeltaRingAngle = (DirectX::XM_PI / mNumRings);
	float fDeltaSegAngle = (DirectX::XM_2PI / mNumSegments);
	int offset = 0;

	// Generate the group of rings for the sphere
	for(unsigned int ring = 0; ring <= mNumRings; ring++ ) {
		float r0 = mRadius * sinf (ring * fDeltaRingAngle);
		float y0 = mRadius * cosf (ring * fDeltaRingAngle);

		// Generate the group of segments for the current ring
		for(unsigned int seg = 0; seg <= mNumSegments; seg++) {
			float x0 = r0 * sinf(seg * fDeltaSegAngle);
			float z0 = r0 * cosf(seg * fDeltaSegAngle);

			// Add one vertex to the strip which makes up the sphere
			XMVECTOR pos = toVector( x0, y0, z0 );
			XMVECTOR normal = toVector( x0, y0, z0 );
			normal = XMVector4Normalize( normal );
			XMFLOAT2 uv((float) seg / (float) mNumSegments, (float) ring / (float) mNumRings);
			addPoint(buffer, pos,
							 normal,
							 uv);

			if (ring != mNumRings) {
				// each vertex (except the last) has six indices pointing to it
				buffer.index(offset + mNumSegments + 1);
				buffer.index(offset);
				buffer.index(offset + mNumSegments);
				buffer.index(offset + mNumSegments + 1);
				buffer.index(offset + 1);
				buffer.index(offset);
				offset ++;
				}
		}; // end for seg
	} // end for ring
}
