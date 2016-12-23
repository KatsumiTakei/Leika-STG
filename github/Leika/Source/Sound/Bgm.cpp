#include "Bgm.h"
#include <Windows.h>
#include <mmsystem.h>
#include <string>
#include <tchar.h>

#pragma comment(lib,"winmm.lib")

#ifdef __cplusplus_cli
#pragma unmanaged
#endif

/// <summary>
/// MCI�̃R�}���h�����N���X
/// </summary>
/// <remarks>
/// �����o�b�t�@�ɃR�}���h�̕�������쐬����
/// </remarks>
class CCreateMCICommand
{
private:

    /// <summary>�쐬�����R�}���h������</summary>
    wchar_t command[2048];
    /// <summary>�G�C���A�X�̎w��</summary>
    wchar_t alias[512];

    /// <summary>
    /// �R�}���h�𐶐�����
    /// </summary>
    /// <param name="cmd">�R�}���h</param>
    /// <param name="alias">�G�C���A�X</param>
    /// <param name="num">�i���o�[</param>
    void Command(wchar_t* cmd,wchar_t* alias,DWORD num)
    {
        swprintf_s( this->command, sizeof(command), L"%s %s%d", cmd, alias, num );
    }

public:
    /// <summary>
    /// �G�C���A�X��ݒ肷��
    /// </summary>
    /// <param name="alias">�G�C���A�X</param>
    CCreateMCICommand(wchar_t* alias)
    {
		wcscpy_s(this->alias, sizeof(this->alias), alias);
    }

    /// <summary>
    /// ���f�B�A�����[�h����
    /// </summary>
    /// <param name="num">�i���o�[</param>
    /// <param name="filename">���f�B�A�̃p�X</param>
    void Open(DWORD num,wchar_t* filename)
    {
        _stprintf_s(this->command,sizeof(this->command),L"Open \"%s\" alias %s%d",filename,this->alias,num);
    }

    /// <summary>
    /// ���f�B�A���Đ�����R�}���h���쐬����
    /// </summary>
    /// <param name="num">�i���o�[</param>
    void Play(DWORD num)
    {
        Command(L"Play",this->alias,num);
    };

    /// <summary>
    /// ���f�B�A���~����R�}���h���쐬����
    /// </summary>
    /// <param name="num">�i���o�[</param>
    void Stop(DWORD num)
    {
        Command(L"Stop",this->alias,num);
    }

    /// <summary>
    /// ���f�B�A���|�[�Y����R�}���h���쐬����
    /// </summary>
    /// <param name="num">�i���o�[</param>
    void Pause(DWORD num)
    {
        Command(L"Pause",this->alias,num);
    }

    /// <summary>
    /// �|�[�Y�������f�B�A���Đ�����R�}���h���쐬����
    /// </summary>
    /// <param name="num">�i���o�[</param>
    void Resume(DWORD num)
    {
        Command(L"Resume",this->alias,num);
    }

    /// <summary>
    /// ���f�B�A���N���[�Y����R�}���h���쐬����
    /// </summary>
    /// <param name="num">�i���o�[</param>
    void Close(DWORD num)
    {
        Command(L"Close",this->alias,num);
    }

};

// �R���X�g���N�^
Bgm::Bgm()
{

}


// �f�X�g���N�^
Bgm::~Bgm()
{

}


// �����I�u�W�F�N�g�̍쐬
bool Bgm::Create()
{

    return true ;
}

// �����I�u�W�F�N�g�̊J��
void Bgm::Release()
{
    
}

// �t�@�C���̃��[�h
bool Bgm::Loab(wchar_t* filename)
{
    TCHAR str[2048];
    MCIERROR err_code ;
    
    swprintf_s(str,L"open %s type mpegvideo alias Music",filename);

    TCHAR path[2048];
    GetCurrentDirectory(sizeof(path),path);

    err_code =  mciSendString( str, NULL, 0, NULL );
    if( err_code != 0 )
    {
        TCHAR error[2048];
        mciGetErrorString( err_code, error, sizeof(error) );

        // TODO ���̃G���[���b�Z�[�W���ǂ��Ŏg�p���邩���߂Ă��Ȃ�

        return false;
    }

    return true;
}

// �ǂݍ��񂾃f�[�^�̃A�����[�h
bool Bgm::Unload()
{
    MCIERROR err_code ;
    err_code = mciSendString(L"Close Music", NULL, 0, NULL );
    if( err_code != 0 ) return false;
    return true;
}

// BGM�̃v���C
bool Bgm::Play()
{
    MCIERROR err_code ;
    err_code = mciSendString( L"Seek Music to start", NULL, 0, NULL );
    if( err_code != 0 ) return false;
    err_code = mciSendString( L"Play Music Repeat", NULL, 0, NULL );
    if( err_code != 0 ) return false;
    return true;
}

// �Đ�����BGM���~
bool Bgm::Stop()
{
    MCIERROR err_code ;
    err_code = mciSendString( L"Stop Music", NULL, 0, NULL );
    if( err_code != 0 ) return false;
    return true;
}

// BGM�̈ꎞ��~
bool Bgm::Pause()
{
    MCIERROR err_code ;
    err_code = mciSendString( L"Pause Music", NULL, 0, NULL );
    if( err_code != 0 ) return false;
    return true;
}

// �ꎞ��~����BGM���Đ�
bool Bgm::Resume()
{
    MCIERROR err_code ;
    err_code = mciSendString( L"Resume Music", NULL, 0, NULL );
    if( err_code != 0 ) return false;
    return true ;
}

// �{�����[����ݒ�
bool Bgm::Volume(int volume)
{
    MCIERROR err_code ;
	TCHAR str[2048];
	if( volume < 0 ) volume = 0;
	if( volume > 1000 ) volume = 1000;
	swprintf_s(str,L"setaudio Music volume to %d",volume);
    err_code = mciSendString( str, NULL, 0, NULL );
    if( err_code != 0 ) return false;
    return true ;
}

// �Q�l
//char buff[ 512 ];
//char *file = "C:\\My Music\\music.wma";�@���t�@�C����(WMA,MID,MP3,WAV�Ȃ�)
//
//sprintf( buff, "Open \"%s\" alias Music", file );
//mciSendString( buff, NULL, 0, NULL );�@�����f�B�A���J��
//getchar();
//mciSendString( "Play Music Repeat", NULL, 0, NULL );�@���Đ�(�J��Ԃ�)
//getchar();
//mciSendString( "Pause Music", NULL, 0, NULL );�@���ꎞ��~
//getchar();
//mciSendString( "Resume Music", NULL, 0, NULL );�@���ꎞ��~����ĊJ
//getchar();
//mciSendString( "Stop Music", NULL, 0, NULL );�@����~
//getchar();
//mciSendString( "Close Music", NULL, 0, NULL );�@�����f�B�A�����


#ifdef __cplusplus_cli
#pragma managed
#endif