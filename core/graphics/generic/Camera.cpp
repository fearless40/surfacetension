#include "../stdafx.h"
#include "Camera.h"

namespace Engine {
	namespace m = Math;
	Camera::Camera(void)
	{
		/*mpd = (privateData*) _aligned_malloc(sizeof(privateData),16);
		mpd->mUp = DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
		mpd->mPos = DirectX::XMVectorReplicate( 0.0f );
		mpd->mRotQuat = DirectX::XMQuaternionRotationRollPitchYaw( 0.5f, 0.5f, 0.5f );
		mpd->mLookAt = DirectX::XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f );*/
	}


	Camera::~Camera(void)
	{
		//_aligned_free( mpd );
	}


	void Camera::lookAt(Vector4 item_position) {
		const Math::XMVECTORF32 upConstant = { 0.f,1.f,0.f,0.f };
		fVector4	up = upConstant;
		fVector4	item_position_sse = Math::XMLoadFloat4(&item_position);

		lookAtFast(mPosition, item_position_sse, up);
	}

	void Camera::lookAt(Vector4 item_position, Vector4 up) {
		fVector4	up_sse = Math::XMLoadFloat4(&up);
		fVector4	lookat_what = Math::XMLoadFloat4(&item_position);
		lookAtFast(mPosition, lookat_what, up_sse);
	}

	void Camera::lookAt(Vector4 eye_position, Vector4 item_position, Vector4 up) {
		mPosition = Math::XMLoadFloat4(&eye_position);
		fVector4	item_pos_sse = Math::XMLoadFloat4(&item_position);
		fVector4	up_sse = Math::XMLoadFloat4(&up);
		mViewMatrix = Math::XMMatrixLookAtLH(mPosition, item_pos_sse, up_sse);
		mQuatRotation =  m::XMQuaternionRotationMatrix(m::XMMatrixTranspose(mViewMatrix));
	}

	void Camera::setPosition(Vector4 position, Vector4 rotq) {
		mPosition = m::XMLoadFloat4(&position);
		mQuatRotation = m::XMLoadFloat4(&rotq);
		calcView();
	}


	void Camera::setPosition(Vector4 position) {
		mPosition = m::XMLoadFloat4(&position);
		calcView();
	}


	void Camera::setPosition(Math::FXMVECTOR position, Math::FXMVECTOR qrotation) {
		mPosition = position;
		mQuatRotation = qrotation;
		calcView();
	}


	void Camera::calcView()
	{
		m::XMMATRIX rotationMatrix = m::XMMatrixTranspose(m::XMMatrixRotationQuaternion(mQuatRotation));
		m::XMMATRIX translationMatrix = m::XMMatrixTranslationFromVector(mPosition);

		mViewMatrix = m::XMMatrixMultiply(translationMatrix,rotationMatrix);
	}

	void Camera::setRotationEuler(Vector3 rotEuler, Vector4 position)
	{
		mQuatRotation = DirectX::XMQuaternionRotationRollPitchYaw(rotEuler.x, rotEuler.y, rotEuler.z);
		mPosition = m::XMLoadFloat4(&position);
		calcView();
	}

	void Camera::setRotationEuler(Vector3 rotEuler)
	{
		mQuatRotation = DirectX::XMQuaternionRotationRollPitchYaw(rotEuler.x, rotEuler.y, rotEuler.z);
		calcView();
	}


	void Camera::setRotation(DirectX::XMFLOAT4 rotQuat) {
		mQuatRotation = m::XMLoadFloat4(&rotQuat);
		calcView();
	}

	void Camera::setRotation(Math::FXMVECTOR rotQuat) {
		mQuatRotation = rotQuat;
		calcView();
	}


	void Camera::setPerspective(float width, float height, float vnear, float vfar)
	{
		mProjMatrix = DirectX::XMMatrixPerspectiveLH(width, height, vnear, vfar);
	}

	void Camera::setPerspectiveFOV(float fovAngleY, float aspectRatio, float vnear, float vfar)
	{
		mProjMatrix = DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, vnear, vfar);
	}

	void Camera::setOrthographic(float width, float height, float vnear, float vfar)
	{
		mProjMatrix = DirectX::XMMatrixOrthographicLH(width, height, vnear, vfar);
	}
}