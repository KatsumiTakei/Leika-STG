#include "OrgSound.h"
#include <windows.h>
#include <MMREG.H>
#include <Msacm.h>	// ACM

#include <mmsystem.h>
#include <dSound.h>
#include <vector>

#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"MsAcm32.lib")

#include <wrl.h>

#ifdef __cplusplus_cli
#pragma unmanaged
#endif // __cplusplus_cli

using namespace std;
using namespace Microsoft::WRL;



class OrgSound::Impl
{
public:
//	LPDIRECTSOUND			m_lpSound;		// インターフェース
//	LPDIRECTSOUNDBUFFER		m_lpPrimaryBuf;	// プライマリバッファ

	ComPtr<IDirectSound>		m_lpSound;
	ComPtr<IDirectSoundBuffer>  m_lpPrimaryBuf;

	//	CTypedPtrArray<CPtrArray,LPDIRECTSOUNDBUFFER> m_BufferArray;	// バッファテーブル
	//	CArray<int,int&> m_BufPauseArray;		// ポーズ状態

//	typedef vector<LPDIRECTSOUNDBUFFER> SOUNDBUFFERARRAY;
	typedef vector<ComPtr<IDirectSoundBuffer>> SOUNDBUFFERARRAY;
	SOUNDBUFFERARRAY m_BufferArray;	// バッファテーブル
	typedef vector<int> PAUSEARRAY;
	PAUSEARRAY m_BufPauseArray;		// ポーズ状態


	typedef struct tORGWAVEINFO
	{
		LPWAVEFORMATEX	lpFormat;
		uint8_t*			lpBuffer;
		uint32_t			dwBufferSize;

	} ORGWAVEINFO, *LPORGWAVEINFO;

	// ポーズ状態
	enum nPAUSE
	{
		PAUSE_NONE,		// どの状態でもない
		PAUSE_PLAY,		// 再生中
		PAUSE_LOOPING,	// ループで再生中
	};



	///////////////////////////////////////////
	/// OrgSound::OrgSound()
	// 
	//  機能       : コンストラクタ
	// 
	////////////////////////////////////////////
	Impl()
	{
		//m_lpSound = NULL;
		//m_lpPrimaryBuf = NULL;
	}

	///////////////////////////////////////////
	/// OrgSound()
	// 
	//  機能       : デストラクタ
	// 
	////////////////////////////////////////////
	~Impl()
	{
		Release();
	}

	///////////////////////////////////////////
	/// OrgSound::CreateBuffer()
	// 
	//  機能       : データテーブル解析し、WAVEデータならば
	//               新しくバッファを作る
	// 
	//  戻り値     : バッファのポインタ
	// 
	//  引数       : 
	// 
	//  呼び出し元 : CreateBuffer
	// 
	//  備考       : 
	// 
	//  作成者     : 
	// 
	////////////////////////////////////////////
	LPDIRECTSOUNDBUFFER CreateBuffer(LPVOID lpBuf)	// Wave形式のデータ
	{
		LPDWORD pdwBuf = (DWORD*)lpBuf;
		LPDWORD pdwEnd;
		DWORD dwRiff;		//
		DWORD dwLength;		//
		DWORD dwType;		//

		// ファイルを確認する
		dwRiff = *pdwBuf++;
		dwLength = *pdwBuf++;
		dwType = *pdwBuf++;

		if (dwRiff != mmioFOURCC('R', 'I', 'F', 'F'))
			return NULL;

		if (dwType != mmioFOURCC('W', 'A', 'V', 'E'))
			return NULL;

		pdwEnd = (LPDWORD)((LPBYTE)pdwBuf + dwLength - 4);

		// ファイルから必要な情報を得る
		LPWAVEFORMATEX pWaveHeader = NULL;	// ＷＡＶＥヘッダ
		LPBYTE  pbWaveData = NULL;			// ＷＡＶＥデータ
		DWORD cbWaveSize = 0;				// データサイズ

		while (pdwBuf < pdwEnd)
		{
			dwType = *pdwBuf++;
			dwLength = *pdwBuf++;

			switch (dwType)
			{
			case mmioFOURCC('f', 'm', 't', ' '):
				// ヘッダ取得
				if (pWaveHeader == NULL)
				{
					if (dwLength < sizeof(WAVEFORMAT))
						return FALSE;
					pWaveHeader = (LPWAVEFORMATEX)pdwBuf;
				}
				break;
			case mmioFOURCC('d', 'a', 't', 'a'):
				// データ取得
				if ((pbWaveData == NULL) || (!cbWaveSize))
				{
					pbWaveData = (LPBYTE)pdwBuf;
					cbWaveSize = dwLength;
				}
				break;
			}
			if (pWaveHeader && (pbWaveData != NULL) && cbWaveSize)
				break;

			pdwBuf = (LPDWORD)((LPBYTE)pdwBuf + ((dwLength + 1) & ~1));
		}

		if (pdwEnd <= pdwBuf)
			return NULL;		// サウンドデータではなかった

		if (pWaveHeader->wFormatTag != WAVE_FORMAT_PCM)
		{
			// PCM以外のフォーマットだったときの処理
			// PCMに変換してからサウンドバッファを作成する

//			LPDIRECTSOUNDBUFFER pBuffer;	// サウンドバッファ
			ComPtr<IDirectSoundBuffer> pBuffer;	// サウンドバッファ
			LPORGWAVEINFO pInfo;			// 変換後のデータを受け取る
			ORGWAVEINFO src;				// 変換元の情報を渡すための変数

			// 変換元の情報
			src.dwBufferSize = cbWaveSize;
			src.lpBuffer = pbWaveData;
			src.lpFormat = pWaveHeader;

			// PCMに変換
			pInfo = ConvertFormat(&src);

			// 変換後のデータでサウンドバッファを作成
			pBuffer = CreateBuffer(pInfo->lpFormat, pInfo->lpBuffer, pInfo->dwBufferSize);

			// 変換で使ったデータを破棄
			ReleaseConvertFormat(pInfo);

			// サウンドバッファを返す(NULLの場合のある)
			return pBuffer.Detach();
		}

		// PCMデータでサウンドバッファを作成
		return CreateBuffer(pWaveHeader, pbWaveData, cbWaveSize);
	}

	///////////////////////////////////////////
	/// CreateBuffer()
	// 
	//  機能       : バッファを作成し、指定されたデータを書き込む
	// 
	//  戻り値     : バッファのポインタ
	// 
	//  引数       : 
	// 
	//  呼び出し元 : CreateBuffer
	// 
	//  備考       : 
	// 
	//  作成者     : 
	// 
	////////////////////////////////////////////
	LPDIRECTSOUNDBUFFER CreateBuffer(
		LPWAVEFORMATEX lpWaveFormat,	// Waveデータのフォーマット
		LPBYTE lpWaveData,				// Waveデータ
		DWORD dwWaveSize)				// Waveデータのサイズ
	{
		if (!m_lpSound)
			return nullptr;

		HRESULT hResult;
		DSBUFFERDESC dsbdesc;
//		LPDIRECTSOUNDBUFFER	lpDirectSoundBuffer;
		ComPtr<IDirectSoundBuffer> lpDirectSoundBuffer;

		//	if( !m_lpSound )
		//		return NULL ;

		// 構造体初期化
		ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
		dsbdesc.dwSize = sizeof(DSBUFFERDESC);
		dsbdesc.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_GETCURRENTPOSITION2;
		dsbdesc.dwBufferBytes = dwWaveSize;
		dsbdesc.lpwfxFormat = lpWaveFormat;

		// バッファ作成
		hResult = m_lpSound->CreateSoundBuffer(&dsbdesc, lpDirectSoundBuffer.GetAddressOf(), nullptr);
		if (hResult != DS_OK)
			return nullptr;

		// バッファをロック
		LPVOID pMem1, pMem2;
		DWORD	dwSize1, dwSize2;
		hResult = lpDirectSoundBuffer->Lock(0, dwWaveSize, &pMem1, &dwSize1, &pMem2, &dwSize2, 0);
		if (hResult != DS_OK)
		{
			//lpDirectSoundBuffer->Release();
			return nullptr;
		}

		// バッファをコピー
		CopyMemory(pMem1, lpWaveData, dwSize1);
		if (0 != dwSize2)
			CopyMemory(pMem2, lpWaveData + dwSize1, dwSize2);

		// ロックを解除
		lpDirectSoundBuffer->Unlock(pMem1, dwSize1, pMem2, dwSize2);

		return lpDirectSoundBuffer.Detach();
	}

	///////////////////////////////////////////
	/// Create()
	// 
	//  機能       : オブジェクトの構築
	// 
	//  戻り値     : TRUE 成功
	// 
	//  引数       : 
	// 
	//  呼び出し元 : 
	// 
	//  備考       : DirectSoundのインターフェイスを協調モードで作成します。
	//               
	// 
	//  作成者     : 
	// 
	////////////////////////////////////////////
	bool Create(HWND hWnd)
	{
		//LPDIRECTSOUND lpSound;
		//LPDIRECTSOUNDBUFFER lpBuffer;
		ComPtr<IDirectSound> lpSound;
		ComPtr<IDirectSoundBuffer> lpBuffer;
		HRESULT hResult;

		// すでに作成済みである
		if ((m_lpSound != NULL) || (m_lpPrimaryBuf != NULL))
			return false;

		// インターフェイスを作成
		hResult = DirectSoundCreate(NULL, lpSound.GetAddressOf(), NULL);
		if (hResult != DS_OK)
		{
			// インターフェース作成に失敗
			return false;
		}

		// 協調レベルを設定
		hResult = lpSound->SetCooperativeLevel(hWnd, DSSCL_NORMAL);
		if (hResult != DS_OK)
		{
			// 協調レベルの設定に失敗
			return false;
		}


		DSBUFFERDESC dsbdesc;

		ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));

		dsbdesc.dwSize = sizeof(DSBUFFERDESC);
		dsbdesc.dwFlags = /*DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY |*/ DSBCAPS_PRIMARYBUFFER;
		dsbdesc.dwBufferBytes = 0;
		dsbdesc.lpwfxFormat = NULL;

		// バッファ作成
		hResult = lpSound->CreateSoundBuffer(&dsbdesc, lpBuffer.GetAddressOf(), NULL);
		if (FAILED(hResult))
		{
			return false;
		}


		// 新しいインターフェイスとバッファをメンバーに設定する
		m_lpSound = lpSound;
		m_lpPrimaryBuf = lpBuffer;

		// 正常終了
		return true;

	}

	///////////////////////////////////////////
	/// CreateBuffer()
	// 
	//  機能       : 音を指定のインデックスに登録
	// 
	//  戻り値     : TRUE 成功
	// 
	////////////////////////////////////////////
	bool CreateBuffer(
		LPVOID lpBuf,	// サウンドのバッファ
		UINT uIndex)	// 登録先のインデックス
	{

		ComPtr<IDirectSoundBuffer> lpSoundBuffer;

		// バッファの開放
		ReleaseBuffer(uIndex);

		lpSoundBuffer = CreateBuffer(lpBuf);

		if (!lpSoundBuffer)
		{
			return false;
		}

		//	m_BufferArray.SetAtGrow((int)uIndex,lpSoundBuffer);
		if (m_BufferArray.size() <= uIndex)
		{
			m_BufferArray.resize(uIndex + 1);
		}
		m_BufferArray.at(uIndex) = lpSoundBuffer;

		return true;
	}

	/////////////////////////////////////////////
	///// ReleaseBuffer()
	//// 
	////  機能       : 指定した音のバッファを開放する
	//// 
	//////////////////////////////////////////////
	//void ReleaseBuffer(
	//	LPDIRECTSOUNDBUFFER lpSoundBuffer) // 解放するバッファのポインタ
	//{
	//	//lpSoundBuffer->Stop();
	//	//lpSoundBuffer->Release();
	//}

	///////////////////////////////////////////
	/// ReleaseBuffer()
	// 
	//  機能       : 指定した音を解放する
	// 
	////////////////////////////////////////////
	void ReleaseBuffer(
		UINT uIndex)	// 解放する音のインデックス
	{
//		LPDIRECTSOUNDBUFFER lpSoundBuffer;
		ComPtr<IDirectSoundBuffer> lpSoundBuffer;

		if (!(m_BufferArray.size() > (int)uIndex))
			return;

		m_BufferArray.at(uIndex).Reset();

//		lpSoundBuffer = m_BufferArray.at(uIndex);

//		if (lpSoundBuffer != NULL)
//			ReleaseBuffer(lpSoundBuffer);

		//	m_BufferArray.SetAt(uIndex,NULL);
//		m_BufferArray.at(uIndex) = NULL;
	}

	///////////////////////////////////////////
	/// ReleaseBufferAll()
	// 
	//  機能       : すべての音を解放する
	// 
	////////////////////////////////////////////
	void ReleaseBufferAll()
	{
		//DWORD i;

		//for (i = 0; i < m_BufferArray.size(); i++)
		//{
		//	ReleaseBuffer(i);
		//}

		//	m_BufferArray.RemoveAll();
		m_BufferArray.clear();
	}

	///////////////////////////////////////////
	/// Play()
	// 
	//  機能       : 音を再生する
	// 
	//  戻り値     : TRUE 成功
	// 
	////////////////////////////////////////////
	bool Play(
		UINT uIndex,		// 再生する音のインデックス
		int iPosition,		// 再生する位置
		BOOL bLoop)		// TRUE ループ再生
	{
		if (!m_lpSound)
			return FALSE;


		//LPDIRECTSOUNDBUFFER lpSoundBuffer;
		HRESULT hResult;
		DWORD	dwLoop = 0;

		// 指定のインデックスはない
		if (!(m_BufferArray.size() > (int)uIndex))
			return FALSE;

		auto lpSoundBuffer = m_BufferArray.at(uIndex);

		if (lpSoundBuffer == NULL)
			return FALSE;

		if (iPosition >= 0)
			lpSoundBuffer->SetCurrentPosition(iPosition);

		if (bLoop)
			dwLoop = DSBPLAY_LOOPING;

		hResult = lpSoundBuffer->Play(0, 0, dwLoop);

		if (hResult != DS_OK)
			return FALSE;

		return TRUE;
	}

	///////////////////////////////////////////
	/// Replay()
	// 
	//  機能       : Stop() で止めた位置から再生する
	// 
	//  戻り値     : TRUE 成功
	// 
	////////////////////////////////////////////
	bool Replay(
		UINT uIndex)	// 再生する音のインデックス
	{
		return Play(uIndex, -1, false);
	}

	///////////////////////////////////////////
	/// PauseAll()
	// 
	//  機能       : 再生中のバッファをポーズする
	// 
	//  戻り値     : ポーズしたバッファの数
	// 
	////////////////////////////////////////////
	int PauseAll()
	{

		//LPDIRECTSOUNDBUFFER lpSoundBuffer;
		ComPtr<IDirectSoundBuffer> lpSoundBuffer;
		HRESULT hResult;
		DWORD dwStatus;
		DWORD i;
		int iPlay;

		if (IsPause())
			return GetPauseNum();


		for (i = 0; i < m_BufferArray.size(); i++)
		{
			lpSoundBuffer = m_BufferArray[i];

			iPlay = PAUSE_NONE;

			if (lpSoundBuffer)
			{
				hResult = lpSoundBuffer->GetStatus(&dwStatus);

				if (hResult == DS_OK)
				{
					if (dwStatus & DSBSTATUS_PLAYING)
					{
						lpSoundBuffer->Stop();
						if (dwStatus & DSBSTATUS_LOOPING)
							iPlay = PAUSE_LOOPING;		// ループで再生
						else
							iPlay = PAUSE_PLAY;		// 普通に再生
					}
				}

			}
			//		m_BufPauseArray.Add(iPlay);
			m_BufPauseArray.push_back(iPlay);
		}

		return GetPauseNum();
	}

	///////////////////////////////////////////
	/// IsPause()
	// 
	//  機能       : 現在ポーズ状態か調べる
	// 
	//  戻り値     : TRUE ポーズ状態
	// 
	//  備考       : 1つもバッファを止めていないときでも
	//               ポーズ状態を確認できる
	// 
	////////////////////////////////////////////
	bool IsPause()
	{
		if (m_BufPauseArray.size() == 0)
			return FALSE;

		return TRUE;
	}

	///////////////////////////////////////////
	/// GetPauseNum()
	// 
	//  機能       : PauseAll()でポーズ状態になったバッファの数を取得
	// 
	//  戻り値     : ポーズ状態のバッファの数
	// 
	////////////////////////////////////////////
	int GetPauseNum()
	{
		DWORD i;
		int count = 0;

		for (i = 0; i < m_BufPauseArray.size(); i++)
		{
			if (m_BufPauseArray[i])
				count++;
		}

		return count;
	}

	///////////////////////////////////////////
	/// Stop()
	// 
	//  機能       : 再生中のバッファを停止する
	// 
	//  戻り値     : TRUE 再生中のバッファを停止
	// 
	//  備考       : 停止状態のバッファを指定するとFALSEが戻ります
	// 
	////////////////////////////////////////////
	bool Stop(
		uint32_t uIndex)	// 停止する音のインデックス
	{
		if (!m_lpSound)
			return FALSE;


		//LPDIRECTSOUNDBUFFER lpSoundBuffer;
		HRESULT hResult;
		DWORD dwStatus;

		if (!(m_BufferArray.size() > (int)uIndex))
			return FALSE;


		// バッファ取得
		auto lpSoundBuffer = m_BufferArray.at((int)uIndex);

		if (!lpSoundBuffer)
			return FALSE;

		// 再生中か？
		hResult = lpSoundBuffer->GetStatus(&dwStatus);
		if (hResult != DS_OK)
			return FALSE;
		if (!(dwStatus & DSBSTATUS_PLAYING))
			return FALSE;

		// サウンド停止
		hResult = lpSoundBuffer->Stop();

		if (hResult != DS_OK)
			return FALSE;

		return TRUE;
	}

	///////////////////////////////////////////
	/// SolvePause()
	// 
	//  機能       : ポーズを解除
	// 
	//  戻り値     : ポーズを解除した音の数
	// 
	////////////////////////////////////////////
	int SolvePause()
	{
		//LPDIRECTSOUNDBUFFER lpSoundBuffer;

		if (!IsPause())
			return FALSE;

		DWORD i;
		int count = 0;

		//	m_lpPrimaryBuf->Play(0,0,DSBPLAY_LOOPING);

		for (i = 0; i < m_BufferArray.size(); i++)
		{
			if (!(m_BufPauseArray.size() > i))
				break;

			auto lpSoundBuffer = m_BufferArray.at(i);

			if (lpSoundBuffer != NULL)
			{
				switch (m_BufPauseArray[i])
				{
				case PAUSE_PLAY:		// はじめから再生
					lpSoundBuffer->Play(0, 0, 0);
					count++;
					break;
				case PAUSE_LOOPING:		// ループで再生
					lpSoundBuffer->Play(0, 0, DSBPLAY_LOOPING);
					count++;
					break;
				}
			}
		}

		m_BufPauseArray.clear();

		return count;
	}

	///////////////////////////////////////////
	/// Release()
	// 
	//  機能       : オブジェクトを解放
	// 
	////////////////////////////////////////////
	void Release()
	{
		ReleaseBufferAll();

		//RELEASE(m_lpPrimaryBuf);
		//RELEASE(m_lpSound);

		m_lpPrimaryBuf.Reset();
		m_lpSound.Reset();
	}

	void SetVolume(uint32_t index, int volume)
	{
		if (m_BufferArray.size() <= index)
			return;

		auto lpBuffer = m_BufferArray[index];

		if (lpBuffer == NULL)
			return;

		lpBuffer->SetVolume(volume);
	}

	void SetPan(uint32_t index, int pan)
	{
		if (m_BufferArray.size() <= index)
			return;

		auto lpBuffer = m_BufferArray[index];

		if (lpBuffer == NULL)
			return;

		lpBuffer->SetPan(pan);
	}

	LPORGWAVEINFO ConvertFormat(LPORGWAVEINFO lpSrc)
	{
		// ACMの操作
		LPORGWAVEINFO pInfo;
		HACMSTREAM hAcm;		// ACMのハンドル

		WAVEFORMATEX *pWaveFmt;		// 変換後のフォーマット
		DWORD dwOutSize;		// 変換後のサイズ


		pWaveFmt = new WAVEFORMATEX;

		ZeroMemory(pWaveFmt, sizeof(WAVEFORMATEX));
		pWaveFmt->wFormatTag = WAVE_FORMAT_PCM;	// 変換後はPCM

		// 変換できるか調べる
		acmFormatSuggest(NULL, lpSrc->lpFormat, pWaveFmt, sizeof(WAVEFORMATEX), ACM_FORMATSUGGESTF_WFORMATTAG);

		// 変換元のストリームを開く
		acmStreamOpen(&hAcm, NULL, lpSrc->lpFormat, pWaveFmt, NULL, 0, 0, ACM_STREAMOPENF_NONREALTIME);

		// 変換後のサイズを取得
		acmStreamSize(hAcm, lpSrc->dwBufferSize, &dwOutSize, ACM_STREAMSIZEF_SOURCE);

		// 変換後用のバッファを確保
		BYTE *lpBuf = new BYTE[dwOutSize];

		// 変換情報の設定
		ACMSTREAMHEADER head;
		ZeroMemory(&head, sizeof(head));
		head.cbStruct = sizeof(ACMSTREAMHEADER);
		head.pbSrc = lpSrc->lpBuffer;
		head.cbSrcLength = lpSrc->dwBufferSize;
		head.pbDst = lpBuf;
		head.cbDstLength = dwOutSize;

		// 変換情報をストリームに渡す
		acmStreamPrepareHeader(hAcm, &head, 0);

		// 実際に変換を行う
		acmStreamConvert(hAcm, &head, 0);

		pInfo = new ORGWAVEINFO;
		pInfo->lpFormat = pWaveFmt;
		pInfo->lpBuffer = lpBuf;
		pInfo->dwBufferSize = head.cbDstLengthUsed;

		// 変換で使用したワークの破棄
		acmStreamUnprepareHeader(hAcm, &head, 0);
		// ストリームを閉じる
		acmStreamClose(hAcm, 0);

		return pInfo;
	}

	void ReleaseConvertFormat(LPORGWAVEINFO lpInfo)
	{
		if (lpInfo)
		{
			delete lpInfo->lpBuffer;
			delete lpInfo->lpFormat;
			delete lpInfo;
		}
	}

};


#define RELEASE(X) if(X){(X)->Release();X=NULL;}

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

// ポーズ状態
enum nPAUSE
{
	PAUSE_NONE,		// どの状態でもない
	PAUSE_PLAY,		// 再生中
	PAUSE_LOOPING,	// ループで再生中
};



///////////////////////////////////////////
/// OrgSound::OrgSound()
// 
//  機能       : コンストラクタ
// 
////////////////////////////////////////////
OrgSound::OrgSound() : pImpl(new Impl())
{
}

///////////////////////////////////////////
/// OrgSound()
// 
//  機能       : デストラクタ
// 
////////////////////////////////////////////
OrgSound::~OrgSound()
{
}

///////////////////////////////////////////
/// OrgSound::CreateBuffer()
// 
//  機能       : データテーブル解析し、WAVEデータならば
//               新しくバッファを作る
// 
//  戻り値     : バッファのポインタ
// 
//  引数       : 
// 
//  呼び出し元 : CreateBuffer
// 
//  備考       : 
// 
//  作成者     : 
// 
////////////////////////////////////////////
//LPDIRECTSOUNDBUFFER OrgSound::CreateBuffer(LPVOID lpBuf)	// Wave形式のデータ
//{
//	pImpl->CreateBuffer(lpBuf);
//}

///////////////////////////////////////////
/// OrgSound::CreateBuffer()
// 
//  機能       : バッファを作成し、指定されたデータを書き込む
// 
//  戻り値     : バッファのポインタ
// 
//  引数       : 
// 
//  呼び出し元 : CreateBuffer
// 
//  備考       : 
// 
//  作成者     : 
// 
////////////////////////////////////////////
//LPDIRECTSOUNDBUFFER OrgSound::CreateBuffer(
//											LPWAVEFORMATEX lpWaveFormat,	// Waveデータのフォーマット
//											LPBYTE lpWaveData,				// Waveデータ
//											uint32_t dwWaveSize)				// Waveデータのサイズ
//{
//	return pImpl->CreateBuffer(lpWaveData, lpWaveData, dwWaveSize);
//}

///////////////////////////////////////////
/// OrgSound::Create()
// 
//  機能       : オブジェクトの構築
// 
//  戻り値     : TRUE 成功
// 
//  引数       : 
// 
//  呼び出し元 : 
// 
//  備考       : DirectSoundのインターフェイスを協調モードで作成します。
//               
// 
//  作成者     : 
// 
////////////////////////////////////////////
bool OrgSound::Create(HWND hWnd)
{
	return pImpl->Create(hWnd);

}

///////////////////////////////////////////
/// OrgSound::CreateBuffer()
// 
//  機能       : 音を指定のインデックスに登録
// 
//  戻り値     : TRUE 成功
// 
////////////////////////////////////////////
bool OrgSound::CreateBuffer(
	void* lpBuf,	// サウンドのバッファ
	uint32_t uIndex)	// 登録先のインデックス
{
	return pImpl->CreateBuffer(lpBuf, uIndex);
}


///////////////////////////////////////////
/// OrgSound::ReleaseBuffer()
// 
//  機能       : 指定した音を解放する
// 
////////////////////////////////////////////
void OrgSound::ReleaseBuffer(
	uint32_t uIndex)	// 解放する音のインデックス
{
	pImpl->ReleaseBuffer(uIndex);
}

///////////////////////////////////////////
/// OrgSound::ReleaseBufferAll()
// 
//  機能       : すべての音を解放する
// 
////////////////////////////////////////////
void OrgSound::ReleaseBufferAll()
{
	pImpl->ReleaseBufferAll();
}

///////////////////////////////////////////
/// OrgSound::Play()
// 
//  機能       : 音を再生する
// 
//  戻り値     : TRUE 成功
// 
bool OrgSound::Play(
					 uint32_t uIndex,		// 再生する音のインデックス
					 int iPosition,		// 再生する位置
					 bool bLoop)		// TRUE ループ再生
{
	return pImpl->Play(uIndex, iPosition, bLoop);
}

///////////////////////////////////////////
/// OrgSound::Replay()
// 
//  機能       : Stop() で止めた位置から再生する
// 
//  戻り値     : TRUE 成功
// 
////////////////////////////////////////////
bool OrgSound::Replay(
	uint32_t uIndex)	// 再生する音のインデックス
{
	return pImpl->Play(uIndex,-1,false);
}

///////////////////////////////////////////
/// OrgSound::PauseAll()
// 
//  機能       : 再生中のバッファをポーズする
// 
//  戻り値     : ポーズしたバッファの数
// 
////////////////////////////////////////////
int OrgSound::PauseAll()
{

	return pImpl->PauseAll();
}

///////////////////////////////////////////
/// OrgSound::IsPause()
// 
//  機能       : 現在ポーズ状態か調べる
// 
//  戻り値     : TRUE ポーズ状態
// 
//  備考       : 1つもバッファを止めていないときでも
//               ポーズ状態を確認できる
// 
////////////////////////////////////////////
bool OrgSound::IsPause()
{
	return pImpl->IsPause();
}

///////////////////////////////////////////
/// OrgSound::GetPauseNum()
// 
//  機能       : PauseAll()でポーズ状態になったバッファの数を取得
// 
//  戻り値     : ポーズ状態のバッファの数
// 
////////////////////////////////////////////
int OrgSound::GetPauseNum()
{
	return pImpl->GetPauseNum();
}

///////////////////////////////////////////
/// OrgSound::Stop()
// 
//  機能       : 再生中のバッファを停止する
// 
//  戻り値     : TRUE 再生中のバッファを停止
// 
//  備考       : 停止状態のバッファを指定するとFALSEが戻ります
// 
////////////////////////////////////////////
bool OrgSound::Stop(
					 uint32_t uIndex)	// 停止する音のインデックス
{
	return pImpl->Stop(uIndex);
}

///////////////////////////////////////////
/// OrgSound::SolvePause()
// 
//  機能       : ポーズを解除
// 
//  戻り値     : ポーズを解除した音の数
// 
////////////////////////////////////////////
int OrgSound::SolvePause()
{
	return pImpl->SolvePause();
}

///////////////////////////////////////////
/// OrgSound::Release()
// 
//  機能       : オブジェクトを解放
// 
////////////////////////////////////////////
void OrgSound::Release()
{
	pImpl->Release();
}

void OrgSound::SetVolume(uint32_t index,int volume)
{
	pImpl->SetVolume(index, volume);
}

void OrgSound::SetPan(uint32_t index,int pan)
{
	pImpl->SetPan(index, pan);
}


