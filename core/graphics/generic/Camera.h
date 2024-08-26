#pragma once

#include "Math.h"

namespace Engine {
	class Camera
	{
	public:
		Camera(void);
		~Camera(void);

		void lookAt(Vector4 item_position);
		void lookAt(Vector4 item_position, Vector4 up);
		void lookAt(Vector4 eye_position, Vector4 item_position,  Vector4 up);
		void lookAtFast(Math::FXMVECTOR eye_position, Math::FXMVECTOR item_position, Math::FXMVECTOR up) {
			mPosition = eye_position;
			mViewMatrix = Math::XMMatrixLookAtLH(mPosition, item_position, up);
			mQuatRotation = Math::XMQuaternionRotationMatrix(Math::XMMatrixTranspose(mViewMatrix));

		}

		void setPosition(Vector4 position, Vector4 rotQ);
		void setPosition(Vector4 position);
		void setPosition(Math::FXMVECTOR position, Math::FXMVECTOR qrotation);

		void setRotationEuler(Vector3 rotEuler);
		void setRotationEuler(Vector3 rotEuler, Vector4 position);

		void setRotation(DirectX::XMFLOAT4 rotQuat);
		void setRotation(Math::FXMVECTOR rotQuat);

		void setPerspective(float width, float height, float near, float far);
		void setPerspectiveFOV(float fovAngleRadiansY, float aspectRatio, float near, float far);
		void setOrthographic(float width, float height, float near, float far);

		fMatrix getView() const noexcept { return mViewMatrix; }
		fMatrix getProj() const noexcept { return mProjMatrix; }
		fVector4 getPosition() const noexcept { return mPosition; }


	private:
		void calcView();

		alignas(16)	fMatrix		mViewMatrix;
		alignas(16) fMatrix		mProjMatrix;
		alignas(16) fVector4	mQuatRotation;
		alignas(16) fVector4	mPosition;

		/*float					mFovAngleRadians;
		float					mAspectRatio;
		float					mNear;
		float					mFar;*/

		/*
		__declspec(align(16)) struct privateData {
			DirectX::XMVECTOR mUp;
			DirectX::XMVECTOR mPos;
			DirectX::XMVECTOR mRotQuat;
			DirectX::XMVECTOR mLookAt;

			DirectX::XMMATRIX mViewProjMatrixT;	//T is that the matrix is transposed
		};
		*/
		//privateData * mpd;

		//void calcViewProj();
	};

};