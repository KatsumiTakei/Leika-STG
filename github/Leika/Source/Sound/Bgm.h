#pragma once
#ifndef _BGM_H_
#define _BGM_H_

// mci���g�����Đ��N���X

#ifdef __cplusplus_cli
#pragma unmanaged
#endif

/// <summary>
/// BGM�֘A�̃N���X
/// </summary>
class Bgm
{
public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    Bgm();

    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
    ~Bgm();

    /// <summary>
    /// �����I�u�W�F�N�g���쐬���܂��B
    /// </summary>
    /// <returns>TRUE�Ő���</returns>
    bool Create();

    /// <summary>
    /// �����I�u�W�F�N�g���J�����܂�
    /// </summary>
    void Release();

    /// <summary>
    /// ���f�B�A�����[�h����
    /// </summary>
    /// <param name="filename">���f�B�A�ւ̃p�X</param>
    /// <returns>TRUE�Ő���</returns>
    bool Loab(wchar_t* filename);


    /// <summary>
    /// ���f�B�A���A�����[�h����
    /// </summary>
    /// <returns>TRUE�Ő���</returns>
    bool Unload();
   
    /// <summary>
    /// ���f�B�A���Đ�����(�J��Ԃ�)
    /// </summary>
    /// <returns>TRUE�Ő���</returns>
    bool Play();

    /// <summary>
    /// �Đ����̃��f�B�A���~����
    /// </summary>
    /// <returns>TRUE�Ő���</returns>
    bool Stop();

    /// <summary>
    /// �Đ����̃��f�B�A���|�[�Y����
    /// </summary>
    /// <returns>TRUE�Ő���</returns>
    bool Pause();

    /// <summary>
    /// �|�[�Y���̃��f�B�A���Đ�����
    /// </summary>
    /// <returns>TRUE�Ő���</returns>
    bool Resume();

    /// <summary>
    /// �{�����[���ύX
    /// </summary>
    /// <param name="volume">�{�����[�� 0�`1000</param>
    /// <returns>TRUE�Ő���</returns>
	bool Volume(int volume);

};

#ifdef __cplusplus_cli
#pragma managed
#endif


#endif // _BGM_H_