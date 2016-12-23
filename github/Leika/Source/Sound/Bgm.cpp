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
/// MCIのコマンド生成クラス
/// </summary>
/// <remarks>
/// 内部バッファにコマンドの文字列を作成する
/// </remarks>
class CCreateMCICommand
{
private:

    /// <summary>作成したコマンドを入れる</summary>
    wchar_t command[2048];
    /// <summary>エイリアスの指定</summary>
    wchar_t alias[512];

    /// <summary>
    /// コマンドを生成する
    /// </summary>
    /// <param name="cmd">コマンド</param>
    /// <param name="alias">エイリアス</param>
    /// <param name="num">ナンバー</param>
    void Command(wchar_t* cmd,wchar_t* alias,DWORD num)
    {
        swprintf_s( this->command, sizeof(command), L"%s %s%d", cmd, alias, num );
    }

public:
    /// <summary>
    /// エイリアスを設定する
    /// </summary>
    /// <param name="alias">エイリアス</param>
    CCreateMCICommand(wchar_t* alias)
    {
		wcscpy_s(this->alias, sizeof(this->alias), alias);
    }

    /// <summary>
    /// メディアをロードする
    /// </summary>
    /// <param name="num">ナンバー</param>
    /// <param name="filename">メディアのパス</param>
    void Open(DWORD num,wchar_t* filename)
    {
        _stprintf_s(this->command,sizeof(this->command),L"Open \"%s\" alias %s%d",filename,this->alias,num);
    }

    /// <summary>
    /// メディアを再生するコマンドを作成する
    /// </summary>
    /// <param name="num">ナンバー</param>
    void Play(DWORD num)
    {
        Command(L"Play",this->alias,num);
    };

    /// <summary>
    /// メディアを停止するコマンドを作成する
    /// </summary>
    /// <param name="num">ナンバー</param>
    void Stop(DWORD num)
    {
        Command(L"Stop",this->alias,num);
    }

    /// <summary>
    /// メディアをポーズするコマンドを作成する
    /// </summary>
    /// <param name="num">ナンバー</param>
    void Pause(DWORD num)
    {
        Command(L"Pause",this->alias,num);
    }

    /// <summary>
    /// ポーズしたメディアを再生するコマンドを作成する
    /// </summary>
    /// <param name="num">ナンバー</param>
    void Resume(DWORD num)
    {
        Command(L"Resume",this->alias,num);
    }

    /// <summary>
    /// メディアをクローズするコマンドを作成する
    /// </summary>
    /// <param name="num">ナンバー</param>
    void Close(DWORD num)
    {
        Command(L"Close",this->alias,num);
    }

};

// コンストラクタ
Bgm::Bgm()
{

}


// デストラクタ
Bgm::~Bgm()
{

}


// 内部オブジェクトの作成
bool Bgm::Create()
{

    return true ;
}

// 内部オブジェクトの開放
void Bgm::Release()
{
    
}

// ファイルのロード
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

        // TODO このエラーメッセージをどこで使用するか決めていない

        return false;
    }

    return true;
}

// 読み込んだデータのアンロード
bool Bgm::Unload()
{
    MCIERROR err_code ;
    err_code = mciSendString(L"Close Music", NULL, 0, NULL );
    if( err_code != 0 ) return false;
    return true;
}

// BGMのプレイ
bool Bgm::Play()
{
    MCIERROR err_code ;
    err_code = mciSendString( L"Seek Music to start", NULL, 0, NULL );
    if( err_code != 0 ) return false;
    err_code = mciSendString( L"Play Music Repeat", NULL, 0, NULL );
    if( err_code != 0 ) return false;
    return true;
}

// 再生中のBGMを停止
bool Bgm::Stop()
{
    MCIERROR err_code ;
    err_code = mciSendString( L"Stop Music", NULL, 0, NULL );
    if( err_code != 0 ) return false;
    return true;
}

// BGMの一時停止
bool Bgm::Pause()
{
    MCIERROR err_code ;
    err_code = mciSendString( L"Pause Music", NULL, 0, NULL );
    if( err_code != 0 ) return false;
    return true;
}

// 一時停止中のBGMを再生
bool Bgm::Resume()
{
    MCIERROR err_code ;
    err_code = mciSendString( L"Resume Music", NULL, 0, NULL );
    if( err_code != 0 ) return false;
    return true ;
}

// ボリュームを設定
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

// 参考
//char buff[ 512 ];
//char *file = "C:\\My Music\\music.wma";　←ファイル名(WMA,MID,MP3,WAVなど)
//
//sprintf( buff, "Open \"%s\" alias Music", file );
//mciSendString( buff, NULL, 0, NULL );　←メディアを開く
//getchar();
//mciSendString( "Play Music Repeat", NULL, 0, NULL );　←再生(繰り返し)
//getchar();
//mciSendString( "Pause Music", NULL, 0, NULL );　←一時停止
//getchar();
//mciSendString( "Resume Music", NULL, 0, NULL );　←一時停止から再開
//getchar();
//mciSendString( "Stop Music", NULL, 0, NULL );　←停止
//getchar();
//mciSendString( "Close Music", NULL, 0, NULL );　←メディアを閉じる


#ifdef __cplusplus_cli
#pragma managed
#endif