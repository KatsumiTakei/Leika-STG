#include "Sound.h"
#include "OrgSound.h"
#include <fstream>
#include <vector>

#ifdef __cplusplus_cli
#pragma unmanaged
#endif // __cplusplus_cli

using namespace std;

class Sound::Impl
{
public:
	Impl()
	{
	}
	~Impl()
	{
	}

	OrgSound sound;

	bool Create(HWND hwnd)
	{
		return sound.Create(hwnd);
	}
	
	bool Load(uint32_t index,const wchar_t* filename)
	{
		// �t�@�C�����J��
		ifstream in(filename, ios::in | ios::binary);

		if (!in) return false;

		in.seekg(0,ios::end);
		auto size = in.tellg();
		in.clear();
		in.seekg(0,ios::beg);

		vector<char> buff((uint32_t)size);

		in.read(buff.data(), size);

		return sound.CreateBuffer(buff.data(), index);
	}
};

Sound::Sound() : pImpl(new Impl())
{
}

Sound::~Sound()
{
}


bool Sound::Load(uint32_t index,const wchar_t* filename)
{
	return pImpl->Load(index, filename);
}
void Sound::Play(uint32_t index)
{
	pImpl->sound.Play(index, 0, false);
}
void Sound::PlayLoop(uint32_t index)
{
	pImpl->sound.Play(index, 0, true);
}
void Sound::Stop(uint32_t index)
{
	pImpl->sound.Stop(index);
}
bool Sound::Create(HWND hwnd)
{
	return pImpl->Create(hwnd);
}
void Sound::Release()
{
	pImpl->sound.Release();
}

// �T�E���h�̃C���X�^���X
static OrgSound sound ;

//�T�E���h�̃��[�h
void SOUND_Load(uint32_t index,const char* filename)
{
	FILE* fp = NULL ; // �t�@�C���|�C���^���i�[����
	int size ; // �t�@�C���T�C�Y���i�[����
	void* buffer ; // �o�b�t�@�̃|�C���^���i�[����

    // �t�@�C���̃I�[�v��
	if( fopen_s(&fp,filename,"rb") != 0 ) return ;

    // �t�@�C���I�[�v�����s
	if( fp == NULL ) return ;

    // �t�@�C���T�C�Y�𒲂ׂ�
	fseek(fp,0,SEEK_END);
	size = ftell(fp);
	fseek(fp,0,SEEK_SET);

    // �t�@�C���T�C�Y�Ɠ����o�b�t�@���m�ۂ���
	buffer = malloc(size);
	if( buffer == NULL )
	{
        // �o�b�t�@�̊m�ێ��s
        // �t�@�C�����N���[�Y����
		fclose(fp);
		return ;
	}

    // �t�@�C����ǂݍ���
	if( fread(buffer,size,1,fp) != 1 )
	{
        // �ǂݍ��݂̎��s
        // �t�@�C������A�o�b�t�@���J������
		fclose(fp);
		free(buffer);
		return ;
	}

    // �o�b�t�@�����ɁA�T�E���h�o�b�t�@���쐬����
	sound.CreateBuffer(buffer,index);

    // ����I��
    // �t�@�C�����
    // �o�b�t�@���J������
	fclose(fp);
	free(buffer);

	return ;
}


// �T�E���h�̍Đ�
void SOUND_Play(int index)
{
	sound.Play(index);
}

// �T�E���h�̃��[�v�Đ�
void SOUND_PlayLoop(int index)
{
	sound.Play(index,0,TRUE);
}

// �T�E���h�̍Đ�
void SOUND_Stop(int index)
{
	sound.Stop(index);
}

// �T�E���h�̍쐬
void SOUND_Create(HWND hwnd)
{

    // DirectX�̃f�o�C�X����A�E�C���h�E�n���h�����擾
//	D3DDEVICE_CREATION_PARAMETERS cd ;
//	d->GetCreationParameters(&cd);

    // �T�E���h�I�u�W�F�N�g�̍쐬
	sound.Create(hwnd);
}

// �T�E���h�̃����[�X
void SOUND_Release()
{
    // �T�E���h�I�u�W�F�N�g�̊J��
	sound.Release();
}