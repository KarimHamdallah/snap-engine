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

void input_handel(Body* player_body)
{
	f32 velx = 0.0f;
	f32 vely = player_body->velocity.y;

	if (Input::isKeyPressed(Key::D))
		physicsWorld::addForce(player_body, glm::vec2(200.0f, 0.0f));//velx += 200.0f;
	if (Input::isKeyPressed(Key::A))
		velx -= 200.0f;
	if (Input::isKeyDown(Key::W))
		vely = 1000.0f; //physicsWorld::addForce(player_body, glm::vec2(0.0f, 1000.0f));
	if (Input::isKeyPressed(Key::S))
		vely -= 100.0f;

	player_body->velocity.x = velx;
	player_body->velocity.y = vely;
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





	// TODO: REMOVE
	u32 body_id = physicsWorld::pushBody(glm::vec2(100, 500), glm::vec2(50.0f));

	f32 width = game_widnow->getWidth();
	f32 height = game_widnow->getHeight();

	u32 static_body_a_id = physicsWorld::pushStaticBody(glm::vec2( width * 0.5 - 25, height - 25 ), glm::vec2( width - 50, 50  ));
	u32 static_body_b_id = physicsWorld::pushStaticBody(glm::vec2( width - 25, height * 0.5 + 25 ), glm::vec2( 50, height - 50 ));
	u32 static_body_c_id = physicsWorld::pushStaticBody(glm::vec2( width * 0.5 + 25, 25          ), glm::vec2( width - 50, 50  ));
	u32 static_body_d_id = physicsWorld::pushStaticBody(glm::vec2( 25, height * 0.5 - 25         ), glm::vec2( 50, height - 50 ));
	u32 static_body_e_id = physicsWorld::pushStaticBody(glm::vec2( width * 0.5, height * 0.5     ), glm::vec2( 150, 150        ));

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
		Body* player = physicsWorld::getBody(body_id);
		input_handel(player);
		
		physicsWorld::physics_update(Time::m_deltaTime);

		LOG_INFO("{0}, {1}", player->velocity.x, player->velocity.y);

		// renderering
		renderer::renderer_begin();
		// TODO:: REMOVE

		for (size_t i = 0; i < physicsWorld::getStaticBodies_count(); i++)
		{
			StaticBody* b = physicsWorld::getStaticBody(i);
			renderer::render_aabb(b->aabb, RED);
		}
		for (size_t i = 0; i < physicsWorld::getBodies_count(); i++)
		{
			Body* b = physicsWorld::getBody(i);
			renderer::render_aabb(b->aabb, WHITE);
		}

		renderer::render_smoothcircle(window_center, 100.0f, RED, 1.0f);

		renderer::renderer_end(game_widnow->getRenderingWindow());

		Time::time_update_late();
	}

	physicsWorld::free_memory();

	return 0;
}