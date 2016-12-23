#include "Stage.h"
#include "Scene/Scene.h"
#include "DirectX/Direct3D.h"
#include "Camera/Camera.h"

#include "Player/Player.h"
#include "Enemy/EnemyFactory/EnemyFactory.h"
#include "Item/Item.h"
#include "Camera/Camera.h"
#include "Utility/TaskManager.h"
#include "InputManager/InputManager.h"

#include "Constant.h"
#include "Function.h"
#include "Utility/GraphManager.h"
#include "Sound/SoundManager.h"
#include "Control/Control.h"
#include "Utility/Debug.h"

using namespace std;
using namespace Constant;
using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace
{
	const int Big = 0;
	const int Middle = 1;
	const int Small = 2;
}

Stage::Stage()
{
	count_ = 0;
	is_living_ = true;
	pos.x = 23.0f;
	pos.y = 12.0f;
	spd.x = 0.f;
	spd.y = 10.0f;
	mode_ = DRAW_ALPHA;
	priority_ = PRIORITY_BACKGROUND;
	enemyFactory = EnemyFactory::instance();

	// 敵の読み込み
	auto res = LoadEnemy(L"Resources/EnemyDate/enemy.map", &EnemyDate);
	// 読み込み失敗
	assert(res + "Enemy Loading Error");

	GraphManager *graphManager = GraphManager::Instance();
	graph_ = graphManager->FindIndexTexture(L"resultbord.png");

	background_ = graphManager->FindIndexModel(L"stage01.cmo");
	cloud_[0] = graphManager->FindIndexModel(L"bigCloud.cmo");
	cloud_[1] = graphManager->FindIndexModel(L"middleCloud.cmo");
	cloud_[2] = graphManager->FindIndexModel(L"smallCloud.cmo");
	/*　
	*　複製したいときはそのままコピーで使う
	*  modelTest = background_;
	*/

	//Model::CreateFromCMO(g_pd3dDevice.Get(), L"Resources/cModels/stage01.cmo", *Control::Instance()->GetFactory())

	assert(background_);

	{// 背景の空の初期化
		Matrix scalem;
		Matrix rotm;
		Matrix tFoward;
		Matrix tDepth;

		scalem = Matrix::CreateScale(1.f);
		// 引数は,y,x,zの順
		rotm = Matrix::CreateFromYawPitchRoll(0.f, 0.f, 0.f);
		tFoward = Matrix::CreateTranslation(Vector3(0.f, 0.f, 0.f));
		tDepth = Matrix::CreateTranslation(Vector3(0.f, 0.f, 157.f));

		localFoward_ = Matrix::Identity;
		localFoward_ *= scalem * rotm * tFoward;

		localDepth_ = localFoward_;
		localDepth_ *= tDepth;
	}
	camera = new Camera();

	{// 背景の雲の初期化
		Matrix scalem;
		Matrix rotm;
		Matrix cloudTrans_[3];

		scalem = Matrix::CreateScale(1.f);
		rotm = Matrix::CreateFromYawPitchRoll(0.f, 0.f, 0.f);
		cloudTrans_[Big] = Matrix::CreateTranslation(10, 4, 50);
		cloudTrans_[Middle] = Matrix::CreateTranslation(0, 4, 75);
		cloudTrans_[Small] = Matrix::CreateTranslation(20, 4, 100);

		cloudWorld_[Big] = scalem * rotm * cloudTrans_[Big];
		cloudWorld_[Middle] = scalem * rotm * cloudTrans_[Middle];
		cloudWorld_[Small] = scalem * rotm * cloudTrans_[Small];
	}

	for (int i = 0; i < 34; i++)
	{
		enemyFactory->SetEnemy(EnemyDate.id[i], 0, EnemyDate.pattern[i], EnemyDate.brknd[i],
			Vector2(static_cast<float>(EnemyDate.x[i]), static_cast<float>(EnemyDate.y[i])),
			Vector2(static_cast<float>(EnemyDate.spd_x[i]), static_cast<float>(EnemyDate.spd_y[i])),
			EnemyDate.bltime[i], EnemyDate.blknd[i], 0, EnemyDate.life[i], EnemyDate.wait[i], (GameObject::ItemID)EnemyDate.itemID[i]);
	}

	for (list<Enemy*>::iterator it = usedEnemy.begin(); it != usedEnemy.end(); ++it)
	{
		Enemy *task = *it;

		task->setIsLiving(false);
	}

	usedList = enemyFactory->enemyList();
}

Stage::~Stage()
{
	if (!usedEnemy.empty())
	{
		for (list<Enemy*>::iterator it = usedEnemy.end(); it != usedEnemy.begin(); ++it)
		{
			Enemy *task = *it;

			enemyFactory->enemyList().push_front(task);
		}
	}
	if (background_)
	{// ロードしたモデルの種類の数だけ以下の処理を適応

		if (background_->name.substr(0, 18) == ModelPath)
		{// パスの部分を消してファイルの名前を取り出す
			background_->name.erase(0, 18);
			for (int i = 0; i < CloudNum; i++)
			{
				cloud_[i]->name.erase(0, 18);
			}
		}

		GraphManager::Instance()->AddAndLoadModel(background_->name.c_str());
		
		for (int i = 0; i < CloudNum; i++)
		{
			GraphManager::Instance()->AddAndLoadModel(cloud_[i]->name.c_str());
		}
	}

	//if (!usedEnemy.empty())
	//{
	//	for (list<Enemy*>::iterator it = usedEnemy.begin(); it != usedEnemy.end(); ++it)
	//	{
	//		Enemy *task = *it;
	//		if (task)
	//		{
	//			delete task;
	//		}
	//	}
	//}
}
#pragma region private_function

void Stage::ScrollBack()
{
	const float StartDepth = -157.f;
	const Vector3 moveSpd = Vector3(0.f,0.f, -0.5f);

	if (localFoward_.Translation().z <= StartDepth)
	{// ループ処理

		XMVECTOR trans = XMVectorSet(0.f, 0.f,  -(localDepth_.Translation().z + StartDepth), 0.f);
		localFoward_ = Matrix::CreateTranslation(trans);
	}
	if (localDepth_.Translation().z <= StartDepth)
	{// ループ処理

		XMVECTOR trans = XMVectorSet(0.f, 0.f,  -(localFoward_.Translation().z + StartDepth), 0.f);
		localDepth_ = Matrix::CreateTranslation(trans);
	}
	for (int i = 0; i < CloudNum; i++)
	{
		if (cloudWorld_[i].Translation().z <= -50)
		{
			XMVECTOR cloudTrans = XMVectorSet(cloudWorld_[i].Translation().x, cloudWorld_[i].Translation().y, 50.f, 0.f);
			cloudWorld_[i] = Matrix::CreateTranslation(cloudTrans);
		}
	}
	if (is_player_unrivaled_)
	{
		const float ran = float((int)XorRand() % 21 - 10);
		XMVECTOR tFoward = XMVectorSet(ran, 0.f, localFoward_.Translation().z + moveSpd.z, 0.f);
		XMVECTOR tDepth = XMVectorSet(ran, 0.f, localDepth_.Translation().z + moveSpd.z, 0.f);

		localFoward_ = Matrix::CreateTranslation(tFoward);
		localDepth_ = Matrix::CreateTranslation(tDepth);

		const float rand = float((int)XorRand() % 210 - 100);

		cloudWorld_[Big] = Matrix::CreateTranslation(XMVectorSet(rand, cloudWorld_[Big].Translation().y,
			cloudWorld_[Big].Translation().z + moveSpd.z, 0));
		cloudWorld_[Middle] = Matrix::CreateTranslation(XMVectorSet(rand, cloudWorld_[Middle].Translation().y,
			cloudWorld_[Middle].Translation().z + moveSpd.z, 0));
		cloudWorld_[Small] = Matrix::CreateTranslation(XMVectorSet(rand, cloudWorld_[Small].Translation().y,
			cloudWorld_[Small].Translation().z + moveSpd.z, 0));
	}
	else
	{
		localFoward_ = Matrix::CreateTranslation(XMVectorSet(0, 0, localFoward_.Translation().z + moveSpd.z, 0));
		localDepth_ = Matrix::CreateTranslation(XMVectorSet(0, 0, localDepth_.Translation().z + moveSpd.z, 0));

		cloudWorld_[Big] = Matrix::CreateTranslation(XMVectorSet(10.f, 4.f, 
			cloudWorld_[Big].Translation().z + moveSpd.z, 0));
		cloudWorld_[Middle] = Matrix::CreateTranslation(XMVectorSet(0.f, 4.f,
			cloudWorld_[Middle].Translation().z + moveSpd.z, 0));
		cloudWorld_[Small] = Matrix::CreateTranslation(XMVectorSet(20.f, 4.f, 
			cloudWorld_[Small].Translation().z + moveSpd.z, 0));
	}
	
}

bool Stage::LoadEnemy(wchar_t* filename, ENEMYDATA* pENEMYDATA)
{
	// ファイルオープン（バイナリー形式）
	fstream fs(filename, ios::binary | ios::in);
	if (!fs)
	{
		return false;
	}

	fs.read((char*)pENEMYDATA, sizeof(ENEMYDATA));
	fs.close();

	return true;
}

void Stage::GenerationEnemy()
{
	const int MaxEnemy = 100;
	for (int i = 0; i < MaxEnemy; i++)
	{
		if (count_ == EnemyDate.count[i] &&
			count_ != 0 &&
			count_ != 2600)
		{
			auto it = usedList.begin();
			Enemy *task = *it;
			task->createEnemy();
			TaskManager::Instance()->Add(task);
			usedEnemy.push_back(task);
			usedList.erase(it);
		}
	}

}

void Stage::setLighting()
{
	Vector3 myLightTarget(camera->GetRefpos().y);
	Vector3 myLightPosition(camera->GetEyepos().y);
	Vector3 myLightDirection = myLightTarget - myLightPosition;
	myLightDirection.Normalize();

	background_->UpdateEffects([&](IEffect* effect)
	{
		if (auto e = dynamic_cast<DGSLEffect*>(effect))
		{
			e->SetLightDirection(0, myLightDirection);
		}
	});

	for (int i = 0; i < CloudNum; i++)
	{
		cloud_[i]->UpdateEffects([&](IEffect* effect)
		{
			if (auto e = dynamic_cast<DGSLEffect*>(effect))
			{
				e->SetLightDirection(0, myLightDirection);
			}
		});
	}
}

#pragma endregion

#pragma region public_function

void Stage::initialize()
{
}

bool Stage::update()
{
#if defined(_DEBUG)
	//if (InputManager::Instance()->IsTouched(KEY_DEBUG))
	//{
	//	count_ = 2580;
	//	debug->Log(count_);
	//}
#endif
	//setLighting();
	camera->UpCamera();
	camera->Update();

	list<GameObject*>gameObject = GameObject::objectList();
	for (list<GameObject*>::iterator it = gameObject.begin(); it != gameObject.end(); ++it)
	{
		GameObject *task = *it;
		if (task->getID() == OBJECT_PLAYER)
		{// 無敵かどうか
			is_player_unrivaled_ = task->GetIsUnrivaled();
			break;
		}
	}

	ScrollBack();
	GenerationEnemy();

	count_++;

	return true;
}

void Stage::draw()
{
	assert(camera);

	const Matrix& view = camera->GetViewmat();
	const Matrix& projection = camera->GetProjmat();
	assert(Direct3D::Instance()->context().Get());
	assert(GraphManager::Instance()->state());
	assert(background_);

	background_->Draw(Direct3D::Instance()->context().Get(), *GraphManager::Instance()->state(), localFoward_, view, projection, false, [&]()
	{// アルファブレンドを設定する
		Direct3D::Instance()->context()->OMSetBlendState(GraphManager::Instance()->state()->NonPremultiplied(), Colors::Black, 0xFFFFFFFF);
		//Direct3D::Instance()->context()->PSSetShader(GraphManager::Instance()->pixelShader(), nullptr, 0);
	});
	background_->Draw(Direct3D::Instance()->context().Get(), *GraphManager::Instance()->state(), localDepth_, view, projection, false, [&]
	{// アルファブレンドを設定する
		Direct3D::Instance()->context().Get()->OMSetBlendState(GraphManager::Instance()->state()->NonPremultiplied(), Colors::Black, 0xFFFFFFFF);
		//Direct3D::Instance()->context()->PSSetShader(GraphManager::Instance()->pixelShader(), nullptr, 0);
	});


	assert(cloud_);
	for (int i = 0; i < CloudNum; i++)
	{
		cloud_[i]->Draw(Direct3D::Instance()->context().Get(), *GraphManager::Instance()->state(), cloudWorld_[i], view, projection, false, [&]
		{
			// アルファブレンドを設定する
			Direct3D::Instance()->context()->OMSetBlendState(GraphManager::Instance()->state()->NonPremultiplied(), Colors::Black, 0xFFFFFFFF);

		});
	}
}

void Stage::finalize()
{
}

#pragma endregion