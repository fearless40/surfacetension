#pragma once

class ConstantBuffer;

class Material : public InvasiveRefCount
{
public:
	Material(void);
	~Material(void);


protected:
	
	// Not sorted by render que
	//ConstantBufferArray psConstantBuffers;		
	//PSShader		* mpsShader;
	

	//Texture			* mTextureArray;		
	int				mNbrTextures;

	//ID3D11RasterizerState1 *	mRasterState;
	int				textureHash;
	bool			mTranslucency;


};

