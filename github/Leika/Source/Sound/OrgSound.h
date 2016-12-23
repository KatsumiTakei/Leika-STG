
#ifndef __ORGSND_H__
#define __ORGSND_H__

#include <windows.h>
#include <windef.h>
#include <memory>
#include <cstdint>

class OrgSound  // DirectSound�̊Ǘ��N���X
{
public:
	OrgSound();			// �R���X�g���N�^
	virtual ~OrgSound();	// �f�X�g���N�^

	void SetPan(uint32_t index, int pan);		// �p���R���g���[��
	void SetVolume(uint32_t index,int volume);	// �{�����[���R���g���[��
	void Release();			// �I�u�W�F�N�g�����
	int  SolvePause();		// �|�[�Y����������
	bool Stop(uint32_t uIndex);	// �w��̉����~���܂�
	int  GetPauseNum();	// �|�[�Y����Ă���o�b�t�@�̐����擾
	bool IsPause();		// �|�[�Y����Ă��邩�H
	int  PauseAll();		// ���ׂẲ����|�[�Y����
	bool Replay(uint32_t uIndex);	// �X�g�b�v�����ʒu����Đ�����
	bool Play(uint32_t uIndex, int iPosition = 0, bool bLoop = FALSE);	// �w�肵���ʒu����Đ�����
	void ReleaseBufferAll();			// ���ׂẴo�b�t�@���J������
	void ReleaseBuffer(uint32_t uIndex);	// �w�肵���C���f�b�N�X�̃o�b�t�@���J��
	bool Create(HWND hWnd);		// �C���^�[�t�F�C�X���쐬
	bool CreateBuffer(void* lpBuf,uint32_t uIndex);	// �w��C���f�b�N�X�Ƀo�b�t�@���쐬
	
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;

};

#endif // __ORGSND_H__