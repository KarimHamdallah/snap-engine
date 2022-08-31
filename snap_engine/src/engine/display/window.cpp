#include "window.h"
#include <GLEW/glew.h>
#include <SDL2/SDL_opengl.h>
#include <engine/logger/asserts.h>

window* window::m_Instance = nullptr;

window::window(const std::string & title, const u32 & width, const u32 & height)
	: m_title(title), m_window(nullptr), m_running(false)
{
	m_width = width;
	m_height = height;

	m_running = init();
	on_window_resize = nullptr;
	on_key_dwon = nullptr;
	on_cursor_pos = nullptr;
	on_mouse_scroll = nullptr;
	on_button_down = nullptr;
}

window::~window()
{
	shutdown();
}

void window::update()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
		{
			m_running = false;
		}
		break;
		case SDL_WINDOWEVENT:
		{
			if (e.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				m_width = e.window.data1;
				m_height = e.window.data2;

				if(on_window_resize != nullptr)
					on_window_resize(e.window.data1, e.window.data2);

				// renderer essential data to be changed upon window resize
				glViewport(0, 0, m_width, m_height);
			}
		}
		break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
		{
				if (on_key_dwon != nullptr)
					on_key_dwon(e.key.keysym.sym, e.key.keysym.scancode, e.key.state, e.key.keysym.mod, e.key.repeat);
		}
		break;
		case SDL_MOUSEMOTION:
		{
			if(on_cursor_pos != nullptr)
				on_cursor_pos((f32)e.motion.x, m_height - (f32)e.motion.y, (f32)e.motion.xrel, (f32)-e.motion.yrel);
		}
		break;
		case SDL_MOUSEWHEEL:
		{
			if (on_mouse_scroll != nullptr)
				on_mouse_scroll((f32)e.wheel.x, (f32)-e.wheel.y);
		}
		break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		{
			if (on_button_down != nullptr)
				on_button_down(e.button.button, e.button.state);
		}
		break;
		default:
			break;
		}
	}
}

void window::shutdown()
{
	SDL_DestroyWindow(m_window);
}

bool window::init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		LOG_ASSERT("Failed to initialize SDL\n");
		return false;
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);


	m_window = SDL_CreateWindow
	(
		"shooter game",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		m_width,
		m_height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);

	if (m_window == NULL)
	{
		LOG_ASSERT("Failed to creat SDL Window\n");
		return false;
	}
	SDL_GL_CreateContext(m_window);

	m_Instance = this;

	return true;
}