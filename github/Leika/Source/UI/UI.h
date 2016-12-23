#pragma once

/**
* @file   UI.h
* @brief  UI ÇÃçXêVÅAï`âÊ
* @author Katsumi Takei
* @date   2016 / 06 / 29
*/

#include "../Texture/Texture.h"
#include "../Utility/task.h"

class GraphManager;
class Player;

class UI : public Task
{
private:

	enum
	{
		SIDES,
		FLEAM,
		ROGO,
		NUM,
		GAUGE_SHEET,
		GAUGE,

		UI_NUM,
	};

	Player *player;
	Texture *uiTexture[UI_NUM];
	GraphManager *graphManager;
public:

	UI(Player *argPlayer);
	~UI() {}

	void initialize() override;
	bool update() override;
	void draw() override;
	void finalize() override;
};