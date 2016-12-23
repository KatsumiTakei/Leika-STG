#pragma once

class Camera
{
private:
#pragma region instance
	// �r���[�s��
	DirectX::SimpleMath::Matrix viewmat_;
	// �J�������W
	DirectX::SimpleMath::Vector3 eyepos_;
	// �^�[�Q�b�g���W
	DirectX::SimpleMath::Vector3 refpos_;
	// ������x�N�g��
	DirectX::SimpleMath::Vector3 upvec_;
	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix projmat_;
	// �c��������p
	float fovY_;
	// �A�X�y�N�g��
	float aspect_;
	// �j�A�N���b�v
	float nearClip_;
	// �t�@�[�O���b�v
	float farClip_;
#pragma endregion
public:

	Camera();
	virtual ~Camera();

	void UpCamera();

	void Update();

#pragma region setter and getter
	inline void SetEyepos(const DirectX::SimpleMath::Vector3 pos) { eyepos_ = pos; }
	inline void SetRefpos(const DirectX::SimpleMath::Vector3 pos) { refpos_ = pos; }
	inline void SetUpvec(const DirectX::SimpleMath::Vector3 pos) { upvec_ = pos; }
	inline void SetAspect(const float aspect) { aspect_ = aspect; }
	inline void SetFovY(const float fovY) { fovY_ = fovY; }
	inline void SetNearCrip(const float nearclip) { nearClip_ = nearclip; }
	inline void SetFarCrip(const float farclip) { farClip_ = farclip; }
	inline const DirectX::SimpleMath::Vector3& GetEyepos()const { return eyepos_; }
	inline const DirectX::SimpleMath::Vector3& GetRefpos()const { return refpos_; }
	inline const DirectX::SimpleMath::Vector3& GetUpvec()const { return upvec_; }
	inline const DirectX::SimpleMath::Matrix&GetViewmat()const { return viewmat_; }
	inline const DirectX::SimpleMath::Matrix& GetProjmat()const { return projmat_; }
#pragma endregion

};
