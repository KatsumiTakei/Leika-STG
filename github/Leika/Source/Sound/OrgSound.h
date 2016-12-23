
#ifndef __ORGSND_H__
#define __ORGSND_H__

#include <windows.h>
#include <windef.h>
#include <memory>
#include <cstdint>

class OrgSound  // DirectSoundの管理クラス
{
public:
	OrgSound();			// コンストラクタ
	virtual ~OrgSound();	// デストラクタ

	void SetPan(uint32_t index, int pan);		// パンコントロール
	void SetVolume(uint32_t index,int volume);	// ボリュームコントロール
	void Release();			// オブジェクトを解放
	int  SolvePause();		// ポーズを解除する
	bool Stop(uint32_t uIndex);	// 指定の音を停止します
	int  GetPauseNum();	// ポーズされているバッファの数を取得
	bool IsPause();		// ポーズされているか？
	int  PauseAll();		// すべての音をポーズする
	bool Replay(uint32_t uIndex);	// ストップした位置から再生する
	bool Play(uint32_t uIndex, int iPosition = 0, bool bLoop = FALSE);	// 指定した位置から再生する
	void ReleaseBufferAll();			// すべてのバッファを開放する
	void ReleaseBuffer(uint32_t uIndex);	// 指定したインデックスのバッファを開放
	bool Create(HWND hWnd);		// インターフェイスを作成
	bool CreateBuffer(void* lpBuf,uint32_t uIndex);	// 指定インデックスにバッファを作成
	
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;

};

#endif // __ORGSND_H__