#include <iostream>
#include <engine/display/window.h>
#include <engine/renderer/renderer.h>
#include <engine/defines.h>
#include <engine/logger/Logger.h>
#include <engine/input/input.h>
#include <engine/time/Time.h>
#include <engine/physics/collisions.h>
#include <engine/renderer/instance_renderer.h>

void on_resize(u32 new_width, u32 new_height)
{
}

int main(int argc, char* argv[])
{
	// setup logger system
	Log::init();

	// setup window system
	std::shared_ptr<window> game_widnow = std::make_shared<window>("shooter game", 800, 600);
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


	glm::vec2 start = glm::vec2(50.0f, 100.0f);
	glm::vec2 end = glm::vec2(0.0f);
	AABB center_aabb = AABB{ window_center, glm::vec2(50.0f, 80.0f) };



	// TODO: REMOVE
	while (!game_widnow->shouldClose())
	{
		Time::time_update();
		game_widnow->update();

		// input
		if(Input::isKeyPressed(Key::EscapeKey))
			game_widnow->setShouldClose(true);
		Input::show_cursor(false);

		// TODO:: REMOVE
		// update
		end = Input::getMousePos();

		// renderering
		renderer::renderer_begin();
		// TODO:: REMOVE
		
		if (Input::isKeyPressed(Key::Enter))
			f32 x = 10.0f;

		RayCastHit hit = collisions::line_vs_AABB(start, end, center_aabb);

		renderer::render_aabb(center_aabb, WHITE);
		renderer::render_line(start, end, hit.hit ? BLUE : WHITE);
		renderer::render_quad(hit.exit_point, glm::vec2(10.0f), RED);
		renderer::render_quad(hit.enter_point, glm::vec2(10.0f), GREEN);


		renderer::renderer_end(game_widnow->getRenderingWindow());

		Time::time_update_late();
	}

	renderer::free_memory();
	physicsWorld::free_memory();

	return 0;
}