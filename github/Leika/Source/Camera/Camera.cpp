#include "Camera.h"
#include "../Stdafx/stdafx.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Camera::Camera()
	: fovY_(XMConvertToRadians(30.0f))
	,aspect_(640.0f / 480.0f)
	,nearClip_(0.1f)
	,farClip_(100.0f)
{
	viewmat_ = Matrix::Identity;
	eyepos_ = Vector3(0.0f, 10.0f, -20.0f);
	refpos_ = Vector3(0.0f, 2.0f, -20.0f);
	upvec_ = Vector3(0.0f, 1.0f, 0.0f);
	projmat_ = Matrix::Identity;
}

Camera::~Camera()
{
}

void Camera::UpCamera()
{
	Vector3 eyepos, refpos;
	const float CAMERA_DISTANCE = 50.0f;
	const Vector3 cameraV = Vector3(0.0f, CAMERA_DISTANCE, -0.1f);

	eyepos = refpos_ + cameraV;

	eyepos = eyepos_ + (eyepos - eyepos_) * 0.03f;
	refpos = refpos_ + (refpos - refpos_) * 0.03f;

	refpos += Vector3(0.0f, 0.5f, 0.f);
	eyepos += Vector3(0.0f, 0.5f, 0.f);
	eyepos_ = eyepos;
	refpos_ = refpos;

	upvec_ = Vector3::TransformNormal(Vector3(0, 1, 0), Matrix::Identity);
}

void Camera::Update()
{
	// ビュー行列を作る
	viewmat_ = Matrix::CreateLookAt(eyepos_, refpos_, upvec_);
	// 射影行列を作る
	projmat_ = Matrix::CreatePerspectiveFieldOfView(fovY_, aspect_, nearClip_, farClip_);
}