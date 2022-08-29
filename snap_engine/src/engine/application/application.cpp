#include "application.h"

application::application()
{
	// setup logger system
	Log::init();

	// setup window system
	win = std::make_shared<window>("shooter game", 800, 600);

	// setup input system
	InputSystem& inputSystem = InputSystem::getInstance();
	inputSystem.init(win);

	// setup time system
	Time::time_init(60);

	// setup renderer sysytem
	renderer::renderer_init();
}

application::~application()
{
	renderer::free_memory();
}