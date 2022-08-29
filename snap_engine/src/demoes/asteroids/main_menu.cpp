#include "main_menu.h"
#include <snap_engine.h>
#include "asteroid.h"

void main_menu::init()
{
	Input::show_cursor(true);
}

void main_menu::update()
{
	if (Input::isKeyDown(Enter))
		GameStateManger.set_state(new game_play());
}

void main_menu::render()
{
	renderer::render_wideline(glm::vec2(100.0f, 50.0f), glm::vec2(500.0f, 50.0f), WHITE);
}

void main_menu::shutdown()
{
	LOG_CRITICAL("main_menu shutdown");
}