#include <iostream>
#include <engine/display/window.h>
#include <engine/renderer/renderer.h>
#include <engine/logger/Logger.h>
#include <engine/input/input.h>
#include <engine/time/Time.h>
#include <engine/physics/physics.h>
#include <engine/renderer/instance_renderer.h>

void on_resize(u32 new_width, u32 new_height)
{
}

int main(int argc, char* argv[])
{
	srand(SDL_GetTicks());
	// setup logger system
	Log::init();

	// setup window system
	std::shared_ptr<window> game_widnow = std::make_shared<window>("snap engine", 800, 600);
	game_widnow->setWindowResizeCallbackPointer(on_resize);

	// setup input system
	InputSystem& inputSystem = InputSystem::getInstance();
	inputSystem.init(game_widnow);

	// setup time system
	Time::time_init(60);

	// setup renderer sysytem
	renderer::renderer_init();
	instance_renderer::instance_renderer_init();

	// setup physics system
	physicsWorld::physics_init();


	glm::vec2 window_center = glm::vec2(game_widnow->getInstance()->getWidth() * 0.5f, game_widnow->getInstance()->getHeight() * 0.5f);





	// TODO: REMOVE
	texture tex = renderer::creat_texture("assets/textures/snap_engine.jpg");






	while (!game_widnow->shouldClose())
	{
		Time::time_update();
		game_widnow->update();

		// input
		if(Input::isKeyPressed(Key::EscapeKey))
			game_widnow->setShouldClose(true);
		Input::show_cursor(false);

		// update physics
		physicsWorld::physics_update(Time::m_deltaTime);


		// TODO:: REMOVE
		// update

		// renderering
		renderer::renderer_begin();
		// TODO:: REMOVE
		renderer::render_sprite(&tex, window_center, glm::vec2(tex.width, tex.height));
		renderer::renderer_end(game_widnow->getRenderingWindow());

		Time::time_update_late();
	}
	return 0;
}