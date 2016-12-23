#include "Control.h"
#include "../DirectX/Direct3D.h"

#include "../Texture/Texture.h"
#include "../GameObject.h"

#include "../Utility/GraphManager.h"
#include "../Sound/SoundManager.h"
#include "../Utility/Debug.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace
{
	const int IMAGE_LOADING_STL = 0;
	const int IMAGE_LOADING_BACK = 1;
}

#pragma region private_function
#pragma endregion

#pragma region public_function

void Control::Start()
{	
	graph_[IMAGE_LOADING_STL] = new Texture(L"Resources/Images/nowLoading.png");
	graph_[IMAGE_LOADING_BACK] = new Texture(L"Resources/Images/loadingBack.png");

	model_ = Model::CreateFromCMO(
		Direct3D::Instance()->pd3dDevice().Get(),
		L"Resources/cModels/logo.cmo",
		*GraphManager::Instance()->factory()
		);

	try
	{
		thread t1(&Control::Initialize, this);
		thread t2(&Control::Draw, this);

		t1.join();
		t2.join();

	}
	catch (exception &ex)
	{
		debug->Log(ex.what());
		exit(EXIT_FAILURE);
	}

}

void Control::Initialize()
{
	GraphManager *graphManager = GraphManager::Instance();
	{// グラフィックマネージャーの初期化
		graphManager->AddAndLoadImage(L"player.png");
		graphManager->AddAndLoadImage(L"smallenemy.png");
		graphManager->AddAndLoadImage(L"item.png");
		graphManager->AddAndLoadImage(L"boss.png");
		graphManager->AddAndLoadImage(L"enemyCrashEffect.png");
		graphManager->AddAndLoadImage(L"playerCrashEffect.png");
		graphManager->AddAndLoadImage(L"magiceffect.png");
		graphManager->AddAndLoadImage(L"screweffect.png");
		graphManager->AddAndLoadImage(L"titleback.png");
		graphManager->AddAndLoadImage(L"titletext.png");
		graphManager->AddAndLoadImage(L"titlename.png");
		graphManager->AddAndLoadImage(L"textscreen.png");
		graphManager->AddAndLoadImage(L"resultbord.png");
		graphManager->AddAndLoadImage(L"pausemenu.png");
		graphManager->AddAndLoadImage(L"number.png");
		graphManager->AddAndLoadImage(L"back.png");
		graphManager->AddAndLoadImage(L"ui.png");
		graphManager->AddAndLoadImage(L"PlayerWeapon.png");
		graphManager->AddAndLoadImage(L"democall.png");
		graphManager->AddAndLoadImage(L"gauge.png");
		graphManager->AddAndLoadImage(L"resultMessege.png");
		graphManager->AddAndLoadImage(L"bossCrashEffect.png");
		graphManager->AddAndLoadImage(L"bullets.png");
		graphManager->AddAndLoadImage(L"vital.png");

		graphManager->AddAndLoadModel(L"stage01.cmo");
		graphManager->AddAndLoadModel(L"bigCloud.cmo");
		graphManager->AddAndLoadModel(L"middleCloud.cmo");
		graphManager->AddAndLoadModel(L"smallCloud.cmo");
	}

	SoundManager *soundManager = SoundManager::Instance();
	{// サウンドマネージャーの初期化
		soundManager->AddAndLoadSound("enemy_crash.wav");
		soundManager->AddAndLoadSound("player_shot.wav");
		soundManager->AddAndLoadSound("item_get.wav");
		soundManager->AddAndLoadSound("power_up.wav");
		soundManager->AddAndLoadSound("lastlife.wav");
		soundManager->AddAndLoadSound("damage.wav");
		soundManager->AddAndLoadSound("cursolmove.wav");
		soundManager->AddAndLoadSound("enter.wav");
		soundManager->AddAndLoadSound("cancel.wav");
		soundManager->AddAndLoadSound("bomb.wav");
		soundManager->AddAndLoadSound("bomb_use.wav");
		soundManager->AddAndLoadSound("chargeShot.wav");
		soundManager->AddAndLoadSound("stage01bgm.wav");
		soundManager->AddAndLoadSound("title.wav");
		soundManager->AddAndLoadSound("enemyDamage.wav");

		//soundManager->AddAndLoadSound("bossCrash.wav");

	}

	isLoading_ = false;
}

bool Control::Update()
{	
	if (score_ > hiScore_)
	{
		hiScore_ = score_;
	}
	GameObject::ObjectOnObject();

	return true;
}

void Control::Draw()
{
	isLoading_ = true;

	Matrix localWorld = Matrix::Identity;
	Matrix scalem = Matrix::CreateScale(0.25f);
	Matrix rotm = Matrix::CreateFromYawPitchRoll(0.f, 0.f, 0.005f);
	Matrix transm = Matrix::CreateTranslation(Vector3(0.f, 0.f, 0.f));

	localWorld = scalem * rotm * transm;

	static const Vector3 eyepos = Vector3(0.0f, 250.0f, -0.1f);
	static const Vector3 refpos = Vector3(0.0f, 0.0f, 0.0f);
	static const Vector3 upvec = Vector3(0.0f, 1.0f, 0.0f).TransformNormal(Vector3(0, 1, 0), Matrix::Identity);
	static const float fovY_(XMConvertToRadians(30.0f));
	static const float aspect_(640.0f / 480.0f);
	static const float nearClip_(0.1f);
	static const float farClip_(500.0f);

	while (isLoading_)
	{// ローディング中のアニメーション

		localWorld *= rotm;

		// バックバッファのクリア
		Direct3D::Instance()->context()->ClearRenderTargetView(Direct3D::Instance()->renderTargetView(), DirectX::Colors::MidnightBlue);
		// 深度バッファのクリア
		Direct3D::Instance()->context()->ClearDepthStencilView(Direct3D::Instance()->depthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		GraphManager::Instance()->spriteBatch()->Begin(SpriteSortMode_Deferred, GraphManager::Instance()->state()->NonPremultiplied());

		DrawGraph(0, 0, graph_[IMAGE_LOADING_BACK]);
		DrawGraph(0, 0, graph_[IMAGE_LOADING_STL]);

		GraphManager::Instance()->spriteBatch()->End();

		GraphManager::Instance()->spriteBatch()->Begin(SpriteSortMode_Deferred, GraphManager::Instance()->state()->NonPremultiplied());

		model_->Draw(Direct3D::Instance()->context().Get(), *GraphManager::Instance()->state(), localWorld,
			Matrix::CreateLookAt(eyepos, refpos, upvec), Matrix::CreatePerspectiveFieldOfView(fovY_, aspect_, nearClip_, farClip_), false, [&]()
		{// アルファブレンドを設定する
			Direct3D::Instance()->context()->OMSetBlendState(GraphManager::Instance()->state()->NonPremultiplied(), Colors::Black, 0xFFFFFFFF);
		});

		GraphManager::Instance()->spriteBatch()->End();
		Direct3D::Instance()->swapChain()->Present(0, 0);
	}
}

void Control::Finalize()
{// シーンの移動ごとに呼び出す
	isCeared_ = false;
	GameObject::ClearObjectList();
	score_ = 0;
	hiScore_ = 0;
}

#pragma endregion