//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   GameMain.h
//!
//! @brief  ゲーム関連のヘッダファイル
//!
//! @date   日付
//!
//! @author 制作者名
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
#pragma once


#include "SceneManager.h"


class Scene
{
protected:
	SceneManager* SceneManager_;

public:
	Scene(SceneManager* SceneManager);
	virtual ~Scene();

	virtual void initialize() = 0;
	virtual bool update() = 0;
	virtual void draw() = 0;
	virtual void finalize() = 0;
};


