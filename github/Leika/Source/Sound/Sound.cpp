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
		// ファイルを開く
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

// サウンドのインスタンス
static OrgSound sound ;

//サウンドのロード
void SOUND_Load(uint32_t index,const char* filename)
{
	FILE* fp = NULL ; // ファイルポインタを格納する
	int size ; // ファイルサイズを格納する
	void* buffer ; // バッファのポインタを格納する

    // ファイルのオープン
	if( fopen_s(&fp,filename,"rb") != 0 ) return ;

    // ファイルオープン失敗
	if( fp == NULL ) return ;

    // ファイルサイズを調べる
	fseek(fp,0,SEEK_END);
	size = ftell(fp);
	fseek(fp,0,SEEK_SET);

    // ファイルサイズと同じバッファを確保する
	buffer = malloc(size);
	if( buffer == NULL )
	{
        // バッファの確保失敗
        // ファイルをクローズする
		fclose(fp);
		return ;
	}

    // ファイルを読み込む
	if( fread(buffer,size,1,fp) != 1 )
	{
        // 読み込みの失敗
        // ファイルを閉じ、バッファを開放する
		fclose(fp);
		free(buffer);
		return ;
	}

    // バッファを元に、サウンドバッファを作成する
	sound.CreateBuffer(buffer,index);

    // 正常終了
    // ファイルを閉じ
    // バッファを開放する
	fclose(fp);
	free(buffer);

	return ;
}


// サウンドの再生
void SOUND_Play(int index)
{
	sound.Play(index);
}

// サウンドのループ再生
void SOUND_PlayLoop(int index)
{
	sound.Play(index,0,TRUE);
}

// サウンドの再生
void SOUND_Stop(int index)
{
	sound.Stop(index);
}

// サウンドの作成
void SOUND_Create(HWND hwnd)
{

    // DirectXのデバイスから、ウインドウハンドルを取得
//	D3DDEVICE_CREATION_PARAMETERS cd ;
//	d->GetCreationParameters(&cd);

    // サウンドオブジェクトの作成
	sound.Create(hwnd);
}

// サウンドのリリース
void SOUND_Release()
{
    // サウンドオブジェクトの開放
	sound.Release();
}