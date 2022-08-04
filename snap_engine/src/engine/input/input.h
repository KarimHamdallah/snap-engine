#pragma once
#include <engine/defines.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <engine/display/window.h>
#include "keys.h"

class InputSystem
{
public:
	InputSystem() {};
	virtual ~InputSystem() {};
public:
	void init(std::shared_ptr<window> _window);
	void cleanup();
public:
	bool isKeyPressed(Key keycode);
	bool isKeyReleased(Key keycode);
	bool isKeyHeld(Key keycode);
	bool isKeyChanged(Key keycode);
	bool isKeyDown(Key keycode);

	bool isButtonPressed(MouseButton button);
	bool isButtonReleased(MouseButton button);
	bool isButtonDown(MouseButton button);

	KeyState getKeyState(Key keycode);
	ButtonState getMouseButtonState(MouseButton button);

	glm::vec2 getMousePos();
	glm::vec2 getMouseScroll();
	glm::vec2 getMouseRelativePos();

public:
	void setKeyState(Key keycode, KeyState state);
	void setMouseButtonState(MouseButton button, ButtonState state);
	void setKeyChanged(Key keycode, bool changed);
	void setButtonPressedThisFrame(MouseButton button, bool pressed_this_frame);
	void setMousePos(f32 x_pos, f32 y_pos, f32 x_rel, f32 y_rel);
	void setMouseScroll(f32 x, f32 y);
public:
	static InputSystem& getInstance();
private:
	std::unordered_map<Key, KeyState> m_keys;
	std::unordered_map<Key, bool> m_keyscahnged;
	std::unordered_map<MouseButton, ButtonState> m_mouseButtons;
	std::unordered_map<MouseButton, bool> m_mouseButtonsPressedThisFrame;

	f32 m_mouseX, m_mouseY, m_scrollX, m_scrollY, m_mouse_relativeX, m_mouse_relativeY;
};

class Input
{
public:
	static bool isKeyPressed(Key keycode);
	static bool isKeyReleased(Key keycode);
	static bool isKeyHeld(Key keycode);
	static bool isKeyChanged(Key keycode);
	static bool isKeyDown(Key keycode);

	static bool isButtonPressed(MouseButton button);
	static bool isButtonReleased(MouseButton button);
	static bool isButtonDown(MouseButton button);
public:
	static void setKeyState(Key keycode, KeyState state);
	static void setKeyChanged(Key keycode, bool changed);
	static void setMouseButtonState(MouseButton button, ButtonState state);
	static void setButtonPressedThisFrame(MouseButton button, bool pressed_this_frame);

	static glm::vec2 getMousePos();
	static glm::vec2 getMouseScroll();
	static glm::vec2 getMouseRelativePos();
	static void show_cursor(bool show);

public:
	static void setMousePos(f32 x_pos, f32 y_pos, f32 x_rel, f32 y_rel);
	static void setMouseScroll(f32 x, f32 y);
private:
	Input() = delete;
	~Input() = delete;
};