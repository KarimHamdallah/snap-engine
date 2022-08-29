#pragma once
#include "game_play.h"
#include <engine/defines.h>

class main_menu : public game_state
{
public:
	virtual void init();
	virtual void update();
	virtual void render();
	virtual void shutdown();

private:
};