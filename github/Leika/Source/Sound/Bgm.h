#pragma once
#ifndef _BGM_H_
#define _BGM_H_

// mciを使った再生クラス

#ifdef __cplusplus_cli
#pragma unmanaged
#endif

/// <summary>
/// BGM関連のクラス
/// </summary>
class Bgm
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    Bgm();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~Bgm();

    /// <summary>
    /// 内部オブジェクトを作成します。
    /// </summary>
    /// <returns>TRUEで成功</returns>
    bool Create();

    /// <summary>
    /// 内部オブジェクトを開放します
    /// </summary>
    void Release();

    /// <summary>
    /// メディアをロードする
    /// </summary>
    /// <param name="filename">メディアへのパス</param>
    /// <returns>TRUEで成功</returns>
    bool Loab(wchar_t* filename);


    /// <summary>
    /// メディアをアンロードする
    /// </summary>
    /// <returns>TRUEで成功</returns>
    bool Unload();
   
    /// <summary>
    /// メディアを再生する(繰り返し)
    /// </summary>
    /// <returns>TRUEで成功</returns>
    bool Play();

    /// <summary>
    /// 再生中のメディアを停止する
    /// </summary>
    /// <returns>TRUEで成功</returns>
    bool Stop();

    /// <summary>
    /// 再生中のメディアをポーズする
    /// </summary>
    /// <returns>TRUEで成功</returns>
    bool Pause();

    /// <summary>
    /// ポーズ中のメディアを再生する
    /// </summary>
    /// <returns>TRUEで成功</returns>
    bool Resume();

    /// <summary>
    /// ボリューム変更
    /// </summary>
    /// <param name="volume">ボリューム 0～1000</param>
    /// <returns>TRUEで成功</returns>
	bool Volume(int volume);

};

#ifdef __cplusplus_cli
#pragma managed
#endif


#endif // _BGM_H_