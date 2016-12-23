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
//	LPDIRECTSOUND			m_lpSound;		// �C���^�[�t�F�[�X
//	LPDIRECTSOUNDBUFFER		m_lpPrimaryBuf;	// �v���C�}���o�b�t�@

	ComPtr<IDirectSound>		m_lpSound;
	ComPtr<IDirectSoundBuffer>  m_lpPrimaryBuf;

	//	CTypedPtrArray<CPtrArray,LPDIRECTSOUNDBUFFER> m_BufferArray;	// �o�b�t�@�e�[�u��
	//	CArray<int,int&> m_BufPauseArray;		// �|�[�Y���

//	typedef vector<LPDIRECTSOUNDBUFFER> SOUNDBUFFERARRAY;
	typedef vector<ComPtr<IDirectSoundBuffer>> SOUNDBUFFERARRAY;
	SOUNDBUFFERARRAY m_BufferArray;	// �o�b�t�@�e�[�u��
	typedef vector<int> PAUSEARRAY;
	PAUSEARRAY m_BufPauseArray;		// �|�[�Y���


	typedef struct tORGWAVEINFO
	{
		LPWAVEFORMATEX	lpFormat;
		uint8_t*			lpBuffer;
		uint32_t			dwBufferSize;

	} ORGWAVEINFO, *LPORGWAVEINFO;

	// �|�[�Y���
	enum nPAUSE
	{
		PAUSE_NONE,		// �ǂ̏�Ԃł��Ȃ�
		PAUSE_PLAY,		// �Đ���
		PAUSE_LOOPING,	// ���[�v�ōĐ���
	};



	///////////////////////////////////////////
	/// OrgSound::OrgSound()
	// 
	//  �@�\       : �R���X�g���N�^
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
	//  �@�\       : �f�X�g���N�^
	// 
	////////////////////////////////////////////
	~Impl()
	{
		Release();
	}

	///////////////////////////////////////////
	/// OrgSound::CreateBuffer()
	// 
	//  �@�\       : �f�[�^�e�[�u����͂��AWAVE�f�[�^�Ȃ��
	//               �V�����o�b�t�@�����
	// 
	//  �߂�l     : �o�b�t�@�̃|�C���^
	// 
	//  ����       : 
	// 
	//  �Ăяo���� : CreateBuffer
	// 
	//  ���l       : 
	// 
	//  �쐬��     : 
	// 
	////////////////////////////////////////////
	LPDIRECTSOUNDBUFFER CreateBuffer(LPVOID lpBuf)	// Wave�`���̃f�[�^
	{
		LPDWORD pdwBuf = (DWORD*)lpBuf;
		LPDWORD pdwEnd;
		DWORD dwRiff;		//
		DWORD dwLength;		//
		DWORD dwType;		//

		// �t�@�C�����m�F����
		dwRiff = *pdwBuf++;
		dwLength = *pdwBuf++;
		dwType = *pdwBuf++;

		if (dwRiff != mmioFOURCC('R', 'I', 'F', 'F'))
			return NULL;

		if (dwType != mmioFOURCC('W', 'A', 'V', 'E'))
			return NULL;

		pdwEnd = (LPDWORD)((LPBYTE)pdwBuf + dwLength - 4);

		// �t�@�C������K�v�ȏ��𓾂�
		LPWAVEFORMATEX pWaveHeader = NULL;	// �v�`�u�d�w�b�_
		LPBYTE  pbWaveData = NULL;			// �v�`�u�d�f�[�^
		DWORD cbWaveSize = 0;				// �f�[�^�T�C�Y

		while (pdwBuf < pdwEnd)
		{
			dwType = *pdwBuf++;
			dwLength = *pdwBuf++;

			switch (dwType)
			{
			case mmioFOURCC('f', 'm', 't', ' '):
				// �w�b�_�擾
				if (pWaveHeader == NULL)
				{
					if (dwLength < sizeof(WAVEFORMAT))
						return FALSE;
					pWaveHeader = (LPWAVEFORMATEX)pdwBuf;
				}
				break;
			case mmioFOURCC('d', 'a', 't', 'a'):
				// �f�[�^�擾
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
			return NULL;		// �T�E���h�f�[�^�ł͂Ȃ�����

		if (pWaveHeader->wFormatTag != WAVE_FORMAT_PCM)
		{
			// PCM�ȊO�̃t�H�[�}�b�g�������Ƃ��̏���
			// PCM�ɕϊ����Ă���T�E���h�o�b�t�@���쐬����

//			LPDIRECTSOUNDBUFFER pBuffer;	// �T�E���h�o�b�t�@
			ComPtr<IDirectSoundBuffer> pBuffer;	// �T�E���h�o�b�t�@
			LPORGWAVEINFO pInfo;			// �ϊ���̃f�[�^���󂯎��
			ORGWAVEINFO src;				// �ϊ����̏���n�����߂̕ϐ�

			// �ϊ����̏��
			src.dwBufferSize = cbWaveSize;
			src.lpBuffer = pbWaveData;
			src.lpFormat = pWaveHeader;

			// PCM�ɕϊ�
			pInfo = ConvertFormat(&src);

			// �ϊ���̃f�[�^�ŃT�E���h�o�b�t�@���쐬
			pBuffer = CreateBuffer(pInfo->lpFormat, pInfo->lpBuffer, pInfo->dwBufferSize);

			// �ϊ��Ŏg�����f�[�^��j��
			ReleaseConvertFormat(pInfo);

			// �T�E���h�o�b�t�@��Ԃ�(NULL�̏ꍇ�̂���)
			return pBuffer.Detach();
		}

		// PCM�f�[�^�ŃT�E���h�o�b�t�@���쐬
		return CreateBuffer(pWaveHeader, pbWaveData, cbWaveSize);
	}

	///////////////////////////////////////////
	/// CreateBuffer()
	// 
	//  �@�\       : �o�b�t�@���쐬���A�w�肳�ꂽ�f�[�^����������
	// 
	//  �߂�l     : �o�b�t�@�̃|�C���^
	// 
	//  ����       : 
	// 
	//  �Ăяo���� : CreateBuffer
	// 
	//  ���l       : 
	// 
	//  �쐬��     : 
	// 
	////////////////////////////////////////////
	LPDIRECTSOUNDBUFFER CreateBuffer(
		LPWAVEFORMATEX lpWaveFormat,	// Wave�f�[�^�̃t�H�[�}�b�g
		LPBYTE lpWaveData,				// Wave�f�[�^
		DWORD dwWaveSize)				// Wave�f�[�^�̃T�C�Y
	{
		if (!m_lpSound)
			return nullptr;

		HRESULT hResult;
		DSBUFFERDESC dsbdesc;
//		LPDIRECTSOUNDBUFFER	lpDirectSoundBuffer;
		ComPtr<IDirectSoundBuffer> lpDirectSoundBuffer;

		//	if( !m_lpSound )
		//		return NULL ;

		// �\���̏�����
		ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
		dsbdesc.dwSize = sizeof(DSBUFFERDESC);
		dsbdesc.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_GETCURRENTPOSITION2;
		dsbdesc.dwBufferBytes = dwWaveSize;
		dsbdesc.lpwfxFormat = lpWaveFormat;

		// �o�b�t�@�쐬
		hResult = m_lpSound->CreateSoundBuffer(&dsbdesc, lpDirectSoundBuffer.GetAddressOf(), nullptr);
		if (hResult != DS_OK)
			return nullptr;

		// �o�b�t�@�����b�N
		LPVOID pMem1, pMem2;
		DWORD	dwSize1, dwSize2;
		hResult = lpDirectSoundBuffer->Lock(0, dwWaveSize, &pMem1, &dwSize1, &pMem2, &dwSize2, 0);
		if (hResult != DS_OK)
		{
			//lpDirectSoundBuffer->Release();
			return nullptr;
		}

		// �o�b�t�@���R�s�[
		CopyMemory(pMem1, lpWaveData, dwSize1);
		if (0 != dwSize2)
			CopyMemory(pMem2, lpWaveData + dwSize1, dwSize2);

		// ���b�N������
		lpDirectSoundBuffer->Unlock(pMem1, dwSize1, pMem2, dwSize2);

		return lpDirectSoundBuffer.Detach();
	}

	///////////////////////////////////////////
	/// Create()
	// 
	//  �@�\       : �I�u�W�F�N�g�̍\�z
	// 
	//  �߂�l     : TRUE ����
	// 
	//  ����       : 
	// 
	//  �Ăяo���� : 
	// 
	//  ���l       : DirectSound�̃C���^�[�t�F�C�X���������[�h�ō쐬���܂��B
	//               
	// 
	//  �쐬��     : 
	// 
	////////////////////////////////////////////
	bool Create(HWND hWnd)
	{
		//LPDIRECTSOUND lpSound;
		//LPDIRECTSOUNDBUFFER lpBuffer;
		ComPtr<IDirectSound> lpSound;
		ComPtr<IDirectSoundBuffer> lpBuffer;
		HRESULT hResult;

		// ���łɍ쐬�ς݂ł���
		if ((m_lpSound != NULL) || (m_lpPrimaryBuf != NULL))
			return false;

		// �C���^�[�t�F�C�X���쐬
		hResult = DirectSoundCreate(NULL, lpSound.GetAddressOf(), NULL);
		if (hResult != DS_OK)
		{
			// �C���^�[�t�F�[�X�쐬�Ɏ��s
			return false;
		}

		// �������x����ݒ�
		hResult = lpSound->SetCooperativeLevel(hWnd, DSSCL_NORMAL);
		if (hResult != DS_OK)
		{
			// �������x���̐ݒ�Ɏ��s
			return false;
		}


		DSBUFFERDESC dsbdesc;

		ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));

		dsbdesc.dwSize = sizeof(DSBUFFERDESC);
		dsbdesc.dwFlags = /*DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY |*/ DSBCAPS_PRIMARYBUFFER;
		dsbdesc.dwBufferBytes = 0;
		dsbdesc.lpwfxFormat = NULL;

		// �o�b�t�@�쐬
		hResult = lpSound->CreateSoundBuffer(&dsbdesc, lpBuffer.GetAddressOf(), NULL);
		if (FAILED(hResult))
		{
			return false;
		}


		// �V�����C���^�[�t�F�C�X�ƃo�b�t�@�������o�[�ɐݒ肷��
		m_lpSound = lpSound;
		m_lpPrimaryBuf = lpBuffer;

		// ����I��
		return true;

	}

	///////////////////////////////////////////
	/// CreateBuffer()
	// 
	//  �@�\       : �����w��̃C���f�b�N�X�ɓo�^
	// 
	//  �߂�l     : TRUE ����
	// 
	////////////////////////////////////////////
	bool CreateBuffer(
		LPVOID lpBuf,	// �T�E���h�̃o�b�t�@
		UINT uIndex)	// �o�^��̃C���f�b�N�X
	{

		ComPtr<IDirectSoundBuffer> lpSoundBuffer;

		// �o�b�t�@�̊J��
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
	////  �@�\       : �w�肵�����̃o�b�t�@���J������
	//// 
	//////////////////////////////////////////////
	//void ReleaseBuffer(
	//	LPDIRECTSOUNDBUFFER lpSoundBuffer) // �������o�b�t�@�̃|�C���^
	//{
	//	//lpSoundBuffer->Stop();
	//	//lpSoundBuffer->Release();
	//}

	///////////////////////////////////////////
	/// ReleaseBuffer()
	// 
	//  �@�\       : �w�肵�������������
	// 
	////////////////////////////////////////////
	void ReleaseBuffer(
		UINT uIndex)	// ������鉹�̃C���f�b�N�X
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
	//  �@�\       : ���ׂẲ����������
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
	//  �@�\       : �����Đ�����
	// 
	//  �߂�l     : TRUE ����
	// 
	////////////////////////////////////////////
	bool Play(
		UINT uIndex,		// �Đ����鉹�̃C���f�b�N�X
		int iPosition,		// �Đ�����ʒu
		BOOL bLoop)		// TRUE ���[�v�Đ�
	{
		if (!m_lpSound)
			return FALSE;


		//LPDIRECTSOUNDBUFFER lpSoundBuffer;
		HRESULT hResult;
		DWORD	dwLoop = 0;

		// �w��̃C���f�b�N�X�͂Ȃ�
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
	//  �@�\       : Stop() �Ŏ~�߂��ʒu����Đ�����
	// 
	//  �߂�l     : TRUE ����
	// 
	////////////////////////////////////////////
	bool Replay(
		UINT uIndex)	// �Đ����鉹�̃C���f�b�N�X
	{
		return Play(uIndex, -1, false);
	}

	///////////////////////////////////////////
	/// PauseAll()
	// 
	//  �@�\       : �Đ����̃o�b�t�@���|�[�Y����
	// 
	//  �߂�l     : �|�[�Y�����o�b�t�@�̐�
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
							iPlay = PAUSE_LOOPING;		// ���[�v�ōĐ�
						else
							iPlay = PAUSE_PLAY;		// ���ʂɍĐ�
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
	//  �@�\       : ���݃|�[�Y��Ԃ����ׂ�
	// 
	//  �߂�l     : TRUE �|�[�Y���
	// 
	//  ���l       : 1���o�b�t�@���~�߂Ă��Ȃ��Ƃ��ł�
	//               �|�[�Y��Ԃ��m�F�ł���
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
	//  �@�\       : PauseAll()�Ń|�[�Y��ԂɂȂ����o�b�t�@�̐����擾
	// 
	//  �߂�l     : �|�[�Y��Ԃ̃o�b�t�@�̐�
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
	//  �@�\       : �Đ����̃o�b�t�@���~����
	// 
	//  �߂�l     : TRUE �Đ����̃o�b�t�@���~
	// 
	//  ���l       : ��~��Ԃ̃o�b�t�@���w�肷���FALSE���߂�܂�
	// 
	////////////////////////////////////////////
	bool Stop(
		uint32_t uIndex)	// ��~���鉹�̃C���f�b�N�X
	{
		if (!m_lpSound)
			return FALSE;


		//LPDIRECTSOUNDBUFFER lpSoundBuffer;
		HRESULT hResult;
		DWORD dwStatus;

		if (!(m_BufferArray.size() > (int)uIndex))
			return FALSE;


		// �o�b�t�@�擾
		auto lpSoundBuffer = m_BufferArray.at((int)uIndex);

		if (!lpSoundBuffer)
			return FALSE;

		// �Đ������H
		hResult = lpSoundBuffer->GetStatus(&dwStatus);
		if (hResult != DS_OK)
			return FALSE;
		if (!(dwStatus & DSBSTATUS_PLAYING))
			return FALSE;

		// �T�E���h��~
		hResult = lpSoundBuffer->Stop();

		if (hResult != DS_OK)
			return FALSE;

		return TRUE;
	}

	///////////////////////////////////////////
	/// SolvePause()
	// 
	//  �@�\       : �|�[�Y������
	// 
	//  �߂�l     : �|�[�Y�������������̐�
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
				case PAUSE_PLAY:		// �͂��߂���Đ�
					lpSoundBuffer->Play(0, 0, 0);
					count++;
					break;
				case PAUSE_LOOPING:		// ���[�v�ōĐ�
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
	//  �@�\       : �I�u�W�F�N�g�����
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
		// ACM�̑���
		LPORGWAVEINFO pInfo;
		HACMSTREAM hAcm;		// ACM�̃n���h��

		WAVEFORMATEX *pWaveFmt;		// �ϊ���̃t�H�[�}�b�g
		DWORD dwOutSize;		// �ϊ���̃T�C�Y


		pWaveFmt = new WAVEFORMATEX;

		ZeroMemory(pWaveFmt, sizeof(WAVEFORMATEX));
		pWaveFmt->wFormatTag = WAVE_FORMAT_PCM;	// �ϊ����PCM

		// �ϊ��ł��邩���ׂ�
		acmFormatSuggest(NULL, lpSrc->lpFormat, pWaveFmt, sizeof(WAVEFORMATEX), ACM_FORMATSUGGESTF_WFORMATTAG);

		// �ϊ����̃X�g���[�����J��
		acmStreamOpen(&hAcm, NULL, lpSrc->lpFormat, pWaveFmt, NULL, 0, 0, ACM_STREAMOPENF_NONREALTIME);

		// �ϊ���̃T�C�Y���擾
		acmStreamSize(hAcm, lpSrc->dwBufferSize, &dwOutSize, ACM_STREAMSIZEF_SOURCE);

		// �ϊ���p�̃o�b�t�@���m��
		BYTE *lpBuf = new BYTE[dwOutSize];

		// �ϊ����̐ݒ�
		ACMSTREAMHEADER head;
		ZeroMemory(&head, sizeof(head));
		head.cbStruct = sizeof(ACMSTREAMHEADER);
		head.pbSrc = lpSrc->lpBuffer;
		head.cbSrcLength = lpSrc->dwBufferSize;
		head.pbDst = lpBuf;
		head.cbDstLength = dwOutSize;

		// �ϊ������X�g���[���ɓn��
		acmStreamPrepareHeader(hAcm, &head, 0);

		// ���ۂɕϊ����s��
		acmStreamConvert(hAcm, &head, 0);

		pInfo = new ORGWAVEINFO;
		pInfo->lpFormat = pWaveFmt;
		pInfo->lpBuffer = lpBuf;
		pInfo->dwBufferSize = head.cbDstLengthUsed;

		// �ϊ��Ŏg�p�������[�N�̔j��
		acmStreamUnprepareHeader(hAcm, &head, 0);
		// �X�g���[�������
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
// �\�z/����
//////////////////////////////////////////////////////////////////////

// �|�[�Y���
enum nPAUSE
{
	PAUSE_NONE,		// �ǂ̏�Ԃł��Ȃ�
	PAUSE_PLAY,		// �Đ���
	PAUSE_LOOPING,	// ���[�v�ōĐ���
};



///////////////////////////////////////////
/// OrgSound::OrgSound()
// 
//  �@�\       : �R���X�g���N�^
// 
////////////////////////////////////////////
OrgSound::OrgSound() : pImpl(new Impl())
{
}

///////////////////////////////////////////
/// OrgSound()
// 
//  �@�\       : �f�X�g���N�^
// 
////////////////////////////////////////////
OrgSound::~OrgSound()
{
}

///////////////////////////////////////////
/// OrgSound::CreateBuffer()
// 
//  �@�\       : �f�[�^�e�[�u����͂��AWAVE�f�[�^�Ȃ��
//               �V�����o�b�t�@�����
// 
//  �߂�l     : �o�b�t�@�̃|�C���^
// 
//  ����       : 
// 
//  �Ăяo���� : CreateBuffer
// 
//  ���l       : 
// 
//  �쐬��     : 
// 
////////////////////////////////////////////
//LPDIRECTSOUNDBUFFER OrgSound::CreateBuffer(LPVOID lpBuf)	// Wave�`���̃f�[�^
//{
//	pImpl->CreateBuffer(lpBuf);
//}

///////////////////////////////////////////
/// OrgSound::CreateBuffer()
// 
//  �@�\       : �o�b�t�@���쐬���A�w�肳�ꂽ�f�[�^����������
// 
//  �߂�l     : �o�b�t�@�̃|�C���^
// 
//  ����       : 
// 
//  �Ăяo���� : CreateBuffer
// 
//  ���l       : 
// 
//  �쐬��     : 
// 
////////////////////////////////////////////
//LPDIRECTSOUNDBUFFER OrgSound::CreateBuffer(
//											LPWAVEFORMATEX lpWaveFormat,	// Wave�f�[�^�̃t�H�[�}�b�g
//											LPBYTE lpWaveData,				// Wave�f�[�^
//											uint32_t dwWaveSize)				// Wave�f�[�^�̃T�C�Y
//{
//	return pImpl->CreateBuffer(lpWaveData, lpWaveData, dwWaveSize);
//}

///////////////////////////////////////////
/// OrgSound::Create()
// 
//  �@�\       : �I�u�W�F�N�g�̍\�z
// 
//  �߂�l     : TRUE ����
// 
//  ����       : 
// 
//  �Ăяo���� : 
// 
//  ���l       : DirectSound�̃C���^�[�t�F�C�X���������[�h�ō쐬���܂��B
//               
// 
//  �쐬��     : 
// 
////////////////////////////////////////////
bool OrgSound::Create(HWND hWnd)
{
	return pImpl->Create(hWnd);

}

///////////////////////////////////////////
/// OrgSound::CreateBuffer()
// 
//  �@�\       : �����w��̃C���f�b�N�X�ɓo�^
// 
//  �߂�l     : TRUE ����
// 
////////////////////////////////////////////
bool OrgSound::CreateBuffer(
	void* lpBuf,	// �T�E���h�̃o�b�t�@
	uint32_t uIndex)	// �o�^��̃C���f�b�N�X
{
	return pImpl->CreateBuffer(lpBuf, uIndex);
}


///////////////////////////////////////////
/// OrgSound::ReleaseBuffer()
// 
//  �@�\       : �w�肵�������������
// 
////////////////////////////////////////////
void OrgSound::ReleaseBuffer(
	uint32_t uIndex)	// ������鉹�̃C���f�b�N�X
{
	pImpl->ReleaseBuffer(uIndex);
}

///////////////////////////////////////////
/// OrgSound::ReleaseBufferAll()
// 
//  �@�\       : ���ׂẲ����������
// 
////////////////////////////////////////////
void OrgSound::ReleaseBufferAll()
{
	pImpl->ReleaseBufferAll();
}

///////////////////////////////////////////
/// OrgSound::Play()
// 
//  �@�\       : �����Đ�����
// 
//  �߂�l     : TRUE ����
// 
bool OrgSound::Play(
					 uint32_t uIndex,		// �Đ����鉹�̃C���f�b�N�X
					 int iPosition,		// �Đ�����ʒu
					 bool bLoop)		// TRUE ���[�v�Đ�
{
	return pImpl->Play(uIndex, iPosition, bLoop);
}

///////////////////////////////////////////
/// OrgSound::Replay()
// 
//  �@�\       : Stop() �Ŏ~�߂��ʒu����Đ�����
// 
//  �߂�l     : TRUE ����
// 
////////////////////////////////////////////
bool OrgSound::Replay(
	uint32_t uIndex)	// �Đ����鉹�̃C���f�b�N�X
{
	return pImpl->Play(uIndex,-1,false);
}

///////////////////////////////////////////
/// OrgSound::PauseAll()
// 
//  �@�\       : �Đ����̃o�b�t�@���|�[�Y����
// 
//  �߂�l     : �|�[�Y�����o�b�t�@�̐�
// 
////////////////////////////////////////////
int OrgSound::PauseAll()
{

	return pImpl->PauseAll();
}

///////////////////////////////////////////
/// OrgSound::IsPause()
// 
//  �@�\       : ���݃|�[�Y��Ԃ����ׂ�
// 
//  �߂�l     : TRUE �|�[�Y���
// 
//  ���l       : 1���o�b�t�@���~�߂Ă��Ȃ��Ƃ��ł�
//               �|�[�Y��Ԃ��m�F�ł���
// 
////////////////////////////////////////////
bool OrgSound::IsPause()
{
	return pImpl->IsPause();
}

///////////////////////////////////////////
/// OrgSound::GetPauseNum()
// 
//  �@�\       : PauseAll()�Ń|�[�Y��ԂɂȂ����o�b�t�@�̐����擾
// 
//  �߂�l     : �|�[�Y��Ԃ̃o�b�t�@�̐�
// 
////////////////////////////////////////////
int OrgSound::GetPauseNum()
{
	return pImpl->GetPauseNum();
}

///////////////////////////////////////////
/// OrgSound::Stop()
// 
//  �@�\       : �Đ����̃o�b�t�@���~����
// 
//  �߂�l     : TRUE �Đ����̃o�b�t�@���~
// 
//  ���l       : ��~��Ԃ̃o�b�t�@���w�肷���FALSE���߂�܂�
// 
////////////////////////////////////////////
bool OrgSound::Stop(
					 uint32_t uIndex)	// ��~���鉹�̃C���f�b�N�X
{
	return pImpl->Stop(uIndex);
}

///////////////////////////////////////////
/// OrgSound::SolvePause()
// 
//  �@�\       : �|�[�Y������
// 
//  �߂�l     : �|�[�Y�������������̐�
// 
////////////////////////////////////////////
int OrgSound::SolvePause()
{
	return pImpl->SolvePause();
}

///////////////////////////////////////////
/// OrgSound::Release()
// 
//  �@�\       : �I�u�W�F�N�g�����
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


