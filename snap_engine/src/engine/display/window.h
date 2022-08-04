#pragma once
#include <SDL2/SDL.h>
#include <engine/defines.h>
#include <string>


class window
{
public:
	window() = default;
	window(const std::string& title, const u32& width, const u32& height);
	~window();
	void update();


	inline void setShouldClose(bool should_close) { m_running = !should_close; }
	inline bool shouldClose() { return (m_running) ? false : true; }

	inline u32 getWidth() { return m_width; }
	inline u32 getHeight() { return m_height; }

	inline static window* getInstance() { return m_Instance; }

	inline SDL_Window* getRenderingWindow() { return m_window; }

	inline void setWindowResizeCallbackPointer(void(*fp)(u32, u32)) { on_window_resize = fp; }
	inline void setWindowKeyCallbackPointer(void(*fp)(i32, i32, u8, u16, u8)) { on_key_dwon = fp; }
	inline void setWindowCursorPosCallbackPointer(void(*fp)(f32, f32, f32, f32)) { on_cursor_pos = fp; }
	inline void setWindowMouseScrollCallbackPointer(void(*fp)(f32, f32)) { on_mouse_scroll = fp; }
	inline void setWindowMouseButtonCallbackPointer(void(*fp)(u8, u8)) { on_button_down = fp; }
private:
	void shutdown();
	bool init();

	u32 m_width;
	u32 m_height;
	std::string m_title;
	SDL_Window* m_window;

	bool m_running;

	static window* m_Instance;

	// window function pointers
	void(*on_window_resize)(u32, u32); // new_width new_height
	void(*on_key_dwon)(i32, i32, u8, u16, u8); // key scancode action mod repeat
	void(*on_cursor_pos)(f32, f32, f32, f32); // xpos ypos relative_x relative_y
	void(*on_mouse_scroll)(f32, f32); // scrollx scroly
	void(*on_button_down)(u8, u8); // button action
};